//
// Created by Jens Klimke on 2019-03-19.
//

#ifndef SIMCORE_ICOMPONENT_H
#define SIMCORE_ICOMPONENT_H


namespace sim {


    class IComponent {


        double _last_time_step{};


    public:


        /**
         * Default constructor
         */
        IComponent() = default;


        /**
         * Default destructor
         */
        virtual ~IComponent() = default;


        /**
         * Handles the initialization
         * @return Success flag
         */
        virtual void initialize(double initTime) = 0;


        /**
         * Handles a simulation step
         * @param simTime The current simulation time
         * @return Success flag
         */
        virtual bool step(double simTime) = 0;


        /**
         * Handles the termination
         * @return Success flag
         */
        virtual void terminate(double simTime) = 0;


    protected:



        /**
         * Resets the model timer
         * @param initTime Initial time
         */
        void initializeTimer(double initTime) {

            _last_time_step = initTime;

        }


        /**
        * Returns the time since the last time step was performed
        * @return Time passed
        */
        double sinceLastTimeStep(double simTime) const {

            return simTime - _last_time_step;

        }


        /**
         * Executes a time step and return the time step size. Attention: only run once per simulation step
         * @param simTime Simulation time
         * @return Time step size
         */
        double timeStep(double simTime) {

            double dt = sinceLastTimeStep(simTime);
            _last_time_step = simTime;

            return dt;

        }


    };



} // namespace sim::model;


#endif //SIMCORE_IPLUGIN_H
