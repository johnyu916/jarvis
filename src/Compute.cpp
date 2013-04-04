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
    //void spanResistors(Pin *source, list<Wire *>wires){
    void spanResistors(Pin *source, list<Resistor *>resistors){
        //cout <<"entering spanResistors"<<endl;
        /* 1. try to reach the end. 
         * 2. if end is reached, must go back and turn all wires on in the path
         */
        if (source == NULL) return;
        //cout <<"spanResistors source pin: "<< source->name()<<endl;

        Wire *wire = source->wire();
        if (!wire) return;
        //cout <<"spanResistors wire: "<< wire->info()<<endl;
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
                spanResistors(nextPin,resistors);
            }
            else if (element->type() == "resistor"){
                Resistor *resistor = (Resistor *)element;
                resistors.push_back(resistor);
                Pin *nextPin = resistor->outPin(pin);
                spanResistors(nextPin, resistors);
            }
            else if (element->type() == "bridge"){
                Bridge *bridge = (Bridge *)element;
                Pin *nextPin = bridge->outPin(pin);
                spanResistors(nextPin, resistors);
            }
        }
    }
    int toggleSwitch(Element *e){
        if (e->type() == "switch"){
            Switch *switc = (Switch *)e;
            bool input = switc->pin("in")->wire()->voltage();
            if (input) switc->isOn(true);
            else switc->isOn(false);
            //cout <<"toggle switch input: "<<input<<" switch isOn: "<<switc->isOn()<<endl;
        }
        return 0;
    }

    void toggleSwitches(Device *device){
        forEachElement(device,toggleSwitch);
    }

    /*
    void toggleSwitchesTemp(Device *device){
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
    */

    int deactivateResistor(Element *e){
        if (e->type() == "resistor"){
            Resistor *r = (Resistor *)e;
            r->isActive(false);
        }
        return 0;
    }

    int completeResistor(Element *e){
        if (e->type() == "power"){
            Power *power = (Power *)e;
            Pin *source = power->pin("source");
            list<Resistor *>resistors;
            spanResistors(source,resistors);
        }
        return 0;
    }
    //tries to complete circuits, then activates all resistors when done
    // 1. reset resistors to inactive
    
    void updateResistors(Device *device){
        forEachElement(device, deactivateResistor);
        forEachElement(device, completeResistor);
    }

    /*
    void completeResistorsTemp(Device *device){
    //void resetDevice(Device *device){
        list<Element *> elements=  device->elements();
        list<Element *>::iterator it;
        for (it = elements.begin(); it != elements.end(); it++){
            Element *e = (*it);
            if (e->type() == "power"){
                Power *power = (Power *)e;
                Pin *source = power->pin("source");
                list<Resistor *>resistors;
                spanResistors(source,resistors);
            }
            else if (e->type() == "input"){
                Input *input = (Input *)e;
                Wire *wire = input->pin()->wire();
                wire->state(input->state());
            }
        }

        list<Device *>devices = device->devices();
        list<Device *>::iterator dit;
        for (dit = devices.begin(); dit != devices.end(); dit++){
            updateResistors(*dit);
        }
    }
    */

    void spanResetWires(Pin* source){
        if (source == NULL) return;

        Wire *wire = source->wire();
        if (!wire) return;
        //cout <<"spanResetWires wire: "<< wire->info()<<endl;
        wire->voltage(false);
        wire->visited(false);
        list<Pin *>pins = wire->pins();
        list<Pin *>::iterator it;
        for (it = pins.begin(); it != pins.end(); it++){
            Pin *pin = (*it);
            if (pin == source) continue;
            Element *element = pin->element();
            if (element->type() == "switch"){
                Switch *switc = (Switch *)element;
                Pin *nextPin = switc->outPin(pin);
                spanResetWires(nextPin);
            }
            else if (element->type() == "resistor"){
                Resistor *resistor = (Resistor *)element;
                Pin *nextPin = resistor->outPin(pin);
                spanResetWires(nextPin);
                
            }
            else if (element->type() == "bridge"){
                Bridge *bridge = (Bridge *)element;
                Pin *nextPin = bridge->outPin(pin);
                spanResetWires(nextPin);
            }
        }
    }

    int spanVoltage(Pin* source, bool voltage){
        if (source == NULL) return 0;

        Wire *wire = source->wire();
        if (!wire) {
            cerr << "source has no wire. source: " << source->name() <<endl;
            return 20;
        };
        if (wire->visited()){
            bool wireVoltage = wire->voltage();
            if (wireVoltage != voltage){
                cerr << "inconsistent voltage found while updating voltages on wire: " << wire->info() <<endl;
                return 21;
            }
            return 0;
        }
        //cout <<"spanVoltage wire: "<< wire->info()<<endl;
        wire->voltage(voltage);
        list<Pin *>pins = wire->pins();
        list<Pin *>::iterator it;
        for (it = pins.begin(); it != pins.end(); it++){
            Pin *pin = (*it);
            if (pin == source) continue;

            Element *element = pin->element();
            if (element->type() == "power"){
                Power *power = (Power *)element;
                if (pin == power->pin("ground")){
                    bool gv = pin->wire()->voltage();
                    if (gv){
                        cerr << "error: voltage at ground is high" <<endl;
                        return 24;
                    }
                }
            }
            if (element->type() == "switch"){
                Switch *switc = (Switch *)element;
                if (!switc->isOn()) return 0;
                Pin *nextPin = switc->outPin(pin);
                return spanVoltage(nextPin, voltage);
            }
            else if (element->type() == "resistor"){
                Resistor *resistor = (Resistor *)element;
                Pin *nextPin = resistor->outPin(pin);
                bool nextVoltage = voltage;
                if (resistor->isActive()) nextVoltage = false;
                return spanVoltage(nextPin, nextVoltage);
                
            }
            else if (element->type() == "bridge"){
                Bridge *bridge = (Bridge *)element;
                Pin *nextPin = bridge->outPin(pin);
                return spanVoltage(nextPin, voltage);
            }

        }
        return 0;
    }
    int resetWires(Element *e){
        if (e->type() == "power"){
            Power *power = (Power *)e;
            Pin *source = power->pin("source");
            spanResetWires(source);
        }
        return 0;
    }

    int completeVoltage(Element *e){
        if (e->type() == "power"){
            Power *power = (Power *)e;
            Pin *source = power->pin("source");
            return spanVoltage(source,true);
        }
        return 0;
    }

    //TODO: first set all voltages to low
    int updateVoltages(Device *device){
        int r = forEachElement(device,resetWires);
        if (r != 0) return r;
        return forEachElement(device,completeVoltage);
    }

    /*
    void updateVoltagesTemp(Device *device){
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
    */

        //switch on/off happens separately
    int compute(Device *device){
        updateResistors(device);

        cout <<"updated resistors"<<endl;
        int rVal = updateVoltages(device);
        if (rVal != 0) return rVal;
        cout <<"updated voltages"<<endl;

        toggleSwitches(device);
        return 0;
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
