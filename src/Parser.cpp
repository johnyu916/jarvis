#include <fstream>
#include <sstream>
#include <iostream>

#include "Parser.h"
#include "Utilities.h"
namespace Jarvis{
    using namespace Devices;
    //return 0 if true
    int runCommand(Command command){
        //tokenize?
        list<string>& tokens = command.tokens();

        /*
        list<string>::iterator sit;
        cout <<"contents of the command: "<<endl;
        for (sit = tokens.begin(); sit != tokens.end(); sit++){
            string token = *sit;
            cout <<token<<" size: "<<token.size()<<endl;
        }
        cout <<"contents end"<<endl;
        */

        //process instructions
        int size = tokens.size();
        if (size == 0){
            //cout <<"no tokens"<<endl;
            //do nothing
            return 0;
        }
        string first = tokens.front();
        tokens.pop_front();

        if (first[0] == '#') return 0;
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
        else if (first == "load"){
            return runLoad(command);
        }
        else if (first == "link"){
            return runLink(command);
        }
        else if (first == "bridge"){
            return runBridge(command);
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
        else if (first == "save"){
            return runSave(command);
        }
        else if (first == "show"){
            return runShow(command);
        }
        else if (first == "verbose"){
            return runVerbose(command);
        }
        else{
            cerr <<"Unknown command(2): "<<first<<endl;
            return 2;
        }
    }
    int runScript(string fileName){
        Device *device = new Device("jarvis", "canvas", NULL);
        return runScript(fileName, device);
    }

    int runScript(string fileName, Device *device){
        string line;
        ifstream stream(fileName.c_str());

        if (stream.is_open()){
            while (stream.good()){
                getline(stream, line);
                Command command(line, device);
                debug("command: "<< line);
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
