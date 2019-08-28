//
// Created by Jens Klimke on 2019-03-19.
//

#ifndef SIMCORE_ITIMER_H
#define SIMCORE_ITIMER_H

namespace sim {

    class ITimer {

    public:


        /**
         * Default constructor
         */
        ITimer() = default;


        /**
         * Default deconstructor
         */
        virtual ~ITimer() = default;


        /**
         * Performs a step
         */
        virtual void step() = 0;


        /**
         * Resets the timer
         */
        virtual void start() = 0;


        /**
         * Stops the timer
         */
        virtual void stop() = 0;


        /**
         * Returns the current simulation time
         * @return
         */
        virtual double time() const = 0;


        /**
         * Resets the timer
         */
        virtual void reset() = 0;

    };

} // namespace ::sim


#endif //SIMCORE_TIMER_H
