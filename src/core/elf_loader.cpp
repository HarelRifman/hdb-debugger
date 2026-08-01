#include "elf_loader.h"

#include <climits>
#include <cstdlib>
#include <cstring>
#include <elf.h>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

namespace {

bool read_ehdr(const string& path, Elf64_Ehdr& ehdr) {
    ifstream file(path, ios::binary);
    if (!file) {
        cerr << "Error: Failed to open '" << path << "' to read ELF header" << endl;
        return false;
    }
    file.read(reinterpret_cast<char*>(&ehdr), sizeof(ehdr));
    if (!file || memcmp(ehdr.e_ident, ELFMAG, SELFMAG) != 0) {
        cerr << "Error: '" << path << "' is not a valid ELF file" << endl;
        return false;
    }
    return true;
}

}  // namespace

bool elf_is_pie(const string& path) {
    Elf64_Ehdr ehdr;
    if (!read_ehdr(path, ehdr)) {
        return false;
    }
    return ehdr.e_type == ET_DYN;
}

uintptr_t elf_min_load_vaddr(const string& path) {
    Elf64_Ehdr ehdr;
    if (!read_ehdr(path, ehdr)) {
        return 0;
    }

    ifstream file(path, ios::binary);
    file.seekg(ehdr.e_phoff);

    uintptr_t min_vaddr = UINTPTR_MAX;
    for (int i = 0; i < ehdr.e_phnum; i++) {
        Elf64_Phdr phdr;
        file.read(reinterpret_cast<char*>(&phdr), ehdr.e_phentsize);
        if (!file) {
            break;
        }
        if (phdr.p_type == PT_LOAD && phdr.p_vaddr < min_vaddr) {
            min_vaddr = phdr.p_vaddr;
        }
    }

    return min_vaddr == UINTPTR_MAX ? 0 : min_vaddr;
}

uintptr_t proc_maps_base_address(pid_t pid, const string& abs_path) {
    ostringstream maps_path;
    maps_path << "/proc/" << pid << "/maps";

    ifstream maps(maps_path.str());
    if (!maps) {
        cerr << "Error: Failed to open " << maps_path.str() << endl;
        return 0;
    }

    // Each line looks like:
    //   555555554000-555555555000 r--p 00000000 08:01 123  /path/to/binary
    // The pathname is only present for file-backed mappings and is the
    // last field, so the first line containing our absolute path is the
    // binary's lowest-addressed segment (they're listed in address order).
    string line;
    while (getline(maps, line)) {
        if (line.find(abs_path) == string::npos) {
            continue;
        }
        return stoull(line.substr(0, line.find('-')), nullptr, 16);
    }

    cerr << "Error: No mapping for '" << abs_path << "' found in " << maps_path.str() << endl;
    return 0;
}

uintptr_t compute_load_bias(pid_t pid, const string& binary_path) {
    char resolved[PATH_MAX];
    if (!realpath(binary_path.c_str(), resolved)) {
        cerr << "Error: Failed to resolve path '" << binary_path << "'" << endl;
        return 0;
    }
    string abs_path(resolved);

    if (!elf_is_pie(binary_path)) {
        cout << "[hdb] Non-PIE binary (ET_EXEC) -- load bias = 0x0" << endl;
        return 0;
    }

    uintptr_t elf_vaddr_base = elf_min_load_vaddr(binary_path);
    uintptr_t runtime_base = proc_maps_base_address(pid, abs_path);
    uintptr_t bias = runtime_base - elf_vaddr_base;

    cout << "[hdb] PIE binary (ET_DYN) detected -- runtime base 0x" << hex << runtime_base
         << ", link-time base 0x" << elf_vaddr_base << ", load bias = 0x" << bias << dec << endl;
    return bias;
}