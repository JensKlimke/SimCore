//
// Copyright (c) 2019-2020 Jens Klimke <jens.klimke@rwth-aachen.de>
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
// Created by Jens Klimke on 2019-04-01
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

        bool inLow = true;
        if(lower.isInBounds(x))
            inLow = lower.interpolate(x) <= y;

        bool inUp = true;
        if(upper.isInBounds(x))
            inUp = y <= upper.interpolate(x);

        return inLow && inUp;

    }



    /**
     * Checks if the curve is set by at least two points
     * @return true if more tha one point is set
     */
    bool isSet() const {

        return lower.isSet() && upper.isSet();

    }


    /**
     * Returns the lower and upper value of the tube
     * @param x Position to be evaluated
     * @return pair of lower and upper value
     */
    std::pair<double, double> getValues(double x) const {

        return {lower.interpolate(x), upper.interpolate(x)};

    }

};

#endif //SIMCORE_SIGNALTUBE_H
