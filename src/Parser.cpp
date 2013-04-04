#include <fstream>
#include <sstream>
#include <iostream>

#include "Parser.h"
namespace Jarvis{
    using namespace Devices;
    //return 0 if true
    int runCommand(Command command){
        //tokenize?
        list<string>& tokens = command.tokens();
        //tokenize(line, tokens);

        /*
        cout <<"contents of the command: "<<endl;
        for (int a = 0; a < tokens.size(); a++){
            cout <<tokens[a]<<endl;
        }
        */

        //process instructions
        int size = tokens.size();
        if (size == 0){
            //do nothing
            return 0;
        }
        string first = tokens.front();
        if (first[0] == '#') return 0;
        //command.tokens().pop_front();
        tokens.pop_front();
      
       /*
        cout <<"tokens left: "; 
        list<string>::iterator it;
        for (it = tokens.begin(); it != tokens.end(); it++){
            cout <<(*it)<<" ";
        }
        cout <<endl;
        */
        if (size == 1){
            if (first == "") return 0;
            else if (first == "exit"){
                return -1;
            }
            else if (first == "clear"){
                //reset device to new
                //Device *device = new Device();
                //State::instance().device(device);
                //command.device()->clear();
                deviceClear(command.device());
                return 0;
            }
            else if (first == "show"){
                return runShow(command);
            }
            else{
                cerr <<"Unknown command: "<<first<<endl;
                return 2;
            }
        }
        else{
            if (first == "load"){
                return runLoad(command);
            }
            else if (first == "link"){
                return runLink(command);
            }
            else if (first == "bridge"){
                return runBridge(command);
            }
            else if (first == "save"){
                return runSave(command);
            }
            /*
            else if (first == "set"){
                return runSet(command);
            }
            */
            else if (first == "unlink"){
                return runUnlink(command);
            }
            else if (first == "run"){
                return runRun(command); 
            }
            else if (first == "show"){
                return runShow(command);
            }
            else{
                cerr <<"Unknown command(2): "<<first<<endl;
                return 2;
            }
        }
    }
    int runScript(string fileName){
        Device *device = new Device("jarvis", "canvas");
        return runScript(fileName, device);
    }

    int runScript(string fileName, Device *device){
        string line;
        ifstream stream(fileName.c_str());

        if (stream.is_open()){
            while (stream.good()){
                getline(stream, line);
                Command command(line, device);
                //cout << line <<endl;
                int result = runCommand(command);
                if (result !=0) return result;
            }
        }
        else{
            cerr << "Unable to open script: "<<fileName<<endl;
            return 3;
        }
        return 0;
    }
}
