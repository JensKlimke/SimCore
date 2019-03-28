//
// Created by Jens Klimke on 2019-03-19.
//

#ifndef SIMULATION_FRAMEWORK_TESTPLUGIN_H
#define SIMULATION_FRAMEWORK_TESTPLUGIN_H

#include <core/ITimer.h>
#include <core/IPlugin.h>



class TestPlugin : public ::sim::ITimer, public ::sim::IPlugin {

public:

    struct Parameters {
        double *stepSize = nullptr;
    };

private:

    double _time{};
    double _stepSize = 1.0;


public:


    TestPlugin() = default;

    ~TestPlugin() override = default;

    void step() override;

    void start() override;

    void stop() override;

    double time() const override;

    void reset() override;


    bool getInput(void **input) override;

    bool getState(void **state) override;

    bool getParameters(void **param) override;

};




#endif //SIMULATION_FRAMEWORK_STEADYTIMER_H
