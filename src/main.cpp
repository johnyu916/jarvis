#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

#include "Commands.h"
#include "Devices.h"
#include "Parser.h"
#include "State.h"

using namespace Jarvis;
using namespace Jarvis::Devices;
using namespace std;


int runShell(){
    string line;
    Device* device = new Device();
    cout << ">> ";
    while (getline(cin,line)){
        //process command
        Command command(line,device);
        int result = runCommand(command);
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
