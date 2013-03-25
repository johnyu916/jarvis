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
        Element *one = elementWithName(canvas, nameOne);
        Pin *pinOne;
        if (one == NULL) return NULL;
        
        //do we need to get the pin?
        string type = one->type();
        if (type == "TRANSISTOR" || type == "DEVICE"){
            string pinName = command.front();
            command.pop_front();
            return elementWithName(one, pinName);
        }
        else if (type == "PIN"){
            return one;
        }
        else{
            return one->pin();
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
                element = new Transistor(name);
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
            element = new Wire(pinOne, pinTwo);
        }
    }
}
