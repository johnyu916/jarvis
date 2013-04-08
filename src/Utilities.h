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

    class vout{
        public:
        vout(){}
        /*
        static void print(...){
            va_list args;
            va_start(args);
            vprintf(args);
            va_end(args);
        }
        template<typename T>
        vout& operator<<(T const& t){
            if (Settings::verbose()){
                cout << t;
            }
            return *this;
        }*/
    
    };
}

#endif
