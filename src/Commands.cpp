#include "Commands.h"
#include "Compute.h"
#include "Parser.h"
#include "State.h"
#include <fstream>
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
    Pin *getPinWithCommand(Command cmd){
        list<string>command = cmd.tokens();
        string nameOne = command.front();
        command.pop_front();
        Device *canvas = cmd.device();
        Element *oneElement = canvas->element(nameOne);

        if (oneElement != NULL){
            string type = oneElement->type();
            if (type == "SWITCH"){
                Switch *swit = (Switch *)oneElement;
                //get second word
                string pinName = command.front();
                command.pop_front();
                return swit->pin(pinName);

            }
            else if (type == "POWER"){
                Power *power = (Power *)oneElement;
                string pinName = command.front();
                command.pop_front();
                return power->pin(pinName);
            }
            else{
                /*
                else if (type == "GROUND"){
                    Ground *ground = (Ground *)oneElement;
                    return ground->pin();

                }
                */
                if (type == "METER"){
                    Meter *meter = (Meter *)oneElement;
                    return meter->pin();

                }
                else{
                    //hmm this is not possible
                    return NULL;
                }
            }
        }
        else { //must be device

            Device *oneDevice = canvas->device(nameOne);
            Pin *pinOne;
            if (oneDevice == NULL) return NULL;
            
            //do we need to get the pin?
            string type = oneDevice->type();
            int size = oneDevice->pinLabels().size();
            if (size == 0) return NULL;
            else if (size == 1){
                return oneDevice->pinLabels().front()->pin();
            }
            else{
                string pinName = command.front();
                command.pop_front();
                return oneDevice->pinLabel(pinName)->pin();
            }
        }
    }

    int runLoad(Command cmd){
   // int runLoad(list<string> command){
       list<string> command = cmd.tokens();
        int size = command.size();
        if (size < 2) return 13;
        string type = command.front();
        command.pop_front();
        string name = command.front();
        command.pop_front();

        Device *canvas = cmd.device();

        //ensure name is not already taken
        bool isExist = isExistWithName(canvas, name);
        if (isExist) return 6;
        Element *element;

        if (size == 2){
            //this cannot be wire
            if (type == "WIRE"){
                return 5;
            }
            else if (type == "SWITCH"){
                element = new Switch(name);
            }
            else if (type == "power"){
                element = new Power(name);
            }
            /*
            else if (type == "ground"){
                element = new Ground(name);
            }
            */
            else if (type == "meter"){
                element = new Meter(name);
            }
            else{
                //this may be a device. look for it.
                type.append(".desc");
                int result = runScript(type, canvas);
                if (result == 0){
                    return 0;
                }
                else{
                //bleh
                    return 7;
                }
            }
            canvas->elements().push_back(element);
            return 0;
        }
   }

   int runLink(Command command){
        //needs at least 2 more
       list<string> tokens = command.tokens();
        int size = tokens.size();
        
        if (size <= 1 || size > 4) return 8;
        //get first pin
        Pin *pinOne = getPinWithCommand(command);
        if (pinOne == NULL) return 9;
        //get second pin
        Pin *pinTwo = getPinWithCommand(command);
        if (pinTwo == NULL) return 11;
        //it is okay of one or neither of them have wires. but it is not okay if both have wires
        return linkPins(pinOne, pinTwo);

            
    }
    int runUnlink(Command command){
        list<string> tokens = command.tokens();
        int size = tokens.size();
        if (size <= 1 || size > 2) return 18;
        Pin *pinOne = getPinWithCommand(command);

        return unlinkPin(pinOne);     
    }
    int runLabel(Command command){
        //must be at lest two
        Pin *pinOne = getPinWithCommand(command);
        if (pinOne == NULL) return 14;

        list<string>tokens = command.tokens();

        string name = tokens.front();
        tokens.pop_front();
        Device *device = command.device();
        PinLabel* label = new PinLabel(name,pinOne);
        device->pinLabels().push_back(label);
        return 0;
    }
    int runRun(Command command){
        //lets run
        list<string>tokens = command.tokens();
        if (tokens.size() == 0) return 16;
        string stepsString = tokens.front();
        int steps=0;
        istringstream(stepsString) >> steps;

        //turn on first
        while (steps > 0){
            compute(command.device());
            steps--;
        }
        
        
    }
    int runSave(Command command){
        return 0;
        //save to file
        list<string>tokens = command.tokens();
        if (tokens.size() == 0) return 16;
        string name = tokens.front();
        if (name == "") return 17;
        name.append("desc"); 
        //see if file exists. if exists, stop.
        ifstream input(name.c_str());
        if (input.is_open()){
            input.close();
            return 18;
        }


       // ofstream out(name);
        
    }
    int runShow(Command command){
        return 0;
    }
}
