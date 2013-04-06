#ifndef JARVIS_DEVICES_H
#define JARVIS_DEVICES_H

#include <list>
#include <string>

using namespace std;
namespace Jarvis{
    namespace Devices{
        //primitive device types
        enum DEVICE_TYPE{
            POWER = 0,
            RESISTOR,
            BRIDGE,
            SWITCH,
            METER,
        };
        
        class Element{
            public:
                Element(string name, string type);
                string name(){return name_;}
                string type(){return type_;}
                virtual string info();
            protected:
                string name_;
                string type_;

        };

        /*
        class Pin;

        class PinLabel{
            public:
                PinLabel(string name, Pin *pin);
                ~PinLabel();
                Pin* pin(){ return pin_;}
                string name(){ return name_;}
            private:
                string name_;
                Pin *pin_;
        };
        */

        class Bridge;
        class Pin;
        //a device can have devices and basic components inside
        class Device {
            public:
                Device();
                ~Device();
                Device(string name, string type);
                list<Device *>& devices(){return devices_;}
                Device* device(string name);
                list<Element *>& elements(){return elements_;}
                Element* element(string name);
                list<Bridge *>& bridges(){
                    return bridges_;}
                Bridge *bridge(string name);
                //list<PinLabel *>& pinLabels(){return pinLabels_;}
                //PinLabel* pinLabel(string name);
                string name(){return name_;}
                string type(){return type_;}
            private:
                string name_;
                string type_;
                list<Device *>devices_;
                list<Element *>elements_;
                list<Bridge *>bridges_;
                //list<PinLabel *>pinLabels_;
        };

        //wire always has at least two devices connected to it. some conditions:
        //1. only one of the devices can be power.
        //2. only one of the devices can be ground.
        //3. only one of the devices can be input.
        //4. only one of the devices can be output.
        //TODO: delete
        class Wire {
        public:
            Wire(Pin *pin0, Pin *pin1);
            list<Pin *>& pins(){return pins_;}
            string info();
            //new pin
            //void linkPin(Pin *pin);
            //void unlinkPin(Pin *pin);
            string name();
            
            bool voltage(){ return voltage_; }
            void voltage(bool state){voltage_=state;}
            bool visited(){ return visited_;}
            void visited(bool visited){ visited_=visited;}
        private:
            bool voltage_; //high or low
            bool visited_; //has been visited
            //bool reachable_; //is this reachable from plus terminal?
            list<Pin *> pins_;
        };

        class Pin{
            public:
                Pin(Element *element, string name);
                ~Pin();
                string name(){ return name_;}
                void wire(Wire *wire){ wire_=wire;}
                Wire *wire(){return wire_;}
                Element *element(){
                    return element_;
                }
            private:
                Wire *wire_;
                Element *element_;
                string name_;
        };

        //an active resistor has current flowing through it
        class Resistor : public Element{
            public:
                Resistor(string name);
                Pin *pin(string name);
                Pin *outPin(Pin *inPin);
                //bool outVoltage(bool inV);
                bool isActive(){ return isActive_;}
                void isActive(bool a){ isActive_ = a;}
                string info();
            private:
                Pin *p0_, *p1_;
                bool isActive_;
        };
        //basically a convenience method.
        //contains power and connects to source or ground depending on 
        /*
        class Input{
        public:
            Input(string name);
            ~Input();
            Pin *pin(){return pin_;}
            void state(bool input);
            bool state();
            //bool state(){return state_;}
        private:
            Power *power_;
            //bool state_;
            
        };
        */
        class Meter : public Element{
        public:
            Meter(string name);
            ~Meter();
            Pin *pin(){return pin_;}
            string info();
        private:
            Pin *pin_;
            
        };
        class Power : public Element{
        public:
            Power(string name);
            ~Power();
            Pin *pin(string type);
            string info();
        private:
            Pin *source_, *ground_;
        };

        /*
        class Ground : public Element{
        public:
            Ground(string name);
            Ground();
            Pin *pin(){return pin_;}
        private:
            Pin *pin_;
        };
        */
        
        class Bridge : public Element{
        public:
            Bridge(string name, Pin *pin);
            ~Bridge();
            Pin *in(){ return in_; }
            Pin *out(){ return out_; }
            Pin *outPin(Pin *inPin);
        private:
            Pin *in_, *out_;
        };

        class Switch : public Element{
            //transistor has 3 wires.
        public:
            Switch(string name);
            ~Switch();
            enum PIN_NAME{
                P0 = 0,
                P1,
                IN
            };
            //Pin *pin(PIN_NAME name);
            Pin *pin(string name);
            Pin *outPin(Pin *pin);
            bool isOn(){return isOn_;}
            void isOn(bool isOn){isOn_=isOn;}
            string info();
        private:
            Pin *p0_, *p1_, *in_;
            bool isOn_; 
        };

        //contains stuff at the current level
        int linkPins(Pin *pin0, Pin* pin1);
        //void linkWireAndPin(Wire *wire, Pin *pin);
        int unlinkPin(Pin *pin);
        //void unlinkWireAndPin(Wire *wire, Pin *pin);


        void deviceClear(Device *device);

        void devicePrint(Device *device);
        void elementPrint(Element *element);

        Device* deviceWithName(Device* device, string name);
        Element* elementWithName(Device* device, string name);
        
        Pin* pinWithName(Device* device, string name);
        bool isExistWithName(Device *device, string name);
        int forEachElement(Device *device, int (*operation)(Element *e));
    }
}

#endif
