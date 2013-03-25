#include "Devices.h"
#include "State.h"
namespace Jarvis{
    namespace Devices{
        Element::Element(string name, string type):name_(name),type_(type){
        }

        Switch::Switch(string name):Element(name,"SWITCH"){
        }
        Power::Power(string name):Element(name,"POWER"){
        }
        Meter::Meter(string name):Element(name,"METER"){

        }
        Ground::Ground(string name):Element(name,"GROUND"){

        }
        Device::Device():name_("UNNAMED"),type_("UNTYPED"){

        }
        Device::Device(string name, string type):name_(name),type_(type){

        }
        PinLabel* Device::pinLabel(string name){
            list<PinLabel*>::iterator iter;
            for (iter = pinLabels_.begin(); iter != pinLabels_.end(); iter++){
                PinLabel *label = (*iter);
                if (label->name() == name){
                    return label;
                }
                
            }
            return NULL;
        }

        Element* Device::element(string name){
            list<Element *>::iterator eiter;
            for (eiter = elements_.begin(); eiter != elements_.end(); eiter++){
                if ((*eiter)->name() == name) return (*eiter);
            }
            return NULL;
        }

        Device* Device::device(string name){
            list<Device *>::iterator diter;
            for (diter = devices_.begin(); diter != devices_.end(); diter++){
                Device* device = (*diter);
                if (device != NULL) return device;
            }
            return NULL;
        }
        bool isExistWithName(string name){
            Device *canvas = State::instance().device();
            Device *device = canvas->device(name);
            if (device) return true;
            Element *element = canvas->element(name);
            if (element) return true;
            PinLabel *label = canvas->pinLabel(name);
            if (label) return true;
            return false;
        }
        //this is recursive search
        Device *deviceWithName(Device *device, string name){
            //now call contains name on names
            list<Device *> devices = device->devices();
            list<Device *>::iterator diter;
            for (diter = devices.begin(); diter != devices.end(); diter++){
                Device* device = deviceWithName(*diter, name);
                if (device != NULL) return device;
            }

            return NULL;
        }

        Wire::Wire(Pin *pin0, Pin *pin1):state_(false),reachable_(false){

            pins_.push_back(pin0);
            pins_.push_back(pin1);
            pin0->wire(this);
            pin1->wire(this);
        }
        void Wire::deletePin(Pin *pin){
            pins_.remove(pin);
        }

        Pin* Switch::pin(string name){
            if (name == "P0"){
                return _p0;
            }
            else if (name == "P1"){
                return _p1;
            }
            else if (name == "IN"){
                return _in;
            }
            else{
                return NULL;
            }
        }
        void Pin::wire(Wire *wire){
            //previous wire should remove it
            if (wire_){
                wire_->deletePin(this);
            }
            wire_=wire;
        }
    }
}
