//
// Created by Jens Klimke on 2019-03-23.
//

#ifndef SIMULATION_FRAMEWORK_TESTSIMULATION_H
#define SIMULATION_FRAMEWORK_TESTSIMULATION_H

#include <tuple>
#include <core/Loop.h>
#include <components/traffic/VehicleModel.h>
#include <components/traffic/PrimaryController.h>
#include <components/timers/BasicTimer.h>
#include <components/timers/RealTimeTimer.h>
#include <components/timers/TimeIsUp.h>
#include <components/data/JsonReporter.h>
#include <components/value/ValueExceed.h>
#include <core/Loop.h>
#include <cmath>


class TestSimulation {


public:

    struct SimSetup {

        struct Agent {
            double  pedal;
            double  steer;
            double  v0;
            bool    agent;
            double* control[2];
        };

        double stepSize = 0.01;
        double endTime = INFINITY;
        double endDistance = INFINITY;
        bool realTime = false;
        bool timeReport = false;
        bool liveLog = false;
        std::string logFile;
        std::vector<Agent> agents{};

    };

    struct Agent {

        AgentModel               *agent = nullptr;
        VehicleModel             *vehicle = nullptr;
        AgentModelAdapter        *adapter = nullptr;

        VehicleModel::State      *vehState = nullptr;
        VehicleModel::Input      *vehInput = nullptr;
        VehicleModel::Parameters *vehParam = nullptr;

        AgentModel::State        *agState = nullptr;
        AgentModel::Input        *agInput = nullptr;
        AgentModel::Parameters   *agParam = nullptr;
        AgentModel::Injection    *agInjec = nullptr;

        PrimaryController *pedal = nullptr;
        PrimaryController *steer = nullptr;

    };

    sim::Loop loop{};

    BasicTimer *timer = nullptr;
    TimeIsUp   *stopTime  = nullptr;
    ValueExceed<double> *stopDist = nullptr;
    TimeReporter *rep = nullptr;
    JsonReporter *jsonLog = nullptr;
    AgentsLogger *liveLog = nullptr;
    std::vector<Agent> agents{};


    TestSimulation() = default;


    ~TestSimulation() {

        delete timer;
        delete stopTime;
        delete stopDist;
        delete rep;
        delete jsonLog;

        for(auto &ag : agents) {

            delete ag.agent;
            delete ag.vehicle;
            delete ag.adapter;
            delete ag.pedal;
            delete ag.steer;

        }

    }


    void create(const SimSetup &setup) {


        // create timer
        timer = setup.realTime ? new RealTimeTimer : new BasicTimer;
        timer->setTimeStepSize(setup.stepSize);

        // add timer
        loop.setTimer(timer);

        // time report
        if(setup.timeReport) {

            rep = new TimeReporter;
            loop.addModel(rep);
            rep->setTimeStepSize(1.0);

        }

        // set stop condition (time)
        if(setup.endTime < 1e9) {
            stopTime = new TimeIsUp;
            stopTime->setStopTime(setup.endTime);
        }

        // add stop condition
        loop.addStopCondition(stopTime);
        loop.addModel(stopTime);

        // create JSON log
        jsonLog = new JsonReporter;
        jsonLog->setFilename(setup.logFile);

        // create live log
        if(setup.liveLog) {
            liveLog = new AgentsLogger;
            liveLog->setFilename("log/live.json");
            liveLog->setTimeStepSize(0.1);
            liveLog->setSize(0.0, -100.0, 500.0, 100.0);
        }

        // reserve space
        agents.clear();
        agents.reserve(setup.agents.size());

        // create agents
        unsigned int i = 0;
        for(auto &ag : setup.agents) {

            // create agent_tests
            Agent unit{};
            unit.vehicle = new VehicleModel;

            // add vehicle model
            loop.addModel(unit.vehicle);

            // link state, input and parameters
            unit.vehicle->getState(reinterpret_cast<void**>(&unit.vehState));
            unit.vehicle->getInput(reinterpret_cast<void**>(&unit.vehInput));
            unit.vehicle->getParameters(reinterpret_cast<void**>(&unit.vehParam));

            // initialize state
            unit.vehState->xy[0] = 0.0;
            unit.vehState->xy[1] = 0.0;
            unit.vehState->v     = ag.v0;
            unit.vehState->s     = 0.0;
            unit.vehState->a     = 0.0;
            unit.vehState->dPsi  = 0.0;
            unit.vehState->psi   = 0.0;

            // initialize inputs
            unit.vehInput->pedal = ag.pedal;
            unit.vehInput->steer = ag.steer;
            unit.vehInput->slope = 0.0;

            // set live log
            AgentsLogger::Agent la{};
            if(setup.liveLog) {

                // create agent_tests and add
                la.x = &unit.vehState->xy[0];
                la.y = &unit.vehState->xy[1];
                la.psi = &unit.vehState->psi;
                la.length = &unit.vehParam->size[0];
                la.width = &unit.vehParam->size[1];
                la.driverPosX = nullptr;
                la.driverPosY = nullptr;

            }

            // something which is only for agent_tests 0
            if(i == 0) {

                // create report model
                jsonLog->addValue("x", &unit.vehState->xy[0]);
                jsonLog->addValue("y", &unit.vehState->xy[1]);
                jsonLog->addValue("a", &unit.vehState->a);
                jsonLog->addValue("v", &unit.vehState->v);
                jsonLog->addValue("dPsi", &unit.vehState->dPsi);
                jsonLog->addValue("pedal", &unit.vehInput->pedal);
                jsonLog->addValue("steer", &unit.vehInput->steer);

                // set stop condition (distance)
                if(setup.endDistance < 1e9) {
                    stopDist = new ValueExceed<double>();
                    stopDist->setValueAndLimit(&unit.vehState->s, setup.endDistance);
                }

            }


            // create agent_tests model
            if(ag.agent) {

                // create agent_tests
                unit.agent = new AgentModel;

                if(i == 0 && setup.injection != nullptr) {

                    // add to injection and adapter
                    setup.injection->setAgent(unit.agent, unit.vehicle);
                    loop.addModel(setup.injection);

                }

                // create adapter
                unit.adapter = new AgentModelAdapter;
                unit.adapter->setUnit(unit.agent, unit.vehicle);

                // get parameters, states, input and injection
                unit.agent->getParameters(reinterpret_cast<void**>(&unit.agParam));
                unit.agent->getInput(reinterpret_cast<void**>(&unit.agInput));
                unit.agent->getState(reinterpret_cast<void**>(&unit.agState));
                unit.agent->getInjection(&unit.agInjec);

                // update standard parameters
                createStandardParameters(unit.agParam);

                // pedal controller
                unit.pedal = new PrimaryController;
                unit.pedal->setParamters(1.0, 0.0, 0.0);
                unit.pedal->setValues(&unit.vehState->a, &unit.agState->subConscious.aDes, &unit.vehInput->pedal);

                // steering controller
                unit.steer = new PrimaryController;
                unit.steer->setParamters(3.0, 0.0, 0.0);
                unit.steer->setValues(&unit.vehState->dPsi, &unit.agState->subConscious.dPsiDes, &unit.vehInput->steer);

                // add models
                loop.addModel(unit.adapter);
                loop.addModel(unit.agent);
                loop.addModel(unit.pedal);
                loop.addModel(unit.steer);

                // set live log values
                if(setup.liveLog) {

                    // create agent_tests and add;
                    la.driverPosX = &unit.agParam->vehicle.pos.x;
                    la.driverPosY = &unit.agParam->vehicle.pos.y;

                }

                // logger
                if(i == 0) {

                    // add log values
                    jsonLog->addValue("vDes",    &unit.agState->conscious.vDes);
                    jsonLog->addValue("aDes",    &unit.agState->subConscious.aDes);
                    jsonLog->addValue("dPsiDes", &unit.agState->subConscious.dPsiDes);
                    jsonLog->addValue("aux",     &unit.agState->aux);

                }

            } else if(ag.control[0] != nullptr) {

                if(i == 0) {

                    // log desired value
                    jsonLog->addValue("aDes", ag.control[0]);
                    jsonLog->addValue("dPsiDes", ag.control[1]);

                }

                // pedal controller
                unit.pedal = new PrimaryController;
                unit.pedal->setParamters(1.0, 0.0, 0.0);
                unit.pedal->setValues(&unit.vehState->a, ag.control[0], &unit.vehInput->pedal);

                // steering controller
                unit.steer = new PrimaryController;
                unit.steer->setParamters(1.0, 0.0, 0.0);
                unit.steer->setValues(&unit.vehState->dPsi, ag.control[1], &unit.vehInput->steer);

                // add models to sim
                loop.addModel(unit.pedal);
                loop.addModel(unit.steer);

            }

            // add agent_tests to live log
            if(liveLog)
                liveLog->addAgent(la);


            // add unit to vector
            agents.emplace_back(unit);

            i++;

        }

        // add stop condition (distance)
        if(stopDist != nullptr) {
            loop.addStopCondition(stopDist);
            loop.addModel(stopDist);
        }

        // add loggers to sim
        if(setup.liveLog)
            loop.addModel(liveLog);

        loop.addModel(jsonLog);

    }

};


#endif //SIMULATION_FRAMEWORK_TESTSIMULATION_H
