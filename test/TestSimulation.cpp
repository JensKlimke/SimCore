//
// Created by Jens Klimke on 2019-03-23.
//

#include <memory>
#include <tuple>
#include <core/Loop.h>
#include <traffic/Agent.h>
#include <traffic/VehicleModel.h>
#include <traffic/PrimaryController.h>
#include <components/timers/BasicTimer.h>
#include <components/timers/RealTimeTimer.h>
#include <components/timers/TimeIsUp.h>
#include <components/data/JsonFileReporter.h>
#include <components/data/TimeReporter.h>
#include <components/value/ValueExceed.h>
#include <core/Loop.h>
#include <cmath>


class TestSimulation {


public:

    struct SimSetup {

        struct SimAgent : public Agent{
            double  pedal;
            double  steer;
            double  v0;
            double* control[2];
        };

        double stepSize = 0.01;
        double endTime = INFINITY;
        double endDistance = INFINITY;
        bool realTime = false;
        bool timeReport = false;

        std::string logFile;
        std::vector<SimAgent> agents{};

    };

    struct Unit : public Agent {

        std::unique_ptr<VehicleModel> vehicle;

        std::unique_ptr<PrimaryController> pedal;
        std::unique_ptr<PrimaryController> steer;

        VehicleModel::State      *vehState = nullptr;
        VehicleModel::Input      *vehInput = nullptr;
        VehicleModel::Parameters *vehParam = nullptr;

    };

    sim::Loop loop{};

    std::unique_ptr<BasicTimer> timer;
    std::unique_ptr<TimeIsUp> stopTime;
    std::unique_ptr<ValueExceed<double>> stopDist;
    std::unique_ptr<TimeReporter> rep;
    std::unique_ptr<JsonFileReporter> jsonLog;

    std::vector<std::unique_ptr<Unit>> units{};

    TestSimulation() = default;
    ~TestSimulation() = default;


    void create(const SimSetup &setup) {


        // create timer
        timer.reset(setup.realTime ? new RealTimeTimer : new BasicTimer);
        timer->setTimeStepSize(setup.stepSize);

        // add timer
        loop.setTimer(timer.get());

        // time report
        if(setup.timeReport) {

            // create and add time reporter
            rep = std::make_unique<TimeReporter>();
            loop.addComponent(rep.get());

            // set step size
            rep->setTimeStepSize(1.0);

        }

        // set stop condition (time)
        if(setup.endTime < 1e9) {
            stopTime = std::make_unique<TimeIsUp>();
            stopTime->setStopTime(setup.endTime);
        }

        // add stop condition
        loop.addStopCondition(stopTime.get());
        loop.addComponent(stopTime.get());

        // create JSON log
        jsonLog = std::make_unique<JsonFileReporter>();
        jsonLog->setFilename(setup.logFile);


        // create agents
        unsigned int i = 0;
        for(auto &ag : setup.agents) {

            // create agent_tests
            auto unit = new Unit;
            unit->vehicle = std::make_unique<VehicleModel>();

            // add vehicle model
            loop.addComponent(unit->vehicle.get());

            // link state, input and parameters
            unit->vehicle->getStates(&unit->vehState);
            unit->vehicle->getInputs(&unit->vehInput);
            unit->vehicle->getParameters(&unit->vehParam);

            // initialize state
            unit->vehState->xy[0] = 0.0;
            unit->vehState->xy[1] = 0.0;
            unit->vehState->v     = ag.v0;
            unit->vehState->s     = 0.0;
            unit->vehState->a     = 0.0;
            unit->vehState->dPsi  = 0.0;
            unit->vehState->psi   = 0.0;

            // initialize inputs
            unit->vehInput->pedal = ag.pedal;
            unit->vehInput->steer = ag.steer;
            unit->vehInput->slope = 0.0;

            // something which is only for agent_tests 0
            if(i == 0) {

                // create report model
                jsonLog->addValue("x", &unit->vehState->xy[0]);
                jsonLog->addValue("y", &unit->vehState->xy[1]);
                jsonLog->addValue("a", &unit->vehState->a);
                jsonLog->addValue("v", &unit->vehState->v);
                jsonLog->addValue("dPsi", &unit->vehState->dPsi);
                jsonLog->addValue("pedal", &unit->vehInput->pedal);
                jsonLog->addValue("steer", &unit->vehInput->steer);

                // set stop condition (distance)
                if(setup.endDistance < 1e9) {
                    stopDist = std::make_unique<ValueExceed<double>>();
                    stopDist->setValueAndLimit(&unit->vehState->s, setup.endDistance);
                }

            }


            if(ag.control[0] != nullptr) {

                if(i == 0) {

                    // log desired value
                    jsonLog->addValue("aDes", ag.control[0]);
                    jsonLog->addValue("dPsiDes", ag.control[1]);

                }

                // pedal controller
                unit->pedal = std::make_unique<PrimaryController>();
                unit->pedal->setParamters(1.0, 0.0, 0.0);
                unit->pedal->setVariables(&unit->vehState->a, ag.control[0], &unit->vehInput->pedal);

                // steering controller
                unit->steer = std::make_unique<PrimaryController>();
                unit->steer->setParamters(1.0, 0.0, 0.0);
                unit->steer->setVariables(&unit->vehState->dPsi, ag.control[1], &unit->vehInput->steer);

                // add models to sim
                loop.addComponent(unit->pedal.get());
                loop.addComponent(unit->steer.get());

            }


            // add unit to vector
            units.emplace_back(std::unique_ptr<Unit>(unit));

            i++;

        }

        // add stop condition (distance)
        if(stopDist != nullptr) {
            loop.addStopCondition(stopDist.get());
            loop.addComponent(stopDist.get());
        }

        // add json logger
        loop.addComponent(jsonLog.get());

    }

};

