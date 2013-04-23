jarvis
======

Digital logic circuit design simulation tool written in C++.

Build:
    make

Usage:
    main program:

    ./jarvis -v -h [script_file]

    script_file is the input file that contains instructions for the program
    see descriptions/ directory for examples
    if script_file is not given, the program starts in shell mode
    -v -- verbose output
    -h -- print help information

    testing freamwork:

    ./jarvist test_file

    test_file is a script that is in the same format as an input file for the main program

Introduction:

In Jarvis, devices are constructed from a few basic components called elements. For example, a not gate is constructed from power, a resistor, and a switch (transistor). These components are connected by wires.

In addition, a device can be constructed from other devices. For example, a nand gate can be constructed from an and gate and a not gate. Thus, a device consists of some number of elements and some number of devices.

An interface to a device or an element is a port. Thus two devices are connected by wiring together a port in the first device to a port in the second device. For example, a resistor has 2 ports, and a switch has 3.

At each "step" of the simulation, circuits are completed from every source to every ground that is properly connected. In addition, switches are turned on or off.

There is also a testing framework. The testing framework requires a input file (.desc) and a .expect file of the same name. The testing program invokes jarvis with the input file and stores the output to a .actual file. It then compares .expect with .actual. If the two files are exactly the same, then the test passes.


Design Language Specification:

>> load componentType componentName

loads a component

elementary componentTypes:

bridge - an external interface to other devices
    - you do not need to specify a port, as a bridge contains just one internal port and one external port

ground - ground (-) terminal of battery
    - you do not need to specify a port, as ground only has one.

meter - tool to measure voltage
    - you do not need to specify a port, as meter only has one.

resistor - simple resistor
    ports:
        p0 - "upper" or left" end of resistor
        p1 - "lower" or "right" end of resistor

source - source (+) terminal of a battery
    - you do not need to specify a port, as source only has one.

switch - transistor with 3 terminals
    - ports:
        in - input to the switch
        p0 - "upper" or "left" end of switch
        p1 - "lower" or "right" end of switch

componentType can also be a compound component consisting of 1 or more elements.

componentName:

unique name to refer to this component

>> link deviceName [pinName] deviceName [pinName]

Link two devices by a wire.
if device has only 1 pin, then it is not necessary to specify pin name.

>> unlink pinName
>> unlink devieName pinName

disconnect the pin from any wire

>> show
>> show deviceName

show shows the current canvas
show deviceName shows the device
show 1 shows just current level. 
show 2 shows current level and one level down.

>> unload
>> unload deviceName
unload removes everything from the current canvas
unload device removes some deviceID or no-op if it doesn't exist 

>> verbose on
>> verbose off

Turn verbosity on or off

>> run
>> run 1
run test. it can potentially run for a long time, so it needs to run on the background. or it may need timeouts. runs in a separate thread. if already running, then stop and run again. 

