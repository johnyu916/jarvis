#include <iostream>

#include "Devices.h"
#include "State.h"
namespace Jarvis{
    namespace Devices{
        Element::Element(string name, string type):name_(name),type_(type){
        }

        Switch::Switch(string name):Element(name,"SWITCH"){
            string pinName = name;
            pinName.append("P0");
            p0_ = new Pin(this,pinName);
            pinName = name;
            pinName.append("P1");
            p1_ = new Pin(this, pinName);
            pinName = name;
            pinName.append("IN");
            in_ = new Pin(this, pinName);
        }
        Switch::~Switch(){
            delete p0_;
            delete p1_;
            delete in_;
        }
        Pin *Switch::outPin(Pin *inPin){
            //first off, is switch on?
            if (!state_) return NULL;

            if (inPin == p0_) return p1_;
            else if (inPin == p1_) return p0_;
            else return NULL;
        }
        Power::Power(string name):Element(name,"POWER"){
            string pinName = name;
            pinName.append("Source");
            source_ = new Pin(this,pinName);
            pinName = name;
            pinName.append("Ground");
            ground_ = new Pin(this,pinName);

        }
        Pin *Power::pin(string type){
            if (type == "source") return source_;
            else if (type == "ground") return ground_;
            else return NULL;
        }
        Power::~Power(){
            delete source_, ground_;
        }
        Input::Input(string name):Element(name,"INPUT"){
            state_ = false;
            pin_ = new Pin(this, "pin");
        }
        Input::~Input(){
            delete pin_;
        }
        void Input::state(bool input){
            //if any wire attached, set it to input
            state_=input;
        }
        

        Meter::Meter(string name):Element(name,"METER"){
            pin_ = new Pin(this,name);
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
                if (device->name() == name) return device;
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

        void printNameTypeLabels(Device *device){
            cout <<device->name()<<" "<< device->type() <<endl;
            list<PinLabel *> labels= device->pinLabels();
            list<PinLabel *>::iterator liter;
            for (liter = labels.begin(); liter != labels.end(); liter++){
                PinLabel *label = (*liter);
                cout << label->name();
                cout <<" ";
                cout << label->pin()->wire()->name() <<endl;
            }

        }

        void devicePrint(Device *device){
            printNameTypeLabels(device);

            list<Device *> devices = device->devices();
            list<Device *>::iterator diter;
            for (diter = devices.begin(); diter != devices.end(); diter++){
                Device *child = (*diter);
                printNameTypeLabels(child);
            }

            list<Element *>elements = device->elements();
            list<Element *>::iterator eit;
            for (eit = elements.begin(); eit != elements.end(); eit++){
                Element *element = (*eit);
                elementPrint(element);
            }
        }


        void elementPrint(Element *element){
            cout <<"element: "<<element->name()<<" "<<element->type()<<endl;
            string type = element->type();
            Wire *wire;
            if (type == "POWER"){
                Power *power = (Power *)element;
                cout <<"source: ";
                wire = power->pin("SOURCE")->wire();
                cout <<wire->name() << " "<<wire->state();
                cout <<" ground: ";
                wire = power->pin("GROUND")->wire();
                cout <<wire->name() << " "<<wire->state();
                cout <<endl;
            }
            else if (type == "SWITCH"){
                Switch *switc = (Switch *)element;
                cout <<"P0: ";
                cout <<switc->pin("P0")->wire()->name();
                cout <<" P1: ";
                cout <<switc->pin("P1")->wire()->name();
                cout <<" IN: ";
                cout <<switc->pin("IN")->wire()->name();
                cout <<endl;

            }
            else if (type == "METER"){
                Meter *meter = (Meter *)element;
                cout <<"state: ";
                cout <<meter->pin()->wire()->state();
                cout <<endl;
            }
            else if (type == "INPUT"){
                //nothing
                Input *input = (Input *)element;
                cout <<"state: ";
                cout << input->state() <<endl;
            }
        }

        Wire::Wire(Pin *pin0, Pin *pin1):state_(false),reachable_(false){
            pins_.push_back(pin0);
            pins_.push_back(pin1);
        }
        string Wire::name(){
            //stringtogether the pin names
            list<Pin *>::iterator pit;
            string name ="";
            for (pit = pins_.begin(); pit != pins_.end(); pit++){
                Pin *pin = (*pit);
                name.append(pin->name());
            }
            return name;
        }
        /*
        void Wire::linkPin(Pin *pin){
        }
        void Wire::unlinkPin(Pin *pin){
        }
        */

        Pin* Switch::pin(string name){
            if (name == "p0"){
                return p0_;
            }
            else if (name == "p1"){
                return p1_;
            }
            else if (name == "in"){
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
        Pin::Pin(Element *element, string name){
            element_ = element;
            wire_ = NULL;
            name_=name;
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
            //cout <<"0"<<endl;

            if (wire1 && wire0) {
                cerr <<"Both pins already connected to some wire"<<endl;
                return 14;
            }
            else if (wire0 && !wire1){
                pin1->wire(wire0);
                wire0->pins().push_back(pin1);
            }
            else if (!wire0 && wire1){
                pin0->wire(wire1);
                wire1->pins().push_back(pin0);
            }
            else {
                //cout <<"1"<<endl;
                Wire* wire = new Wire(pin0,pin1);
                pin0->wire(wire);
                pin1->wire(wire);

            }
            //cout <<"linkpins done"<<endl;
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
