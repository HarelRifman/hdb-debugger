#ifndef ELF_LOADER_H
#define ELF_LOADER_H

#include <cstdint>
#include <string>
#include <sys/types.h>

// True if the ELF file at `path` is ET_DYN (a PIE executable, or a shared
// object -- since this is only ever called on the program we just
// execve()'d, ET_DYN here always means "PIE"). ET_EXEC binaries are always
// loaded at their link-time addresses, so they never need a bias.
bool elf_is_pie(const std::string& path);

// Lowest virtual address    the ELF's PT_LOAD segments: the address the
// linker assumed the first loaded byte would sit at. For PIE binaries this
// is what nm/objdump/readelf report; the kernel is free to load the image
// somewhere else entirely at runtime.
uintptr_t elf_min_load_vaddr(const std::string& path);

// Lowest mapped address of `abs_path` in the running process `pid`, read
// from /proc/[pid]/maps -- i.e. where the kernel actually put the image
// this run. Requires an absolute, canonicalized path (matching how the
// kernel renders it in /proc/[pid]/maps).
uintptr_t proc_maps_base_address(pid_t pid, const std::string& abs_path);

// Runtime load bias for `binary_path` running as `pid`:
//   bias = runtime_base - elf_vaddr_base
// Add this to any static address (from nm/objdump/readelf) to get the
// address it actually lives at in this run; subtract it from a runtime
// address (e.g. RIP) to get back the static address. Non-PIE binaries
// always have bias 0.
uintptr_t compute_load_bias(pid_t pid, const std::string& binary_path);

#endif