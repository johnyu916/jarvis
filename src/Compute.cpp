#include "Compute.h"
#include <iostream>

#include "Utilities.h"
namespace Jarvis{
    using namespace Devices;
    void activateResistors(list<Resistor *>resistors){
        list<Resistor *>::iterator rit;
        for (rit = resistors.begin(); rit != resistors.end(); rit++){
            Resistor *resistor = (*rit);
            resistor->isActive(true);
            debug("activated resistor: "<<resistor->info());
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
            //get element for source
            Element *element = pin->element();
            if (element->visited()) continue;
            element->visited(true);
            if (element->type() == "source"){
                cerr << "Encountered source source while completing circuit."<<endl;
                return;
            }
            else if (element->type() == "ground"){
                //cout <<"ground reached: "<<endl;
                //end reached.
                activateResistors(resistors);
                //upWires(wires);
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

    int deactivateResistorAndResetVisited(Element *e){
    //int deactivateResistor(Element *e){
        if (e->type() == "resistor"){
            Resistor *r = (Resistor *)e;
            r->isActive(false);
        }
        e->visited(false);
        return 0;
    }

    int completeResistor(Element *e){
        if (e->type() == "source"){
            Source *source = (Source *)e;
            source->visited(true);
            Pin *pin= source->pin();
            list<Resistor *>resistors;
            spanResistors(pin,resistors);
        }
        return 0;
    }
    //tries to complete circuits, then activates all resistors when done
    // 1. reset resistors to inactive
    
    void updateResistors(Device *device){
        forEachElement(device, deactivateResistorAndResetVisited);
        forEachElement(device, completeResistor);
    }

    void spanResetWires(Pin* source){
        if (source == NULL) return;

        Wire *wire = source->wire();
        if (!wire) return;
        //cout <<"spanResetWires wire: "<< wire->info()<<endl;
        wire->voltage(false);
        list<Pin *>pins = wire->pins();
        list<Pin *>::iterator it;
        for (it = pins.begin(); it != pins.end(); it++){
            Pin *pin = (*it);
            if (pin == source) continue;
            Element *element = pin->element();
            if (element->visited()) continue;
            element->visited(true);
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
            return 0; //have separate audit if you want to check connectivity
            cerr << "source has no wire. source: " << source->name() <<endl;
            return 20;
        }
        /*
        if (wire->visited()){
            bool wireVoltage = wire->voltage();
            if (wireVoltage != voltage){
                cerr << "inconsistent voltage found while updating voltages on wire: " << wire->info() <<endl;
                return 21;
            }
            return 0;
        }*/
        //cout <<"spanVoltage setting wire: "<< wire->info()<< " to voltage: "<<voltage<<endl;
        wire->voltage(voltage);
        list<Pin *>pins = wire->pins();
        list<Pin *>::iterator it;
        for (it = pins.begin(); it != pins.end(); it++){
            Pin *pin = (*it);
            //cout <<"spanvoltage pin name: "<<pin->name()<<endl;
            if (pin == source) continue;

            Element *element = pin->element();
            if (element->visited()) continue;
            element->visited(true);
            
            if (element->type() == "source"){
                Source *source = (Source *)element;
                //warning? 

            }
            else if(element->type() == "ground"){ 
                bool gv = pin->wire()->voltage();
                if (gv){
                    cerr << "error: voltage at ground is high. " << pin->wire()->info()<<endl;
                    return 24;
                }
            }
            else if (element->type() == "switch"){
                Switch *switc = (Switch *)element;
                if (!switc->isOn()) continue;
                Pin *nextPin = switc->outPin(pin);
                int rVal = spanVoltage(nextPin, voltage);
                if (rVal != 0) return rVal;
            }
            else if (element->type() == "resistor"){
                Resistor *resistor = (Resistor *)element;
                Pin *nextPin = resistor->outPin(pin);
                bool nextVoltage = voltage;
                if (resistor->isActive()) nextVoltage = false;
                int rVal = spanVoltage(nextPin, nextVoltage);
                if (rVal != 0) return rVal;
                
            }
            else if (element->type() == "bridge"){
                Bridge *bridge = (Bridge *)element;
                Pin *nextPin = bridge->outPin(pin);
                int rVal = spanVoltage(nextPin, voltage);
                if (rVal != 0) return rVal;
            }

        }
        return 0;
    }
    int resetVisited(Element *e){
        e->visited(false);
        return 0;
    }
    int resetWires(Element *e){
        if (e->type() == "source"){
            Source *source = (Source *)e;
            source->visited(true);
            Pin *pin= source->pin();
            spanResetWires(pin);
        }
        return 0;
    }

    int completeVoltage(Element *e){
        if (e->type() == "source"){
            Source *source = (Source *)e;
            source->visited(true);
            Pin *pin = source->pin();
            return spanVoltage(pin,true);
        }
        return 0;
    }

    //TODO: first set all voltages to low
    int updateVoltages(Device *device){
        forEachElement(device,resetVisited);
        forEachElement(device,resetWires);
        forEachElement(device,resetVisited);

        return forEachElement(device,completeVoltage);
    }


        //switch on/off happens separately
    int compute(Device *device){
        updateResistors(device);

        debug("updated resistors");
        int rVal = updateVoltages(device);
        if (rVal != 0) return rVal;
        debug("updated voltages");

        toggleSwitches(device);
        return 0;
    }
}
