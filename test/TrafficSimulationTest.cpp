//
// Created by Jens Klimke on 2019-04-20.
//

#include <cmath>
#include <unordered_map>
#include <components/traffic/Environment.h>
#include <components/traffic/Agent.h>
#include <components/timers/TimeIsUp.h>
#include <socket/DataPublisher.h>
#include <data/TimeReporter.h>
#include <data/DataManager.h>
#include <core/IStorable.h>
#include <core/Loop.h>
#include <core/IComponent.h>
#include <core/functions.h>
#include <timers/RealTimeTimer.h>
#include <gtest/gtest.h>

#ifndef EPS_DISTANCE
#define EPS_DISTANCE 1e-6
#endif


struct DriverModel : public ::sim::IComponent, public Agent {

    static std::unordered_map<unsigned long, DriverModel> agents;


    double s  = 0.0;
    double v  = 10.0 / 3.6;
    double a  = 0.0;

    double ac  = 0.73;
    double bc  = 1.67;
    double v0  = 30.0 / 3.6;
    double T   = 1.5;
    double s0  = 2.0;


    double idm(double ds, double dv) {

        // calculate acceleration
        auto s_star = s0 + v * T + (v * dv / (2.0 * sqrt(ac * bc)));
        a = ac * (1.0 - pow(v / v0, 4) - pow(s_star / ds, 2));

        return a;

    }


    std::pair<bool, bool> mobil(double ds0f, double v0f, double ds1f, double v1f, double ds0b, double v0b,
            double ds1b, double v1b) {

        auto a00m = idm(ds0f, v - v0f);          // acc(M)
        auto a11m = idm(ds1f, v - v1f);          // acc'(M')
        auto a00b = idm(-ds0b, v0b - v);         // acc(B)
        auto a01b = idm(ds1f - ds1b, v1b - v1f); // acc(B')
        auto a10b = idm(ds0f - ds0b, v0b - v0f); // acc'(B)
        auto a11b = idm(-ds1b, v1f - v);         // acc'(B')

        auto bSave = 0.5;
        auto p = 1.0;
        auto aThr = 0.0;

        return {
            a11b > -bSave,                                        // safety criterion
            a11m - a00m > p * (a00b + a01b - a10b - a11b) + aThr  // incentive criterion
        };

    }


    void initialize(double initTime) override {

        initializeTimer(initTime);

    }

    bool step(double simTime) override {

        // get targets
        auto tars = this->getTargets();

        auto id_front_ego = 0;
        auto ds_front_ego = INFINITY;
        auto v_front_ego = 0.0;

        auto id_back_ego = 0;
        auto ds_back_ego = -INFINITY;
        auto v_back_ego = 0.0;

        auto id_front_tar = 0;
        auto ds_front_tar = INFINITY;
        auto v_front_tar = 0.0;

        auto id_back_tar = 0;
        auto ds_back_tar = -INFINITY;
        auto v_back_tar = 0.0;


        // get relevant target
        for(auto &tar : tars) {

            // check if distance is larger than zero
            if(tar.distance > 0.0 && tar.lane == 0 && tar.distance < ds_front_ego) {
                ds_front_ego = tar.distance;
                id_front_ego = tar.id;
            }

            // check if distance is larger than zero
            if(tar.distance < 0.0 && tar.lane == 0 && tar.distance > ds_back_ego) {
                ds_back_ego = tar.distance;
                id_back_ego = tar.id;
            }

            // check if distance is larger than zero
            if(tar.distance > 0.0 && tar.lane != 0 && tar.distance > ds_front_tar) {
                ds_front_tar = tar.distance;
                id_front_tar = tar.id;
            }

            // check if distance is larger than zero
            if(tar.distance < 0.0 && tar.lane != 0 && tar.distance > ds_back_tar) {
                ds_back_tar = tar.distance;
                id_back_tar = tar.id;
            }

        }

        // calculate acceleration
        a = idm(ds_front_ego, v - v_front_ego);

        // calculate distance
        auto dt = timeStep(simTime);
        auto ds_step = 0.5 * a * dt * dt + v * dt;

        // calculate distance and speed
        s += ds_step;
        v += a * dt;

        // move agent
        this->move(ds_step, 0.0);

        return true;

    }

    void terminate(double simTime) override {}


};

std::unordered_map<unsigned long, DriverModel> DriverModel::agents{};


class TrafficSimulationTest : public ::testing::Test, public Environment {


protected:

    ::sim::Loop sim{};
    std::string file;

public:

    void SetUp() override {

        // get map path: SIMMAP_SRC has to be provided by cmake
        std::stringstream ss{};
        ss << SIMMAP_SRC << "/tests/tracks/CircleR100.xodr";
        file = ss.str();

        // load map
        this->registerMap(file);

        // add virtual horizon as component
        sim.addComponent(this);

    }


    void TearDown() override {

        // clear all
        this->clear();

    }

};



TEST(TrafficSimulationTestModels, Models) {

    DriverModel ego{};
    DriverModel t0f{};
    DriverModel t0b{};
    DriverModel t1f{};
    DriverModel t1b{};

    // TODO:

}



TEST_F(TrafficSimulationTest, TrafficSimulation) {

    using namespace ::sim;

    // create objects
    BasicTimer timer;
    TimeIsUp stop;

    // set acceleration
    //timer.setAcceleration(5.0);

    // time reporter
    TimeReporter tr;
    tr.setTimeStepSize(1.0);

    // set parameters
    timer.setTimeStepSize(0.1);
    stop.setStopTime(400.0);

    // data manager
    sim::data::DataManager data;
    sim::data::DataPublisher pub;
    pub.setDataManager(&data);

    // todo add time to data manager

    // set timer and stop condition
    sim.setTimer(&timer);
    sim.addStopCondition(&stop);

    // models
    sim.addComponent(&tr);
    sim.addComponent(&pub);
    sim.addComponent(&stop);
    sim.addComponent(this);

    // define agent size
    unsigned int n = 10;

    // create containers
    std::vector<const sim::data::IStorable*> store;
    store.reserve(n);

    // init agents
    for(unsigned int i = 0; i < n; ++i) {

        // create id
        auto id = i + 1;

        // create IDM object
        DriverModel::agents[id] = DriverModel{};
        auto dm = &DriverModel::agents[id];

        // create agent
        this->createAgent(dm, id, {"1", "-2"});

        // set velocity
        dm->v0 -= (2.0 * i / 3.6);

        // place agent
        auto s = (2 * M_PI * 100.0 / n) * i;
        if(s < M_PI * 50.0)
            dm->setMapPosition("R1-LS1-R1", s, 0.0);
        else if(s < M_PI * 100.0)
            dm->setMapPosition("R1-LS2-R1", s - M_PI * 50.0, 0.0);
        else
            dm->setMapPosition("R2-LS1-L1", s - M_PI * 100.0, 0.0);

        // save agent to storable list and add as component
        store.emplace_back(dm);
        sim.addComponent(dm);

    }

    // save agents to data manager
    data.registerStorableVector("agent", store);

    sim.run();

}