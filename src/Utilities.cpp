#include "Utilities.h"
#include <iostream>
namespace Jarvis{
    void listCerr(list<string>& tokens){
        list<string>::iterator it;
        for (it = tokens.begin(); it != tokens.end(); it++){
            string word = (*it);
            cerr << word << " ";
        }
        cerr <<endl;
    }
    bool isNumber(const string& s)
    {
            string::const_iterator it = s.begin();
            while (it != s.end() && std::isdigit(*it)) ++it;
            return !s.empty() && it == s.end();
    }
}
