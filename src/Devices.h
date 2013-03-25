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
            GROUND,
            TRANSISTOR,
            METER,
        };
        
        class Element{
            public:
                Element(string name, string type);
                string name(){return name_;}
                string type(){return type_;}
            protected:
                string name_;
                string type_;
        };

        class Pin;

        class PinLabel{
            public:
                PinLabel(string name, Pin *_pin);
            private:
                string name_;
                Pin *pin_;
        };

        //a device can have devices and basic components inside
        class Device : public Element{
            public:
                Device();
                Device(string name, string type);
                list<Device *>& devices(){return devices_;}
                //list<Element *>& elements(){return elements_;}
                list<PinLabel *>& pinLabels(){return pinLabels_;}
            private:
                list<Device *>devices_;
                //list<Element *>elements_;
                list<PinLabel *>pinLabels_;
        };

        //wire always has at least two devices connected to it. some conditions:
        //1. only one of the devices can be power.
        //2. only one of the devices can be ground.
        //3. only one of the devices can be input.
        //4. only one of the devices can be output.
        class Wire {
        public:
            Wire(Pin *pin0, Pin *pin1);
            list<Pin *> *pins(){return &pins_;}
            
            bool state(){ return state_; }
            void state(bool state){state_=state;}
        private:
            bool state_; //high or low
            bool reachable_; //is this reachable from plus terminal?
            list<Pin *> pins_;
        };

        class Pin {
            public:
                Pin(Wire *wire);
                void wire(Wire *wire){wire_=wire;}
                Wire *wire(){return wire_;}
            private:
                Wire *wire_;
        };

        class Meter : public Element{
        public:
            Meter(string name);
            Pin *pin(){return pin_;}
        private:
            Pin *pin_;
            
        };
        class Power : public Element{
        public:
            Power(string name);
            Pin *pin(){return pin_;}
        private:
            Pin *pin_;
        };

        class Ground : public Element{
        public:
            Ground(string name);
            Ground();
            Pin *pin(){return pin_;}
        private:
            Pin *pin_;
        };


        class Transistor : public Element{
            //transistor has 3 wires.
        public:
            Transistor(string name);
            enum PIN_NAME{
                P0 = 0,
                P1,
                IN
            };
            Pin *pin(PIN_NAME name);
        private:
            Pin *_p0, *_p1, *_in;

        };

        //contains stuff

        Element *elementWithName(Device *device, string name);
    }
}

#endif
