#ifndef JARVIS_COMMANDS_H
#define JARVIS_COMMANDS_H

#include <list>
#include <string>

using namespace std;
namespace Jarvis{
    /*
    class LoadCommand{
        public:
            LoadCommand(list<string> commands);
            string type(){return type_;}
            string name(){return name_;}
            list<string> args(){return args_;}
        private:
            string type_;
            string name_;
            list<string> args_;
    }; */
    //int runLoad(LoadCommand cmd);
    int runLoad(list<string> command);
    
}
#endif
