//
// Created by Jens Klimke on 2019-04-20.
//

#include <gtest/gtest.h>
#include <components/traffic/Environment.h>
#include <components/traffic/Agent.h>
#include <components/timers/TimeIsUp.h>
#include <core/Loop.h>
#include <core/IComponent.h>
#include <cmath>
#include <timers/BasicTimer.h>

#ifndef EPS_DISTANCE
#define EPS_DISTANCE 1e-6
#endif


struct IDM : public ::sim::IComponent {


    Agent *ag = nullptr;

    double dv = 0.0;
    double v  = 0.0;
    double s  = 0.0;
    double a  = 0.0;


    void initialize(double initTime) override {

        initializeTimer(initTime);

    }

    bool step(double simTime) override {

        // parameters
        double ac  = 0.73;
        double bc  = 1.67;
        double v0  = 100.0 / 3.6;
        double T   = 1.5;
        double s0  = 2.0;

        // calculate acceleration
        auto s_star = s0 + v * T + (v * dv / (2.0 * sqrt(ac * bc)));
        a = ac * (1.0 - pow(v / v0, 4) - pow(s_star / s, 2));

        // calculate distance
        auto dt = timeStep(simTime);
        auto ds = 0.5 * a * dt * dt + v * dt;

        // move agent
        ag->move(ds, 0.0);

        return true;

    }

    void terminate(double simTime) override {}


};



class EnvironmentTest : public ::testing::Test, public Environment {


protected:

    ::sim::Loop sim{};


public:

    void SetUp() override {

        // get map path: SIMMAP_SRC has to be provided by cmake
        std::stringstream ss;
        ss << SIMMAP_SRC << "/tests/tracks/CircleR100.xodr";

        // load map
        this->registerMap(ss.str());

        // add virtual horizon as component
        sim.addComponent(this);


    }


    void TearDown() override {

        // clear all
        this->clear();

    }

};



TEST_F(EnvironmentTest, TrafficSimulation) {

    using namespace ::sim;

    // create objects
    BasicTimer timer;
    TimeIsUp stop;

    // set parameters
    timer.setTimeStepSize(0.1);
    stop.setStopTime(10.0);

    // create loop
    Loop sim;

    // set timer and stop condition
    sim.setTimer(&timer);
    sim.addStopCondition(&stop);

    // models
    sim.addComponent(&stop);
    sim.addComponent(this);


    // create agent vector
    unsigned int n = 1;
    std::vector<IDM> agents(n);

    // init agents
    for(unsigned int i = 0; i < n; ++i) {

        // add agent
        auto agent = this->createAgent(i + 1, {"1", "-2"});
        agents[i].ag = agent;

        // place agent
        double s = (double) i * 10 / (2 * M_PI * 100.0);
        if(s < M_PI * 50.0)
            agent->setMapPosition("R1-LS1-R1", s, 0.0);
        else if(s < M_PI * 100.0)
            agent->setMapPosition("R1-LS2-R1", s - M_PI * 50.0, 0.0);
        else
            agent->setMapPosition("R2-LS1-L1", s - M_PI * 100.0, 0.0);

        // add agent as component
        sim.addComponent(&agents[i]);

    }

    sim.run();

}