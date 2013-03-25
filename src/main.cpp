#include "Devices.h"
#include "Commands.h"
#include "State.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

using namespace Jarvis;
using namespace Jarvis::Devices;
using namespace std;
void tokenize(string line, list<string>&tokens){
    istringstream stream(line);
    while (stream.good()){
        string token;
        stream >> token;
        tokens.push_back(token);
    }
}

//return 0 if true
int runCommand(string line){
    //tokenize?
    list<string>tokens;
    tokenize(line, tokens);

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
    tokens.pop_front();
    if (size == 1){
        if (first == "") return 0;
        else if (first == "exit"){
            return 1;
        }
        else if (first == "clear"){
            //reset device to new
            Device *device = new Device();
            //State::instance();
            State::instance().device(device);
            return 0;
        }
        else{
            return 2;
        }
    }
    else{
        if (first == "load"){
            runLoad(tokens);
        }
        else if (first == "link"){

        }
        else if (first == "save"){

        }
        else{
            return 2;
        }
    }
}

//read off of a batch file
int runScript(char *fileName){
    string line;
    ifstream stream(fileName);

    if (stream.is_open()){
        while (stream.good()){
            getline(stream, line);
            int result = runCommand(line);
            if (result !=0) return result;
        }
    }
    else{
        cerr << "Unable to open script: "<<fileName<<endl;
        return 3;
    }
    return 0;
}

int runShell(){
    string line;
    cout << ">> ";
    while (getline(cin,line)){
        //process command
        int result = runCommand(line);
        if (result != 0) return result;
        cout << ">> ";
        
    }
    return 0;
}

void printHelp(){
    cout << "Usage:"<<endl;
    cout << "jarvis" <<endl;
    cout << "jarvis script"<<endl;
}

int main(int argc, char *argv[]){
    //file name
    if (argc == 1){
        runShell();
    }
    else if (argc == 2){
        runScript(argv[1]);
    }
    else{
        printHelp();
    }
    //some component
    //construct something.
    //Wire *wire = new Wire();
    
    return 0;

}
