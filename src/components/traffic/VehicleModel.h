//
// Created by Jens Klimke on 2019-03-17.
//

#ifndef SIMCORE_VEHICLEMODEL_H
#define SIMCORE_VEHICLEMODEL_H


#include <core/Model.h>
#include <cmath>


class VehicleModel : public ::sim::Model {

public:

    struct Parameters {
        double steerTransmission  = 0.474;
        double wheelBase          = 2.7;
        double cwA                = 0.6;
        double mass               = 1.8e3;
        double power              = 1.2e5;
        double rollCoefficient[3] = {9.91e-3, 1.95e-5, 1.76e-9};
        double size[2]            = {5.0, 2.2};
        double driverPos[2]       = {0.5, 0.5};
    };


    struct Input {
        double steer = 0.0;
        double pedal = 0.0;
        double slope = 0.0;
    };


    struct State {
        double s;
        double ds;
        double psi;
        double dPsi;
        double xy[2];
        double v;
        double a;
    };


private:

    Parameters _param;
    State      _state{};
    Input      _input;

public:

    VehicleModel() = default;

    void initialize(double initTime) override;

    bool step(double simTime) override;

    void terminate(double simTime) override;

    // default accessors
    PARAM_ACCESS(_param)
    INPUT_ACCESS(_input)
    STATE_ACCESS(_state)


    std::vector<DataEntry> getData(Context context) override {

        std::vector<DataEntry> ret;
        ret.reserve(2);

        switch(context) {
            case Context::PARAMETER:
                /*
                    double steerTransmission  = 0.474;
                    double wheelBase          = 2.7;
                    double cwA                = 0.6;
                    double mass               = 1.8e3;
                    double power              = 1.2e5;
                    double rollCoefficient[3] = {9.91e-3, 1.95e-5, 1.76e-9};
                    double size[2]            = {5.0, 2.2};
                    double driverPos[2]       = {0.5, 0.5};
                 */
                // ADD(ret, steerTransmission, _param);
                break;
            case Context::INPUT:
                break;
            case Context::STATE:
                break;
            default:
                break;
        }

        return ret;

    }

};


#endif //SIMCORE_VEHICLEMODEL_H
