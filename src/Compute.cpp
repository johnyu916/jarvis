#include "Compute.h"
#include "Devices.h"

namespace Jarvis{
    void upWires(list<Wire *>wires){
        list<Wire *>pins = wire->pins();
        list<Wire *>::iterator it;
        for (it = pins.begin(); it != pins.end(); it++){
            Wire *wire = (*it);
            wire->state(true);
        }
    }
    void spanMesh(Pin *source, list<Wire *>wires){
        /* 1. try to reach the end. 
         * 2. if end is reached, must go back and turn all wires on in the path
         */
        if (source == NULL) return;

        Wire *wire = source->wire();
        wires.push_back(wire);
        list<Pin *>pins = wire->pins();
        list<Pin *>::iterator it;
        for (it = pins.begin(); it != pins.end(); it++){
            Pin *pin = (*it);
            if (pin == source) continue;
            //get element for power
            Element *element = pin->element();
            if (element->type() == "POWER"){
                Power *power = (Power *)element;
                if (pin == power->ground()){
                    //end reached.
                    upWires(wires);
                    return;
                }
            }
            else if (element->type() == "SWITCH"){
                Switch *switc = (Switch *)element;
                Pin *nextPin = switc->outPin(pin);
                spanMesh(nextPin);
            }
        }
    }

    void compute(Device *device){
    //void resetDevice(Device *device){
        /*1. set all wires to 0
         *2. locate all power sources
         *3. try to complete circuits
         *
         */
        list<Element *> elements=  device->elements();
        list<Elementi *>::iterator it;
        for (it = elements.begin(); it != elements.end(); it++){
            Element *e = (*it);
            if (e->type() == "POWER"){
                Power *power = (Power *)e;
                Pin *source = power->source();
                spanMesh(source);
            }
        }

        list<Device *>devices = device->devices();
        list<Device *>::iterator it;
        for (it = devices.begin(); it != devices.end(); it++){
            resetDevice(*it);
        }
    }
    /*
    void compute(Device *device){
        //examine all switches. alter state. then call power device

        list<Element *> elements=  device->elements();
        list<Elementi *>::iterator it;
        for (it = elements.begin(); it != elements.end(); it++){
            Element *element = (*it);
            if (element->type() == "SWITCH"){
                Switch *swi = (Switch *)element;
                
            }
        }

        list<Device *>devices = device->devices();
        list<Device *>::iterator it;
        for (it = devices.begin(); it != devices.end(); it++){
            
        }
        
    }*/
}
