//
// Created by Jens Klimke on 2019-03-22.
//

#ifndef PRIMARY_CONTROLLER_H
#define PRIMARY_CONTROLLER_H


#include <cmath>
#include <algorithm>

namespace agent_model {

    class PrimaryController {

    public:

        struct Input {
            double *value;
            double *target;
        };

        struct State {
            double in;
            double u;
            double *y;
        };

        struct Parameters {
            double P = 0.0;
            double I = 0.0;
            double D = 0.0;
            double range[2] = {-1.0, 1.0};
        };


    private:

        bool _reset = false;

        Input _input{};
        State _state{};
        Parameters _parameters{};


    public:


        /**
         * Perform a controller step with the given time step size
         * @param dt Time step size
         */
        void step(double dt);


        /**
         * Reset the controller states (except the output value)
         */
        void reset();


        /**
         * Set the controller variables
         * @param value The actual value
         * @param target The desired value
         * @param output The output value
         */
        void setVariables(double *value, double *target, double *output);


        /**
         * Sets the parameters
         * @param k_p Proportional parameter
         * @param k_i Integral parameter
         * @param k_d Derivative parameter
         */
        void setParamters(double k_p, double k_i, double k_d);

    };

}

#endif // PRIMARY_CONTROLLER_H
