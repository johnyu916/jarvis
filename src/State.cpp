#include "State.h"

namespace Jarvis{
    State::State(){
        device_ = new Device();
    }
    void State::device(Device *device){
        if (device_) delete device_;
        device_ = device;
    }
}
