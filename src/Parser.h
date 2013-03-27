#ifndef JARVIS_PARSER_H
#define JARVIS_PARSER_H


namespace Jarvis{
    //read off of a batch file
    int runCommand(string line);
    int runScript(char *fileName);
    int runScript(char *fileName, Device *device);
}

#endif
