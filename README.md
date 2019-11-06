# SimCore
A C++ Simulation Core

[![Build Status](https://travis-ci.com/JensKlimke/SimCore.svg?branch=sim-only)](https://travis-ci.com/JensKlimke/SimCore)

This little header only tool allows you to build your own simulation software. The framework currently supports the following base features:
* Handling components 

## Compiling
Run cmake (libraries will be installed automatically) and compile

Run cmake with -DSIMCORE_ENABLE_TESTING=0 or set SIMCORE_ENABLE_TESTING=0 in cmake file to disable testing

## Usage
See tests to understand how the framework works. I will add a description soon. Probably. 

## TODO:
* Adding a component or an interface to control the simulation (start, stop, pause, etc.)
* Add doxygen and improve Readme ;-)