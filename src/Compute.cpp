#include "Compute.h"
#include <iostream>

namespace Jarvis{
    using namespace Devices;
    void activateResistors(list<Resistor *>resistors){
        list<Resistor *>::iterator rit;
        for (rit = resistors.begin(); rit != resistors.end(); rit++){
            Resistor *resistor = (*rit);
            resistor->isActive(true);
        }
    }

    void upWires(list<Wire *>wires){
        list<Wire *>::iterator it;
        for (it = wires.begin(); it != wires.end(); it++){
            Wire *wire = (*it);
            wire->voltage(true);
            //cout <<"upped wire: "<<wire->info()<<endl;
        }
    }
    //void spanMesh(Pin *source, list<Wire *>wires){
    void spanMesh(Pin *source, list<Resistor *>resistors){
        //cout <<"entering spanMesh"<<endl;
        /* 1. try to reach the end. 
         * 2. if end is reached, must go back and turn all wires on in the path
         */
        if (source == NULL) return;
        //cout <<"spanMesh source pin: "<< source->name()<<endl;

        Wire *wire = source->wire();
        if (!wire) return;
        //cout <<"spanMesh wire: "<< wire->info()<<endl;
        //wires.push_back(wire);
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
                    activateResistors(resistors);
                    //upWires(wires);
                    return;
                }
            }
            else if (element->type() == "switch"){
                Switch *switc = (Switch *)element;
                if (!switc->isOn()) return;

                Pin *nextPin = switc->outPin(pin);
                spanMesh(nextPin,resistors);
            }
            else if (element->type() == "resistor"){
                Resistor *resistor = (Resistor *)element;
                resistors.push_back(resistor);
                Pin *nextPin = resistor->outPin(pin);
                spanMesh(nextPin, resistors);
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
                bool input = switc->pin("in")->wire()->voltage();
                if (input) switc->isOn(true);
                else switc->isOn(false);
                //cout <<"toggle switch input: "<<input<<" switch isOn: "<<switc->isOn()<<endl;
            }
        }

        list<Device *>devices = device->devices();
        list<Device *>::iterator dit;
        for (dit = devices.begin(); dit != devices.end(); dit++){
            toggleSwitches(*dit);
        }
    }

    void completeCircuits(Device *device){
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
                list<Resistor *>resistors;
                spanMesh(source,resistors);
            }
            /*
            else if (e->type() == "input"){
                Input *input = (Input *)e;
                Wire *wire = input->pin()->wire();
                wire->state(input->state());
            }
            */
        }

        list<Device *>devices = device->devices();
        list<Device *>::iterator dit;
        for (dit = devices.begin(); dit != devices.end(); dit++){
            completeCircuits(*dit);
        }
    }

    void spanVoltage(Pin* source, bool voltage){
        if (source == NULL) return;

        Wire *wire = source->wire();
        if (!wire) return;
        //cout <<"spanVoltage wire: "<< wire->info()<<endl;
        wire->voltage(voltage);
        list<Pin *>pins = wire->pins();
        list<Pin *>::iterator it;
        for (it = pins.begin(); it != pins.end(); it++){
            Pin *pin = (*it);
            if (pin == source) continue;
            Element *element = pin->element();
            if (element->type() == "switch"){
                Switch *switc = (Switch *)element;
                bool nextVoltage;
                if (switc->isOn()) nextVoltage = voltage;
                else nextVoltage = false;
                Pin *nextPin = switc->outPin(pin);
                spanVoltage(nextPin, nextVoltage);
            }
            else if (element->type() == "resistor"){
                Resistor *resistor = (Resistor *)element;
                Pin *nextPin = resistor->outPin(pin);
                bool nextVoltage = resistor->outVoltage(voltage);
                spanVoltage(nextPin, nextVoltage);
                
            }
        }
    }

    void updateVoltages(Device *device){
        list<Element *> elements=  device->elements();
        list<Element *>::iterator it;
        for (it = elements.begin(); it != elements.end(); it++){
            Element *e = (*it);
            if (e->type() == "power"){
                Power *power = (Power *)e;
                Pin *source = power->pin("source");
                spanVoltage(source,true);
            }
        }

        list<Device *>devices = device->devices();
        list<Device *>::iterator dit;
        for (dit = devices.begin(); dit != devices.end(); dit++){
            updateVoltages(*dit);
        }
    
    }

        //switch on/off happens separately
    void compute(Device *device){
        completeCircuits(device);
        updateVoltages(device);
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
