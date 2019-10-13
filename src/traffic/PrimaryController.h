//
// Created by Jens Klimke on 2019-03-22.
//

#ifndef PRIMARY_CONTROLLER_H
#define PRIMARY_CONTROLLER_H


#include <cmath>
#include <algorithm>
#include <core/Model.h>


class PrimaryController : public sim::Model {

public:

    struct Input {
        double *value;
        double *target;
        double *direct;
    };

    struct State {
        double in;
        double u;
        double *y;
    };

    struct Parameters {
        double k_P = 0.0;
        double k_I = 0.0;
        double k_D = 0.0;
        double range[2] = {-1.0, 1.0};
        double d_P = 1.0;
        double dyMax = INFINITY;
    };


private:

    bool _reset = false;

    Input _input{};
    State _state{};
    Parameters _parameters{};


public:

    /**
     * Initialization
     * @param initTime Initialize time
     */
    void initialize(double initTime) override;


    /**
     * Perform a controller step with the given time step size
     * @param dt Time step size
     */
    bool step(double simTime) override;


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


    /**
     * Returns the data of the model
     * @param context Context of the data
     * @return
     */
    std::vector<DataEntry> getData(Context context) const override;


    // generate accessor methods
    PARAM_ACCESS(Parameters, _parameters);
    INPUT_ACCESS(Input, _input);
    STATE_ACCESS(State, _state);


};

#endif // PRIMARY_CONTROLLER_H
