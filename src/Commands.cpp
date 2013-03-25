#include "Commands.h"
#include "Devices.h"
#include "State.h"

namespace Jarvis{
    using namespace Devices;
    /*
    LoadCommand::LoadCommand(list<string> commands){
        type_="";
        name_="";
        
        //must be at least two
    }*/

    //get pin from current canvas
    Pin *getPinWithCommand(list<string>command){
        string nameOne = command.front();
        command.pop_front();
        Device *canvas = State::instance().device();
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
            else{
                if (type == "POWER"){
                    Power *power = (Power *)oneElement;
                    return power->pin();
                }
                else if (type == "GROUND"){
                    Ground *ground = (Ground *)oneElement;
                    return ground->pin();

                }
                else if (type == "METER"){
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

    int runLoad(list<string> command){
        int size = command.size();
        if (size < 2) return 13;
        string type = command.front();
        command.pop_front();
        string name = command.front();
        command.pop_front();

        Device *canvas = State::instance().device();

        //ensure name is not already taken
        bool isExist = isExistWithName(name);
        if (isExist) return 6;
        Element *element;

        if (size == 2){
            //this cannot be wire
            if (type == "wire"){
                return 5;
            }
            else if (type == "transistor"){
                element = new Switch(name);
            }
            else if (type == "power"){
                element = new Power(name);
            }
            else if (type == "ground"){
                element = new Ground(name);
            }
            else if (type == "meter"){
                element = new Meter(name);
            }
            else if (type == "DEVICE"){
                
            }
            else{
                //bleh
                return 7;
            }
            canvas->elements().push_back(element);

        }
        else{
            if (type != "wire"){
                return 10;
            }
            //needs at least 2 more
            if (size == 3) return 8;
            //get first pin
            Pin *pinOne = getPinWithCommand(command);
            if (pinOne == NULL) return 9;
            //get second pin
            Pin *pinTwo = getPinWithCommand(command);
            if (pinTwo == NULL) return 11;
            //everything is ready
            Wire* newWire= new Wire(pinOne, pinTwo);
            
        }
    }
}
