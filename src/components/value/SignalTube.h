//
// Created by Jens Klimke on 2019-04-01.
//

#ifndef SIMCORE_SIGNALTUBE_H
#define SIMCORE_SIGNALTUBE_H

#include "SignalCurve.h"

class SignalTube {

    SignalCurve upper{};
    SignalCurve lower{};

public:


    SignalTube() = default;


    /**
     * Define the upper curve
     * @param x x-values
     * @param y y-values
     */
    void defineUpper(std::vector<double> &&x, std::vector<double> &&y) {

        upper.define(std::move(x), std::move(y));

    }


    /**
     * Define the lower curve
     * @param x x-values
     * @param y y-values
     */
    void defineLower(std::vector<double> &&x, std::vector<double> &&y) {

        lower.define(std::move(x), std::move(y));

    }


    /**
     * Returns the center value between the upper and the lower curve at the given position
     * @param x Position
     * @return The center value at x
     */
    double center(double x) const {

        return 0.5 * (lower.interpolate(x) + upper.interpolate(x));

    }


    /**
     * Checks if the value is within the band
     * @param x x-value
     * @param y y-value
     * @return true when the value is within the band
     */
    bool in(double x, double y) const {

        return lower.interpolate(x) <= y && y <= upper.interpolate(x);

    }



    /**
     * Checks if the curve is set by at least two points
     * @return true if more tha one point is set
     */
    bool isSet() const {

        return lower.isSet() && upper.isSet();

    }


};

#endif //SIMCORE_SIGNALTUBE_H
