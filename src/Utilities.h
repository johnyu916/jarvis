#ifndef JARVIS_UTILITIES
#define JARVIS_UTILITIES

#include <cstdarg>
#include <iostream>
#include <list>
#include <string>

#include "Settings.h"

#define debug(str) if (Settings::instance().verbose()) std::cout <<str <<std::endl;
using namespace std;
namespace Jarvis{

    void listCerr(list<string>& tokens);
    bool isNumber(const string& s);
}

#endif
