#include <iostream>
#include <sstream>
#include "Devices.h"
#include "State.h"
namespace Jarvis{
    namespace Devices{
        Element::Element(string name, string type):name_(name),type_(type){
        }

        string Element::info(){
            return name_ + " " + type_;
        }
        Bridge::Bridge(string name, Pin *inPin):Element(name,"bridge"){
            string pinName = name + "/in";
            in_ = new Pin(this, pinName);
            pinName = name + "/out";
            out_ = new Pin(this, pinName);

            linkPins(inPin, in_);
        }

        Bridge::~Bridge(){
            delete in_;
            delete out_;
        }
        Pin *Bridge::outPin(Pin *inPin){
            if (inPin == in_) return out_;
            else if (inPin == out_) return in_;
            else return NULL;
        }

        Switch::Switch(string name):Element(name,"switch"){
            string pinName = name;
            pinName.append("/P0");
            p0_ = new Pin(this,pinName);
            pinName = name;
            pinName.append("/P1");
            p1_ = new Pin(this, pinName);
            pinName = name;
            pinName.append("/IN");
            in_ = new Pin(this, pinName);
        }
        Switch::~Switch(){
            delete p0_;
            delete p1_;
            delete in_;
        }
        Pin *Switch::outPin(Pin *inPin){
            //if (!isOn_) return NULL;

            if (inPin == p0_) return p1_;
            else if (inPin == p1_) return p0_;
            else return NULL;
        }
        Power::Power(string name):Element(name,"power"){
            string pinName = name;
            pinName.append("/Source");
            source_ = new Pin(this,pinName);
            pinName = name;
            pinName.append("/Ground");
            ground_ = new Pin(this,pinName);

        }
        string Power::info(){
            ostringstream sout;
            sout <<"type: power. source: ";
            //wire = power->pin("source")->wire();
            Wire *wire = source_->wire();
            if (wire) sout <<" wire: "<<wire->info()<<endl;
            sout <<" ground: ";
            wire = ground_->wire();
            if (wire) sout <<" wire: "<<wire->info();
            return sout.str();

        }
        Pin *Power::pin(string type){
            if (type == "source") return source_;
            else if (type == "ground") return ground_;
            else return NULL;
        }
        Power::~Power(){
            delete source_, ground_;
        }
        Resistor::Resistor(string name):Element(name,"resistor"){
            string pinName = name + "/p0";
            p0_ = new Pin(this, pinName);
            pinName = name + "/p1";
            p1_ = new Pin(this, pinName);
            isActive_ = false;
        }
        Pin *Resistor::pin(string name){
            if (name == "p0") return p0_;
            else if (name == "p1") return p1_;
            else return NULL;
        }
        /*
        bool Resistor::outVoltage(bool inV){
            if (!inV) return false;
            if (isActive_ && inV) return false;
            else return true;
        }*/
        Pin *Resistor::outPin(Pin *inPin){
            if (inPin == p0_) return p1_;
            else if (inPin == p1_) return p0_;
            else return NULL;
        }

        string Resistor::info(){
            ostringstream sout;
            sout << "type: resistor. p0 wire: " << p0_->wire()->info()<<endl;
            sout << "p1 wire: " << p1_->wire()->info()<<endl;
            return sout.str();
        }

        /*
        Input::Input(string name):Element(name,"input"){
            //state_ = false;
            power_ = new Power(name+"/Power");
        }
        Input::~Input(){
            delete power_;
        }
        void Input::state(bool input){
            //if any wire attached, set it to input
            //state_=input;
            //unlink both wires

            if (input){
            }
        }
        */
        Meter::Meter(string name):Element(name,"meter"){
            pin_ = new Pin(this,name);
        }
        Meter::~Meter(){
            delete pin_;
        }
        string Meter::info(){
            ostringstream sout;
            sout <<"Meter. "<<name_<<" state: ";
            sout <<pin_->wire()->voltage();
            return sout.str();
        }
        /*
        Ground::Ground(string name):Element(name,"ground"){

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

            list<Bridge *>pinLabels = device->bridges();
            list<Bridge *>::iterator pit;
            for (pit = pinLabels.begin(); pit != pinLabels.end(); pit++){
                Bridge *label = (*pit);
                delete label;

            }
        }
        Bridge* Device::bridge(string name){
            list<Bridge*>::iterator iter;
            for (iter = bridges_.begin(); iter != bridges_.end(); iter++){
                Bridge *label = (*iter);
                if (label->name() == name){
                    return label;
                }
                
            }
            return NULL;
        }
        /*
        Bridge* Device::pinLabel(string name){
            list<Bridge*>::iterator iter;
            for (iter = pinLabels_.begin(); iter != pinLabels_.end(); iter++){
                Bridge *label = (*iter);
                if (label->name() == name){
                    return label;
                }
                
            }
            return NULL;
        }*/

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
            
            Bridge *label = canvas->bridge(name);
            //Bridge *label = canvas->pinLabel(name);
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
            cout <<"Device: "<<device->name()<<" "<< device->type() <<endl;
            list<Bridge *> labels= device->bridges();
            list<Bridge *>::iterator liter;
            for (liter = labels.begin(); liter != labels.end(); liter++){
                Bridge *label = (*liter);
                cout <<"Bridge: "<< label->name();
                cout <<" ";
                cout <<"In Wire: "<< label->in()->wire()->name() <<endl;
                Wire *outWire = label->out()->wire();
                if (outWire){
                    cout <<"Out Wire: "<<outWire->name() <<endl;
                }
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
            cout <<"element: "<<element->info() <<endl;
            string type = element->type();
            Wire *wire;
            if (type == "power"){
                
            }
            else if (type == "switch"){

            }
            else if (type == "meter"){
            }
            else if (type == "resistor"){
             //   Resistor *resistor = (Resistor *)element;
              //  cout <<"Resistor. ";
               // cout <<resistor->info()<<endl;
            }
            
            /*
            else if (type == "input"){
                Input *input = (Input *)element;
                cout <<"state: ";
                cout << input->state() <<endl;
            }
            */
        }

        Wire::Wire(Pin *pin0, Pin *pin1):voltage_(false)/*,reachable_(false)*/{
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
                name.append("--");
            }
            return name;
        }
        string Wire::info(){
            ostringstream stream;
            stream << name() <<endl<<"voltage: " << voltage_;
            return stream.str();
            
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
        string Switch::info(){
            ostringstream sout;
            sout <<"Switch. P0: ";
            //sout <<switc->pin("p0")->wire()->name();
            sout <<p0_->wire()->name();
            sout <<" P1: ";
            sout <<p1_->wire()->name();
            sout <<" IN: ";
            sout <<in_->wire()->name();
            return sout.str();

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

        //pre: pins must exist
        //link pin0 and pin1.
        //if pin0 and pin1 are both attached to some wire, then return (you should disconnect at least one pin from its wire first with call to unlink)
        int linkPins(Pin *pin0, Pin* pin1){
            Wire *wire0 = pin0->wire();
            Wire *wire1 = pin1->wire();
            //cout <<"0"<<endl;

            if (wire1 && wire0) {
                cerr <<"Both pins already connected to some wire. wire1: "<<wire0->info() << " wire2: "<<wire1->info()<<endl;
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

        //run some function over every element in our device tree
        int forEachElement(Device *device, int (*operation)(Element *element)){
            list<Element *> elements=  device->elements();
            list<Element *>::iterator it;
            for (it = elements.begin(); it != elements.end(); it++){
                Element *e = (*it);
                int rVal = operation(e);
                if (rVal != 0) return rVal;
            }

            list<Device *>devices = device->devices();
            list<Device *>::iterator dit;
            for (dit = devices.begin(); dit != devices.end(); dit++){
                Device *d = (*dit);
                int rVal = forEachElement(d, operation);
                if (rVal != 0) return rVal;
            }
            return 0;
        }
    }
}
