#ifndef JARVIS_STATE_H
#define JARVIS_STATE_H

// singletone state class
#include "Devices.h"
namespace Jarvis{
    using namespace Devices;
    class State{
        public:
            static State& instance(){
                static State instance;
                return instance;
            }
            void device(Device *device);
            Device *device(){return device_;}

        private:
            State();
            State(State const&);
            void operator=(State const&);
            Device *device_; //current loaded device
    };
}

#endif
