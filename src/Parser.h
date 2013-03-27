#ifndef JARVIS_PARSER_H
#define JARVIS_PARSER_H

#include "Commands.h"

namespace Jarvis{
    using namespace Devices;
    //read off of a batch file
    int runCommand(Command command);
    int runScript(string fileName);
    int runScript(string fileName, Device *device);
}

#endif
