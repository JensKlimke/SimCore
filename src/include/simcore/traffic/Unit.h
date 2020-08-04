// Copyright (c) 2020 Jens Klimke.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
// Created by Jens Klimke on 2020-03-20.
//


#ifndef SIMCORE_UNIT_H
#define SIMCORE_UNIT_H

#include <map>
#include "sim_types.h"

namespace sim::traffic {


    class Unit {


    public:

        /** A class to store the state of a dynamic object. */
        struct DynamicState {
            Vector3 position = {0.0, 0.0, 0.0}; // The actual position (in *m*)
            double velocity = 0.0;              // The actual velocity (in *m/s*)
            double acceleration = 0.0;          // The actual acceleration (in *m/s^2*)
            double yawAngle = 0.0;              // The actual yaw angle (in *rad*)
            double yawRate = 0.0;               // The actual yaw rate (in *rad/s*)
            double wheelAngle = 0.0;            // The actual wheel steer angle (in *rad*)
        };


        /** A class to store the parameters. */
        struct DynamicParameters {
            double wheelBase = 3.0;                 // The wheel base (distance between axles) (in *m*)
            Vector3 size = {5.0, 2.2, 1.5};         // The size of the vehicle (in *m*)
            Vector3 reference = {0.5, 0.5, 1.1};    // The reference point related to the center point (e.g. the driver's position in the vehicle) (in *m*)
        };


        /**
         * @brief Construct unit with given ID
         * Resets the auto counter to a higher value if ID is higher than counter
         * Attention: Overwrites an exiting unit in index if ID already in use.
         * @param id ID to be set
         */
        explicit Unit(id_type id) : _id(id) {

            // store to index
            __index[id] = this;

            // reset counter
            if(id > __id_counter)
                __id_counter = id;

        }


        /**
         * Creates an unit with a auto-generated ID
         */
        Unit() : Unit(++__id_counter) {};


        /**
         * @brief Destructor of unit
         * Removes the unit from the index
         */
        virtual ~Unit() {
            __index.erase(_id);
        };


        /**
         * Returns the ID of the unit
         * @return ID of the unit
         */
        id_type getID() const {
            return _id;
        }


        /**
         * Returns the parameters of the unit
         * @return Parameters of the unit
         */
        const DynamicParameters &getParameters() const {
            return parameters;
        }

        /**
         * Returns a reference to the parameters of the unit
         * @return Parameters of the unit
         */
        DynamicParameters &getParameters() {
            return parameters;
        }

        /**
         * Returns the state of the unit
         * @return State of the unit
         */
        const DynamicState &getState() const {
            return state;
        }

        /**
         * Returns a reference to the state of the unit
         * @return State of the unit
         */
        DynamicState getState() {
            return state;
        }


    private:

        static id_type __id_counter;
        static std::map<id_type, Unit *> __index;

        id_type _id;

    protected:

        DynamicParameters parameters{};
        DynamicState state{};


    };

}


#endif //SIMCORE_UNIT_H
