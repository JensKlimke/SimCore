// Copyright (c) 2021 Jens Klimke.
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
// Created by Jens Klimke on $date.get('yyyy-M-d').
// Contributors:
//


#ifndef SIMCORE_SPLINE_H
#define SIMCORE_SPLINE_H

#include "Polynomial.h"

namespace simbasic {


    struct SplineSegment {
        double end = INFINITY;
        Polynomial polynomial;
    };

    typedef std::vector<SplineSegment> SplineDefinition;

    struct Spline : public SplineDefinition {
        Spline() = default;
        double operator() (double x) const;
    };

    /**
     * Calculates the spline at the given sample points
     * @param x Sample point
     * @return The evaluation of the polynomial
     */
    double Spline::operator()(double x) const {

        // iterate over segments
        for(const auto &s : *this) {

            // check if x is not in this segment
            if(x > s.end)
                continue;

            // evaluate polynomial
            return s.polynomial(x);

        }

        throw std::invalid_argument("Could not find valid element");

    }

}

#endif //SIMCORE_SPLINE_H
