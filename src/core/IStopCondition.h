//
// Created by klimke on 12.03.2019.
//

#ifndef SIMCORE_STOPCONDITION_H
#define SIMCORE_STOPCONDITION_H

#include <string>

namespace sim {



    class IStopCondition {

    public:

        enum class StopCode { NONE, OBJECTIVES_MISSED, OBJECTIVES_REACHED, SIM_ENDED };


    private:

        StopCode _code = StopCode::NONE;


    public:


        /**
         * Constructor for stop condition
         */
        IStopCondition() = default;


        /**
         * Default destructor
         */
        ~IStopCondition() = default;


        /**
         * Checks if the stop condition is set to a stopped state
         * @return
         */
        virtual bool hasStopped() const {

            return getCode() != StopCode::NONE;

        }


        /**
         * Resets the stop condition
         */
        virtual void reset() {

            _code = StopCode::NONE;

        }


        /**
         * Returns the stop code
         * @return
         */
        StopCode getCode() const {

            return _code;

        }


    protected:


        /**
         * Call stop command with given code
         * @param code Code to be set
         */
        void stop(StopCode code) {

            _code = code;

        }


        /**
         * Call stop command (with objectives missed)
         */
        void failed() {

            _code = StopCode::OBJECTIVES_MISSED;

        }


        /**
         * Call stop command (with objectives reached)
         */
        void success() {

            _code = StopCode::OBJECTIVES_REACHED;

        }


        /**
         * Call stop command (with simulation ended regularly)
         */
        void end() {

            _code = StopCode::SIM_ENDED;

        }

    };

}


#endif //SIMCORE_STOPCONDITION_H
