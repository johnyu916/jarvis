#include "Commands.h"
#include "Compute.h"
#include "Parser.h"
#include "State.h"
#include "Utilities.h"
#include <fstream>
#include <iostream>
#include <sstream>

namespace Jarvis{
    using namespace Devices;

    void tokenize(string line, list<string>&tokens){
        istringstream stream(line);
        while (stream.good()){
            string token;
            stream >> token;
            tokens.push_back(token);
        }
    }

    Command::Command(string line, Device* device){
        //lets set the tokens
        tokenize(line, tokens_);
        device_=device;
    }

    /*
    LoadCommand::LoadCommand(list<string> commands){
        type_="";
        name_="";
        
        //must be at least two
    }*/

    //get pin from current canvas
    Pin *getPinWithCommand(Command& cmd){
        list<string>& command = cmd.tokens();
        string nameOne = command.front();
        command.pop_front();
        Device *canvas = cmd.device();
        Element *oneElement = canvas->element(nameOne);

        if (oneElement != NULL){
            string type = oneElement->type();
            //cout <<"element found type: "<<type<<endl;
            if (type == "switch"){
                Switch *swit = (Switch *)oneElement;
                //get second word
                string pinName = command.front();
                command.pop_front();
                return swit->pin(pinName);
            }
            else if (type == "power"){
                Power *power = (Power *)oneElement;
                string pinName = command.front();
                command.pop_front();
                return power->pin(pinName);
            }
                /*
                else if (type == "GROUND"){
                    Ground *ground = (Ground *)oneElement;
                    return ground->pin();

                }
                */
            else if (type == "meter"){
                Meter *meter = (Meter *)oneElement;
                return meter->pin();
            }
            else if (type == "resistor"){
                Resistor *resistor = (Resistor *)oneElement;
                string pinName = command.front();
                command.pop_front();
                return resistor->pin(pinName);
            }
            /*
            else if (type == "input"){
                Input *input  = (Input *)oneElement;
                return input->pin();
            }*/
            else{
                cerr << "Unknown element type: "<<type;
                //hmm this is not possible
                return NULL;
            }
        }
        else { //must be device

            Device *oneDevice = canvas->device(nameOne);
            Pin *pinOne;
            if (oneDevice == NULL){
                cerr << "unable to find device with name: "<<nameOne<<endl;
                return NULL;
            }
            //do we need to get the pin?
            string type = oneDevice->type();
            //int size = oneDevice->pinLabels().size();
            //int size = oneDevice->bridges().size();

            /*
            if (size == 0){
                cerr <<"Device with name: "<<nameOne<< " has no pinLabels"<<endl;
                return NULL;
            }
            else if (size == 1){
                return oneDevice->bridges().front()->out();
            }
            else{
                */
                string pinName = command.front();
                command.pop_front();
                //Bridge *bridge = oneDevice->bridge(pinName);
                Element *bridgeE = oneDevice->element(pinName);
                if (bridgeE == NULL){
                    cerr << "No such element found" << pinName <<endl;
                }
                //PinLabel *pinLabel = oneDevice->pinLabel(pinName);
                Bridge *bridge = (Bridge *)bridgeE;
                if (bridge == NULL){
                    cerr<< "No bridge found for pin name: "<<pinName<<endl;
                    return NULL;
                }
                return bridge->out();
            //}
        }
    }

    int runLoad(Command cmd){
   // int runLoad(list<string> command){
       //cout <<"in runLoad"<<endl;
        

       list<string> command = cmd.tokens();
        int size = command.size();
        if (size != 2) {
            cerr << "Must have at least 2 arguments but has: "<<size<<" "<<endl;
            list<string>::iterator it;
            for (it = command.begin(); it != command.end(); it++){
                cerr <<(*it)<<" ";
            }
            cerr <<endl;
            return 13;
        }
        
        string type = command.front();
        command.pop_front();
        string name = command.front();
        command.pop_front();

        Device *canvas = cmd.device();

        //ensure name is not already taken
        bool isExist = isExistWithName(canvas, name);
        if (isExist) {
            cerr << "Part with name: " <<name<< " already exists"<<endl;
            return 6;
        }
        Element *element;

        if (size == 2){
            //this cannot be wire
            /*
            if (type == "wire"){
                cerr << " "<<endl;
                return 5;
            }
            if (type == "input"){
                element = new Input(name);
            }*/
            if (type == "meter"){
                element = new Meter(name);
            }
            else if (type == "power"){
                //cout <<"in runLoad power"<<endl;
                element = new Power(name);
            }
            else if (type == "resistor"){
                element = new Resistor(name);
            }
            else if (type == "switch"){
                element = new Switch(name);
            }
            else{
                //this may be a device. look for it.
                string filename = "descriptions/";
                filename.append(type);
                filename.append(".desc");
                //type.append(".desc");
                Device *newDevice = new Device(name, type);
                int result = runScript(filename, newDevice);
                if (result == 0){
                    canvas->devices().push_back(newDevice);
                    return 0;
                }
                else{
                //bleh
                    cerr << "failed to run script: "<<type<<endl;
                    return 7;
                }
            }
            canvas->elements().push_back(element);
            return 0;
        }
   }
   /*
    int runSet(Command command){
       list<string> tokens = command.tokens();
        int size = tokens.size();
        
        if (size != 2) return 20;
        string name = tokens.front();
        tokens.pop_front();

        Element *element = command.device()->element(name);
        if (element == NULL || element->type() != "input") return 24;

        Input *input = (Input *)element;

        if (input == NULL) return 21;
    
        string value = tokens.front();
        tokens.pop_front();

        if (value == "high") input->state(true);
        else if (value == "low") input->state(false);
        else return 22;
        return 0;
    }
    */
   int runLink(Command command){
        //needs at least 2 more
       list<string>& tokens = command.tokens();
        int size = tokens.size();
        
        if (size <= 1 || size > 4){
            cerr<< "There should be 2-4 arguments, but there are: "<<size<<endl;
            return 8;
        }
        //get first pin
        Pin *pinOne = getPinWithCommand(command);
        if (pinOne == NULL) {
            cerr<< "Couldn't find pin one" <<endl;
            return 9;
        }
        //get second pin
        Pin *pinTwo = getPinWithCommand(command);
        if (pinTwo == NULL) {
            cerr << "Couldn't find pin two" <<endl;
            return 11;
        }
        //it is okay of one or neither of them have wires. but it is not okay if both have wires
        return linkPins(pinOne, pinTwo);
            
    }
    int runUnlink(Command command){
        list<string> tokens = command.tokens();
        int size = tokens.size();
        if (size <= 1 || size > 2){
            cerr << "size needs to be 1 or 2, but it is: "<<size<<". list: ";
            listCerr(tokens);
            return 19;
        }
        Pin *pinOne = getPinWithCommand(command);

        return unlinkPin(pinOne);     
    }
    int runBridge(Command command){
        Pin *pinOne = getPinWithCommand(command);
        if (pinOne == NULL) {
            cerr <<"runLabel: could not find pin one"<<endl;
            return 14;
        }
        list<string>tokens = command.tokens();

        string name = tokens.front();
        tokens.pop_front();
        Device *device = command.device();
        Bridge *bridge = new Bridge(name, pinOne);
        device->elements().push_back(bridge);
        //device->bridges().push_back(bridge);
        //PinLabel* label = new PinLabel(name,pinOne);
        //device->pinLabels().push_back(label);
        return 0;
        
    }
    /*
    int runLabel(Command command){
        //must be at lest two
        Pin *pinOne = getPinWithCommand(command);
        if (pinOne == NULL) {
            cerr <<"runLabel: could not find pin one"<<endl;
            return 14;
        }
        list<string>tokens = command.tokens();

        string name = tokens.front();
        tokens.pop_front();
        Device *device = command.device();
        PinLabel* label = new PinLabel(name,pinOne);
        device->pinLabels().push_back(label);
        return 0;
    }*/
    int runRun(Command command){
        //lets run
        list<string>tokens = command.tokens();
        if (tokens.size() == 0) {
            cerr << "runRun: no tokens left"<<endl;
            return 16;
        }
        string stepsString = tokens.front();
        int steps=0;
        istringstream(stepsString) >> steps;

        //turn on first
        while (steps > 0){
            cout <<"steps left: "<<steps<<endl;
            int rVal = compute(command.device());
            if (rVal != 0) return rVal;
            steps--;
        }
        
        return 0;
    }
    int runSave(Command command){
        return 0;
        //save to file
        list<string>tokens = command.tokens();
        if (tokens.size() == 0) return 16;
        string name ="descriptions/"; 
        name.append(tokens.front());

        if (name == "") return 17;
        name.append(".desc"); 
        //see if file exists. if exists, stop.
        ifstream input(name.c_str());
        if (input.is_open()){
            input.close();
            return 18;
        }

       // ofstream out(name);
        
    }

    void printDevice(Device *device){
        //print following:
        //1. it's name
        //2. it's type

        //3. sub-device names and types. also their pinLabels and wire that it is connected to. 
        //4. element names and their types

    }
    //what are some interesting things to show?
    //value at output pin
    //connectivity (structure)
    //
    int runShow(Command command){
        list<string>tokens = command.tokens();
        Device *device = command.device();
        if (tokens.size() == 0){
            //no tokens. show everything
            devicePrint(device);
            return 0;
        }
        
        string name = tokens.front();
        if (name == "CANVAS"){
            //printDevice(command.device());
            devicePrint(device);
            return 0;
        }
        else{
            Element *element = device->element(name);
            if (element != NULL){
                elementPrint(element);
            }


        }
        //try to locate this element


        return 0;
    }
}
