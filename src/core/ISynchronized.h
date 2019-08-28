//
// Created by klimke on 27.03.2019.
//

#ifndef SIMCORE_SYNCHRONIZED_H
#define SIMCORE_SYNCHRONIZED_H

#include "IComponent.h"

#ifndef EPS_TIME
#define EPS_TIME 1e-9
#endif

namespace sim {

    class ISynchronized : public sim::IComponent {

        double _timeStepSize;
        double _nextExecTime;

    public:

        /**
         * Initializes the component. Increases the nexte execution time by the initialization time
         * @param initTime Initialization time
         */
        void initialize(double initTime) override {

            _nextExecTime += initTime;

        }


        /**
         * Performs a simulation step, when the next execution time is reached
         * @param simTime Current simulation time
         * @return Returns a flag whether the step is executed or skipped
         */
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

#endif //SIMCORE_SYNCHRONIZED_H
