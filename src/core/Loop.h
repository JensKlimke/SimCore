//
// Created by Jens Klimke on 2019-03-19.
//

#ifndef SIMCORE_LOOP_H
#define SIMCORE_LOOP_H

#include "Exceptions.h"
#include <string>
#include <vector>
#include <unordered_map>


namespace sim {

    // pre-declare classes for usage in Loop
    class IComponent;
    class IStopCondition;
    class ITimer;


    class Loop {

    public:

        enum class Status { INITIALIZED, RUNNING, STOPPED };


    private:

        Status _status = Status::STOPPED;
        bool   _stop   = true;

        // TODO: model names
        std::vector<IComponent*> _models{};
        std::vector<IStopCondition*> _stop_conditions{};

        ITimer *_timer = nullptr;


    public:

        /**
         * Default constructor
         */
        Loop() = default;


        /**
         * Default deconstructor
         */
        virtual ~Loop() = default;


        /**
         * Sets the timer of the loop
         * @param timer Timer to be set
         */
        void setTimer(ITimer *timer);


        /**
         * Adds an stop condition to the loop
         * @param stop Stop condition
         */
        void addStopCondition(IStopCondition *stop);


        /**
         * Adds an model to the loop
         * @param name Name of the model
         * @param model Model to be set
         */
        void addModel(sim::IComponent *model);


        /**
         * Run simulation
         */
        void run();


        /**
         * Abort the running simulation
         */
        void stop();


        /**
         * Returns the status of the loops
         * @return Status
         */
        Status getStatus() const;



    private:


        /**
         * Initialize simulation
         */
        void initialize();


        /**
         * Terminate simulation
         */
        void terminate();


    };


}; // namespace ::sim


#endif //SIMCORE_LOOP_H
