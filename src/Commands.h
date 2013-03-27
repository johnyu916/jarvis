#ifndef JARVIS_COMMANDS_H
#define JARVIS_COMMANDS_H

#include <list>
#include <string>

using namespace std;
namespace Jarvis{
    class Command{
        public:
            Command(string line, Device* device);
            list<string>& tokens(){return tokens_};
            Device *device(){return device_;}
        private:
            list<string> tokens_;
            Device *device_;
    };
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
    int runLoad(Command cmd);
    int runLink(Command command);
    int runLabel(Command command);
    int runSave(Command command);
    
    //int runLoad(list<string> command);
    
}
#endif