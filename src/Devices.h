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
                Pin* pin(){ return pin_;}
                string name(){ return name_;}
            private:
                string name_;
                Pin *pin_;
        };

        //a device can have devices and basic components inside
        class Device {
            public:
                Device();
                Device(string name, string type);
                list<Device *>& devices(){return devices_;}
                Device* device(string name);
                list<Element *>& elements(){return elements_;}
                Element* element(string name);
                list<PinLabel *>& pinLabels(){return pinLabels_;}
                PinLabel* pinLabel(string name);
                string name(){return name_;}
                string type(){return type_;}
            private:
                string name_;
                string type_;
                list<Device *>devices_;
                list<Element *>elements_;
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
            //new pin
            void deletePin(Pin *pin);
            
            bool state(){ return state_; }
            void state(bool state){state_=state;}
        private:
            bool state_; //high or low
            bool reachable_; //is this reachable from plus terminal?
            list<Pin *> pins_;
        };

        class Pin{
            public:
                Pin(Wire *wire);
                void wire(Wire *wire);
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


        class Switch : public Element{
            //transistor has 3 wires.
        public:
            Switch(string name);
            enum PIN_NAME{
                P0 = 0,
                P1,
                IN
            };
            //Pin *pin(PIN_NAME name);
            Pin *pin(string name);
        private:
            Pin *_p0, *_p1, *_in;

        };

        //contains stuff at the current level
        Device* deviceWithName(Device* device, string name);
        Element* elementWithName(Device* device, string name);
        Pin* pinWithName(Device* device, string name);
        bool isExistWithName(string name);
    }
}

#endif
