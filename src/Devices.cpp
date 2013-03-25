#include "Devices.h"
namespace Jarvis{
    namespace Devices{
        Element::Element(string name, string type):name_(name),type_(type){
        }

        Transistor::Transistor(string name):Element(name,"TRANSISTOR"){
        }
        Power::Power(string name):Element(name,"POWER"){
        }
        Meter::Meter(string name):Element(name,"METER"){

        }
        Ground::Ground(string name):Element(name,"GROUND"){

        }

        Device *deviceWithName(Device *device, string name){
            if (device->name() == name) return device;
            list<Element *> elements = device->elements();
            list<Element *>::iterator eiter;
            for (eiter = elements.begin(); eiter != elements.end(); eiter++){
                if ((*eiter)->name() == name) return (*eiter);
            }

            //now call contains name on names
            list<Device *> devices = device->devices();
            list<Device *>::iterator diter;
            for (diter = devices.begin(); diter != devices.end(); diter++){
                Element *device = elementWithName(*diter, name);
                if (device != NULL) return device;
            }


            return NULL;
        }

        Wire::Wire(Pin *pin0, Pin *pin1):state_(false),reachable_(false){
            pins_.push_back(pin0);
            pins_.push_back(pin1);
        }
    }
}
