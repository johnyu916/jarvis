#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>

#include "Commands.h"
#include "Devices.h"
#include "Parser.h"
#include "Settings.h"
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
        // -1 is special status from exit
        if (result == -1) return result;
        //if (result != 0) return result;
        cout << ">> ";
        
    }
    return 0;
}

void printHelp(){
    cout << "Usage:"<<endl;
    cout << "jarvis" <<endl;
    cout << "jarvis script"<<endl;
    cout << "-v for verbose, -h for help"<<endl;
}

int main(int argc, char *argv[]){

    char c;
    while ((c = getopt(argc, argv, "vh")) != -1){
        switch (c){
            case 'v':
                cout <<"verbose on"<<endl;
                Settings::instance().verbose(true);
                break;
            case 'h':
                printHelp();
                return 0;
                break;
            default:
                printHelp();
                return 100;
        }
    }

    if (optind < argc) return runScript(argv[optind]);
    else return runShell();
    
    //file name
    /*
    if (argc == 1){
        return runShell();
    }
    else if (argc == 2){
        return runScript(argv[1]);
    }
    else{
        printHelp();
    }
    */
    //some component
    //construct something.
    
    return 0;
}
