# SimCore
A C++ Simulation Core

This little header only tool allows you to build your own simulation software. The framework currently supports the following base features:
* Handling components 

## Compiling
Run cmake (libraries will be installed automatically) and compile

Run cmake with -DSIMCORE_ENABLE_TESTING=0 or set SIMCORE_ENABLE_TESTING=0 in cmake file to disable testing

## Usage
See tests to understand how the framework works. I will add a description soon. Probably. 

## TODO:
* Improve the live visualisation
    * Add list of available variables
    * Plot variables
    * Add simulation time instead of current timestamp 
    * Add road model plot
    * Dashboard style
    * Zooming in plots
    * Show acceleration and velocity vectors (and other values) in xy-plot
    * Show driver model related values in xy-plot
    * Communication with simulation (pause, stop, restart, etc.)
    * Create cache to replay in browser (enable sim or server to log data locally and provide after simulation has been finished)

