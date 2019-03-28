//
// Created by Jens Klimke on 2019-03-23.
//

#ifndef SIMULATION_FRAMEWORK_TESTSIMULATION_H
#define SIMULATION_FRAMEWORK_TESTSIMULATION_H

#include <tuple>
#include <core/Loop.h>
#include <models/VehicleModel.h>
#include <models/BasicTimer.h>
#include <models/RealTimeTimer.h>
#include <models/TimeIsUp.h>
#include <models/JsonReporter.h>
#include <models/PrimaryController.h>
#include <models/agent/AgentModel.h>
#include <models/agent/AgentModelAdapter.h>
#include <models/agent/AgentInjection.h>
#include <models/agent/AgentsLogger.h>
#include <models/TimeReporter.h>
#include <core/Loop.h>
#include "AgentStandardParameters.h"



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
        double endTime;
        bool realTime = false;
        bool timeReport = false;
        bool liveLog = false;
        std::string logFile;
        AgentInjection *injection = nullptr;
        std::vector<Agent> agents{};

    };


protected:

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

    sim::Loop sim{};

    BasicTimer *timer = nullptr;
    TimeIsUp   *stop  = nullptr;
    TimeReporter *rep = nullptr;
    JsonReporter *jsonLog = nullptr;
    AgentsLogger *liveLog = nullptr;
    std::vector<Agent> agents{};


public:


    TestSimulation() = default;


    ~TestSimulation() {

        delete timer;
        delete stop;
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
        sim.setTimer(timer);

        // time report
        if(setup.timeReport) {

            rep = new TimeReporter;
            sim.addModel(rep);
            rep->setTimeStepSize(1.0);

        }

        // set stop condition
        stop = new TimeIsUp;
        stop->setStopTime(setup.endTime);

        // add stop condition
        sim.addStopCondition(stop);
        sim.addModel(stop);

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

            // create agent
            Agent unit{};
            unit.vehicle = new VehicleModel;

            // add vehicle model
            sim.addModel(unit.vehicle);

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

                // create agent and add
                la.x = &unit.vehState->xy[0];
                la.y = &unit.vehState->xy[1];
                la.psi = &unit.vehState->psi;
                la.length = &unit.vehParam->size[0];
                la.width = &unit.vehParam->size[1];
                la.driverPosX = nullptr;
                la.driverPosY = nullptr;

            }

            // add json logger
            if(i == 0) {

                // create report model
                jsonLog->addValue("x", &unit.vehState->xy[0]);
                jsonLog->addValue("y", &unit.vehState->xy[1]);
                jsonLog->addValue("a", &unit.vehState->a);
                jsonLog->addValue("v", &unit.vehState->v);
                jsonLog->addValue("dPsi", &unit.vehState->dPsi);
                jsonLog->addValue("pedal", &unit.vehInput->pedal);
                jsonLog->addValue("steer", &unit.vehInput->steer);

            }


            // create agent model
            if(ag.agent) {

                // create agent
                unit.agent = new AgentModel;

                if(i == 0 && setup.injection != nullptr) {

                    // add to injection and adapter
                    setup.injection->setAgent(unit.agent, unit.vehicle);
                    sim.addModel(setup.injection);

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
                unit.steer->setParamters(1.0, 0.0, 0.0);
                unit.steer->setValues(&unit.vehState->dPsi, &unit.agState->subConscious.dPsiDes, &unit.vehInput->steer);

                // add models
                sim.addModel(unit.adapter);
                sim.addModel(unit.agent);
                sim.addModel(unit.pedal);
                sim.addModel(unit.steer);

                // set live log values
                if(setup.liveLog) {

                    // create agent and add;
                    la.driverPosX = &unit.agParam->vehicle.pos.x;
                    la.driverPosY = &unit.agParam->vehicle.pos.y;

                }

                // logger
                if(i == 0) {

                    // add log values
                    jsonLog->addValue("vDes",    &unit.agState->conscious.vDes);
                    jsonLog->addValue("aDes",    &unit.agState->subConscious.aDes);
                    jsonLog->addValue("dPsiDes", &unit.agState->subConscious.dPsiDes);

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
                sim.addModel(unit.pedal);
                sim.addModel(unit.steer);

            }

            // add agent to live log
            if(liveLog)
                liveLog->addAgent(la);


            // add unit to vector
            agents.emplace_back(unit);

            i++;

        }

        // add loggers to sim
        if(setup.liveLog)
            sim.addModel(liveLog);

        sim.addModel(jsonLog);

    }

};


#endif //SIMULATION_FRAMEWORK_TESTSIMULATION_H
