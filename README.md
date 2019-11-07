# SimCore
A C++ Simulation Core

[![Build Status](https://travis-ci.com/JensKlimke/SimCore.svg?branch=master)](https://travis-ci.com/JensKlimke/SimCore)

This little header only tool allows you to build your own simulation software. The framework currently supports the following base features:
* Handling and registering components (or simulation models) synchronously with defined time step sizes 
* (Accelerated) pseudo real time
* Organization of simulation and model signals in a data manager
* Save data to JSON log files
* Publish simulation signals via websocket 
* Stopping the simulation by user-specific stop conditions
* Create signal curves and tubes to check simulation signals

That's all for the first. 

## Compiling

The project is designed as a header only project. You can include the file from the `include` folder. There is a websocket functionality (`DataPublisher.h`) which requires some cpp files to be built to a static library. This functionality can be activated by the cmake option `BUILD_WEBSOCKET`.

To build the project including the tests and the websocket functionality, just create and compile the project using cmake like this: 

`cmake -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTS=ON -DBUILD_WEBSOCKET=ON ..`

## Usage
See tests to understand how the framework works. I will add a description soon. Probably. 

## TODO:

Features: 
* Adding a component or an interface to control the simulation (start, stop, pause, etc.)
* Register all parameters of core components for data manager

Testing:
* Add nodejs script to receive published data (store to file and check file after test)

Project:
* Add doxygen and improve Readme ;-)