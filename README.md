# Command Runner Application
## Overview
The Command Runner is a framework for executing sequences of commands. Each command has its own implementation.
Commands are read from a specification file, passed as an argument to the executable when run on the command line. See below for an example of such a sequence.
Sequences can be nested.
## Requirements
1.	Each command will be called to respond for the following event:
a.	Execution started
b.	Execution progress
c.	Execution ended
2.	Each command will have:
a.	pre-execution sequnce
b.	post execution sequence
3.	Commands will be added in the desired execution order
4.	Commands have the ability to run in the following order:
a.	Sequensially (one at a time)
b.	Parallel (if part of the commands are grouped for parallel run)
5.	We will have a special command for:
a.	Loop a series of commands until we reach a given repeat number or a stop criterion is satisfied
b.	Branch the execution. Meaning, if a given criteria is satisfied, we will execute one commend, otherwise we will execute a different command
6.	Once we start the command runner execution, it will run the commands in the added sequence
7.	Once we cancel the runner, we will stop execution
8.	Runs are single threaded only
## Supporting Modules
The following modules are available for the commands:
### Environment
The environment module manages states of a collection of variables that represent the physical environment of the robot. It includes the robot location, orientation, and sensors, all in unit distance:
1. **Location**: two variables that represent the X and Y planar location.
2. **Yaw (X)**: represents the angle from "north", positive=clockwise.
3. **Height (Y)**: represents a relative movement of the fork-lift, positive=up..
4. **Movement (Z)**: represents a relative movement of the robot, positive=forward.
5. **Fork Load**: true when the fork is loaded with a bin.
### Logger
A logging facility based on the fine Spdlog open-source library by Gabi Melman. Supports severity level messaging, both to console as well as to a rotating file collection.
### getopt
A getopt implementation was incorporated to enable robust argument parsing. Code was taken from the [IoTivity Library](https://github.com/iotivity/iotivity/tree/master/resource/c_common/windows).
## Example of a specification file
(Also available under ./test/TestSimple.txt)
```
# Test file for a simple, happy flow.

Move-to 10 10
Move-to 10 -20
Parallel
    Turn right 90
    Lift-fork 10
End
Loop 3 X eq 0
    Move 500
    Lift-fork 10
    Move 500
End
If fork-sensor eq true
    Turn right 90
Else
    Raise-error No bin on fork
End
Stop
```
## Known Issues and Caveats
1. It's currently not possible to nest parallel execution (i.e. only one level of parallel sub-sequences exists).
2. Only the happy flow was tested. The implementation does little in terms of error checking.