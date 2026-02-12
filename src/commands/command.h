#ifndef COMMAND_H
#define COMMAND_H

#include <vector>
#include <string>

using namespace std;

class Debugger;  // Forward declaration

class Command {
public:
    virtual void execute(Debugger& debugger, const vector<string>& args) = 0;
    virtual ~Command() = default;
};

#endif
