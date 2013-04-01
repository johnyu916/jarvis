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
}
