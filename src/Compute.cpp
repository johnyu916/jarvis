#include "Compute.h"
#include <iostream>

#include "Utilities.h"
namespace Jarvis{
    using namespace Devices;
    int resetVisited(Element *e);
    void activateResistors(list<Resistor *>resistors){
        list<Resistor *>::iterator rit;
        for (rit = resistors.begin(); rit != resistors.end(); rit++){
            Resistor *resistor = (*rit);
            resistor->isActive(true);
            debug("activated resistor: "<<resistor->fullName());
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
        if (source->visited()) return;
        source->visited(true);
        //debug("spanResistors source pin: "<< source->fullName());

        Wire *wire = source->wire();
        if (!wire) return;
        //debug("spanResistors wire: "<< wire->name());
        list<Pin *>pins = wire->pins();
        list<Pin *>::iterator it;
        for (it = pins.begin(); it != pins.end(); it++){
            Pin *pin = (*it);
            if (pin == source) continue;
            pin->visited(true);
            //get element for source
            Element *element = pin->element();
            if (element->type() == "source"){
                //cerr << "Encountered source "<< element->info() <<" while completing circuit."<<endl;
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
                if (!switc->isOn()) continue;

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
            if (input) {
                debug("turning on switch: "<<switc->fullName());
            }
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
        resetVisited(e);
        return 0;
    }

    int completeResistor(Element *e){
        if (e->type() == "source"){
            Source *source = (Source *)e;
            //debug("completeResistor source info: "<<source->info());
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
        if (source->visited()) return;
        source->visited(true);

        Wire *wire = source->wire();
        if (!wire) return;
        //cout <<"spanResetWires wire: "<< wire->info()<<endl;
        wire->voltage(false);
        list<Pin *>pins = wire->pins();
        list<Pin *>::iterator it;
        for (it = pins.begin(); it != pins.end(); it++){
            Pin *pin = (*it);
            if (pin == source) continue;
            pin->visited(true);
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
        if (source->visited()) return 0;
        source->visited(true);

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
            pin->visited(true);

            Element *element = pin->element();
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
        if (e->type() == "source"){
            Source *s = (Source *)e;
            s->pin()->visited(false);
        }
        else if (e->type() == "ground"){
            Ground *g = (Ground *)e;
            g->pin()->visited(false);
        }
        else if (e->type() == "resistor"){
            Resistor *r = (Resistor *)e;
            r->pin("p0")->visited(false);
            r->pin("p1")->visited(false);
        }
        else if (e->type() == "switch"){
            Switch *r = (Switch *)e;
            r->pin("p0")->visited(false);
            r->pin("p1")->visited(false);
            r->pin("in")->visited(false);
        }
        else if (e->type() == "bridge"){
            Bridge *b = (Bridge *)e;
            b->in()->visited(false);
            b->out()->visited(false);
        }
        else if (e->type() == "meter"){
            Meter *g = (Meter *)e;
            g->pin()->visited(false);
        }
        else{
            cerr << "Unknown element: "<< e->info()<<endl;
        }

        return 0;
    }
    int resetWires(Element *e){
        if (e->type() == "source"){
            Source *source = (Source *)e;
            Pin *pin= source->pin();
            spanResetWires(pin);
        }
        return 0;
    }

    int completeVoltage(Element *e){
        if (e->type() == "source"){
            Source *source = (Source *)e;
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
