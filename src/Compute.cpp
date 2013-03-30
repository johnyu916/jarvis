#include "Compute.h"
#include <iostream>

namespace Jarvis{
    using namespace Devices;
    void upWires(list<Wire *>wires){
        list<Wire *>::iterator it;
        for (it = wires.begin(); it != wires.end(); it++){
            Wire *wire = (*it);
            wire->state(true);
            //cout <<"upped wire: "<<wire->info()<<endl;
        }
    }
    void spanMesh(Pin *source, list<Wire *>wires){
        //cout <<"entering spanMesh"<<endl;
        /* 1. try to reach the end. 
         * 2. if end is reached, must go back and turn all wires on in the path
         */
        if (source == NULL) return;

        Wire *wire = source->wire();
        //cout <<"spanMesh wire: "<< wire->info()<<endl;
        wires.push_back(wire);
        list<Pin *>pins = wire->pins();
        list<Pin *>::iterator it;
        for (it = pins.begin(); it != pins.end(); it++){
            Pin *pin = (*it);
            if (pin == source) continue;
            //get element for power
            Element *element = pin->element();
            if (element->type() == "power"){
                Power *power = (Power *)element;
                if (pin == power->pin("ground")){
        //cout <<"ground reached: "<<endl;
                    //end reached.
                    upWires(wires);
                    return;
                }
            }
            else if (element->type() == "switch"){
                Switch *switc = (Switch *)element;
                Pin *nextPin = switc->outPin(pin);
                spanMesh(nextPin,wires);
            }
        }
    }
    void toggleSwitches(Device *device){
        list<Element *> elements=  device->elements();
        list<Element *>::iterator it;
        for (it = elements.begin(); it != elements.end(); it++){
            Element *e = (*it);
            if (e->type() == "switch"){
                Switch *switc = (Switch *)e;
                bool input = switc->pin("in")->wire()->state();
                if (input) switc->state(true);
                else switc->state(false);
            }
        }

        
        list<Device *>devices = device->devices();
        list<Device *>::iterator dit;
        for (dit = devices.begin(); dit != devices.end(); dit++){
            toggleSwitches(*dit);
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
        list<Element *>::iterator it;
        for (it = elements.begin(); it != elements.end(); it++){
            Element *e = (*it);
            if (e->type() == "power"){
                Power *power = (Power *)e;
                Pin *source = power->pin("source");
                list<Wire *>wires;
                spanMesh(source,wires);
            }
            else if (e->type() == "INPUT"){
                Input *input = (Input *)e;
                Wire *wire = input->pin()->wire();
                wire->state(input->state());
            }
        }

        list<Device *>devices = device->devices();
        list<Device *>::iterator dit;
        for (dit = devices.begin(); dit != devices.end(); dit++){
            compute(*dit);
        }

        //switch on/off happens separately
        toggleSwitches(device);
    }
    /*
    void compute(Device *device){
        //examine all switches. alter state. then call power device

        list<Element *> elements=  device->elements();
        list<Elementi *>::iterator it;
        for (it = elements.begin(); it != elements.end(); it++){
            Element *element = (*it);
            if (element->type() == "switch"){
                Switch *swi = (Switch *)element;
                
            }
        }

        list<Device *>devices = device->devices();
        list<Device *>::iterator it;
        for (it = devices.begin(); it != devices.end(); it++){
            
        }
        
    }*/
}
