#ifndef JARVIS_DEVICES_H
#define JARVIS_DEVICES_H

#include <list>
#include <string>

using namespace std;
namespace Jarvis{
    namespace Devices{
        //primitive device types
        enum DEVICE_TYPE{
            SOURCE = 0,
            GROUND,
            RESISTOR,
            BRIDGE,
            SWITCH,
            METER,
        };

        class Device;
        
        class Element{
            public:
                Element(string name, string type, Device *parent);
                string name(){return name_;}
                string fullName();
                string type(){return type_;}
                virtual string info();
                Device *parent(){return parent_;}
            protected:
                Device *parent_;
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
                Device(string name, string type, Device *parent);
                list<Device *>& devices(){return devices_;}
                Device* device(string name);
                list<Element *>& elements(){return elements_;}
                Element* element(string name);
                //list<Bridge *>& bridges(){  return bridges_;}
                //Bridge *bridge(string name);
                //list<PinLabel *>& pinLabels(){return pinLabels_;}
                //PinLabel* pinLabel(string name);
                string name(){return name_;}
                Device *parent(){return parent_;}
                string type(){return type_;}
            private:
                string name_;
                string type_;
                Device *parent_;
                list<Device *>devices_;
                list<Element *>elements_;
                //list<Bridge *>bridges_;
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
            string name();
            
            bool voltage(){ return voltage_; }
            void voltage(bool state){voltage_=state;}
        private:
            bool voltage_; //high or low
            //bool reachable_; //is this reachable from plus terminal?
            list<Pin *> pins_;
        };

        class Pin{
            public:
                Pin(Element *element, string name);
                ~Pin();
                string name(){ return name_;}
                string fullName();
                string info();
                bool visited(){ return visited_;}
                void visited(bool visited){ visited_=visited;}
                void wire(Wire *wire){ wire_=wire;}
                Wire *wire(){return wire_;}
                Element *element(){return element_;}
            private:
                Wire *wire_;
                bool visited_;

                Element *element_;
                string name_;
        };

        //an active resistor has current flowing through it
        class Resistor : public Element{
            public:
                Resistor(string name, Device *parent);
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
            Meter(string name, Device *parent);
            ~Meter();
            Pin *pin(){return pin_;}
            string info();
        private:
            Pin *pin_;
            
        };
        class Source :public Element{
        public:
            Source(string name, Device *parent);
            ~Source();
            string info();
            Pin *pin(){return pin_;}
        private:
            Pin *pin_;
        };
        /*
        class Power : public Element{
        public:
            Power(string name);
            ~Power();
            Pin *pin(string type);
            string info();
        private:
            Pin *source_, *ground_;
        };
        */

        class Ground : public Element{
        public:
            Ground(string name, Device *parent);
            ~Ground();
            string info();
            Pin *pin(){return pin_;}
        private:
            Pin *pin_;
        };
        
        class Bridge : public Element{
        public:
            Bridge(string name, Pin *pin, Device *parent);
            ~Bridge();
            Pin *in(){ return in_; }
            Pin *out(){ return out_; }
            Pin *outPin(Pin *inPin);
            string info();

        private:
            Pin *in_, *out_;
        };

        class Switch : public Element{
            //transistor has 3 wires.
        public:
            Switch(string name, Device *parent);
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

        void devicePrint(Device *device, int level);
        void elementPrint(Element *element);

        Device* deviceWithName(Device* device, string name);
        Element* elementWithName(Device* device, string name);
        
        Pin* pinWithName(Device* device, string name);
        bool isExistWithName(Device *device, string name);
        int forEachElement(Device *device, int (*operation)(Element *e));
    }
}

#endif
