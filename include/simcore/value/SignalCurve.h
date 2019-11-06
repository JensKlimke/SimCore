//
// Copyright (c) 2019 Jens Klimke <jens.klimke@rwth-aachen.de>. All rights reserved.
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
// Created by Jens Klimke on 2019-03-28
//

#ifndef SIMCORE_SIGNALPROGRESS_H
#define SIMCORE_SIGNALPROGRESS_H

#ifndef EPS_DISTANCE
#define EPS_DISTANCE 1e-9
#endif

#include <vector>
#include <stdexcept>
#include <cmath>

class SignalCurve {

    std::vector<double> _x;
    std::vector<double> _y;


public:


    struct Position {
        double ds0;
        double v0;
        double ds1;
        double v1;
    };


    SignalCurve() = default;
    ~SignalCurve() = default;

    /**
     * Define the curve
     * @param x x-values
     * @param y y-values
     */
    void define(std::vector<double> &&x, std::vector<double> &&y) {

        if(_x.size() != _y.size())
            throw std::invalid_argument("sizes must be equal");

        _x = std::move(x);
        _y = std::move(y);

    }


    /**
     * Check if the value is within defined boundaries
     * @param x Value to be checked
     * @return Flag if the value is in bounds
     */
    bool isInBounds(double x) const {

        return x >= _x.front() && x <= _x.back();

    }


    /**
     * Calculates the y-value at the given x point by linear interpolation
     * @param x x-value
     * @return y-value
     */
    double interpolate(double x) const {

        // check if x is out of bounds
        if(x < _x.front() - EPS_DISTANCE || x > _x.back() + EPS_DISTANCE)
            throw std::invalid_argument("x out of bounds");

        auto w = where(x);
        return w.v0 - w.ds0 * (w.v1 - w.v0) / (w.ds1 - w.ds0);

    }


    /**
     * Returns the previous value
     * @param x x-value
     * @return y-value
     */
    double previous(double x) const {

        // check if x is out of bounds
        if(x < _x.front() - EPS_DISTANCE)
            throw std::invalid_argument("x out of bounds");

        auto w = where(x);

        if(w.ds1 <= 0.0)
            return w.v1;
        else
            return w.v0;

    }


    /**
     * Returns the previous value
     * @param x x-value
     * @return y-value
     */
    double next(double x) const {

        // check if x is out of bounds
        if(x > _x.back() + EPS_DISTANCE)
            throw std::invalid_argument("x out of bounds");

        auto w = where(x);

        if(w.ds0 > 0.0)
            return w.v0;
        else
            return w.v1;

    }


    /**
     * Returns the previous and the next value at the given point. If the value is out of bounds, the closest
     * values are taken. The distances are given relative to the given position
     * @param x x-Position
     * @return Structure of positions and values
     */
    Position where(double x) const {

        auto n = _x.size();

        // check boundaries
        if(x <= _x[0])
            return {_x[0] - x, _y[0], _x[1] - x, _y[1]};

        else if(x >= _x[n - 1])
            return {_x[n - 2] - x, _y[n - 2], _x[n - 1] - x, _y[n - 1]};


        unsigned int i;
        for(i = 1; i < _x.size(); ++i) {

            // check for strictly monotonous
            if(_x[i - 1] >= _x[i])
                throw std::runtime_error("interpolation only possible with strictly monotonous data");

            // interpolate if in bounds
            if (_x[i] > x)
                return {_x[i - 1] - x, _y[i - 1], _x[i] - x, _y[i]};

        }

        return {};

    }


    /**
     * Calculates if the data point hit the signal curve
     * @param x x-value
     * @param y y-value
     * @param eps Tolerance (in y-direction, half band width)
     * @return Flag
     */
    bool hit(double x, double y, double eps = EPS_DISTANCE) const {

        return fabs(interpolate(x) - y) < eps;

    }


    /**
     * Checks if the curve is set by at least two points
     * @return true if more tha one point is set
     */
    bool isSet() const {

        return _x.size() > 1;

    }

};


#endif //SIMCORE_SIGNALPROGRESS_H
