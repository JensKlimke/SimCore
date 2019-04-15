//
// Created by klimke on 27.03.2019.
//

#ifndef SIMULATION_FRAMEWORK_SYNCHRONIZED_H
#define SIMULATION_FRAMEWORK_SYNCHRONIZED_H

#include "IModel.h"

#ifndef EPS_TIME
#define EPS_TIME 1e-9
#endif

namespace sim {

    class ISynchronized : public sim::IModel {

        double _timeStepSize;
        double _nextExecTime;

    public:

        void initialize(double initTime) override {

            _nextExecTime += initTime;

        }

        bool step(double simTime) override {

            if(simTime + EPS_TIME >= _nextExecTime) {

                _nextExecTime += _timeStepSize;
                return true;

            }

            return false;

        }


        /**
         * Sets the time step size of the model
         * @param timeStepSize Time step size
         */
        void setTimeStepSize(double timeStepSize, double firstExecutionAfterStart = 0.0) {

            _timeStepSize = timeStepSize;
            _nextExecTime = firstExecutionAfterStart;

        }

    };


} // namespace ::sim

#endif //SIMULATION_FRAMEWORK_SYNCHRONIZED_H
