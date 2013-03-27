#include "Devices.h"
#include "State.h"
namespace Jarvis{
    namespace Devices{
        Element::Element(string name, string type):name_(name),type_(type){
        }

        Switch::Switch(string name):Element(name,"SWITCH"){
        }
        Switch::~Switch(){
            delete p0_;
            delete p1_;
            delete in_;
        }
        Pin *Switch::outPin(Pin *inPin){
            //first off, is switch on?
            if (! in_->wire()->state()) return NULL;

            if (inPin == p0_) return p1_;
            else if (inPin == p1_) return p0_;
            else return NULL;
        }
        Power::Power(string name):Element(name,"POWER"){
            source_ = new Pin(this);
            ground_ = new Pin(this);

        }
        Pin *Power::pin(string type){
            if (type == "SOURCE") return source_;
            else if (type == "GROUND") return ground_;
            else return NULL;
        }
        Power::~Power(){
            delete source_, ground_;
        }
        Meter::Meter(string name):Element(name,"METER"){

        }
        Meter::~Meter(){
            delete pin_;
        }
        /*
        Ground::Ground(string name):Element(name,"GROUND"){

        }
        Ground::~Ground(){
            delete pin_;
        }
        */
        Device::Device():name_("UNNAMED"),type_("UNTYPED"){

        }
        Device::Device(string name, string type):name_(name),type_(type){

        }
        Device::~Device(){
            deviceClear(this);
        }

        void deviceClear(Device *device){
            //unload everything
            list<Device *>devices = device->devices();
            list<Device *>::iterator dit;
            for (dit = devices.begin(); dit != devices.end(); dit++){
                Device *device = (*dit);
                delete device;
            }

            list<Element *>elements = device->elements();
            list<Element *>::iterator eit;
            for (eit = elements.begin(); eit != elements.end(); eit++){
                Element *element = (*eit);
                delete element;
            }

            list<PinLabel *>pinLabels = device->pinLabels();
            list<PinLabel *>::iterator pit;
            for (pit = pinLabels.begin(); pit != pinLabels.end(); pit++){
                PinLabel *label = (*pit);
                delete label;

            }
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
        bool isExistWithName(Device *canvas, string name){
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
        }
        /*
        void Wire::linkPin(Pin *pin){
        }
        void Wire::unlinkPin(Pin *pin){
        }
        */

        Pin* Switch::pin(string name){
            if (name == "P0"){
                return p0_;
            }
            else if (name == "P1"){
                return p1_;
            }
            else if (name == "IN"){
                return in_;
            }
            else{
                return NULL;
            }
        }
        /*
        void Pin::wire(Wire *wire){
            //previous wire should remove it
            if (wire_){
                wire_->deletePin(this);
            }
            wire_=wire;
        }*/
        Pin::Pin(Element *element){
            element_ = element;
            wire_ = NULL;
        }
        Pin::~Pin(){
            //wire_->deletePin(this);            
        }
        PinLabel::PinLabel(string name, Pin *pin):name_(name),pin_(pin){
        }
        PinLabel::~PinLabel(){
            pin_ = NULL;
        }

        //pre: pins must exist
        //link pin0 and pin1.
        //if pin0 and pin1 are both attached to some wire, then return (you should disconnect at least one pin from its wire first with call to unlink)
        int linkPins(Pin *pin0, Pin* pin1){
            Wire *wire0 = pin0->wire();
            Wire *wire1 = pin1->wire();

            if (wire1 && wire0) return 14;
            else if (wire0 && !wire1){
                pin1->wire(wire0);
                wire0->pins().push_back(pin1);
            }
            else if (!wire0 && wire1){
                pin0->wire(wire1);
                wire1->pins().push_back(pin0);
            }
            else {
                Wire* wire = new Wire(pin0,pin1);
                pin0->wire(wire);
                pin1->wire(wire);

            }
            return 0;
        }

        int unlinkPin(Pin *pin){
            Wire* wire = pin->wire();
            if (wire == NULL) return 1;
            pin->wire(NULL);
            wire->pins().remove(pin);
            //if there are no pins left, destroy self.
            if (wire->pins().size() == 0) delete wire;
            return 0;
        }
    }
}
