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


#ifndef SIMCORE_POLYNOMIAL_H
#define SIMCORE_POLYNOMIAL_H

#include <cmath>
#include <vector>
#include <float.h>

namespace simbasic {

    /*!< Definition of a polynomial parameter */
    struct PolyParam {
        long order;
        double parameter;
    };

    /*!< Definition of the polynomial */
    typedef std::vector<PolyParam> PolyDefinition;

    /*!< Polynomial class */
    struct Polynomial : public PolyDefinition {
        Polynomial() = default;
        explicit Polynomial(std::vector<double> &&parameters);
        explicit Polynomial(double constant);
        explicit Polynomial(long order, double parameter);
        double operator()(double x) const;
        void add(long order, double parameter);
    };


    /**
     * Creates a constant polynomial (order 0)
     * @param constant Constant value
     */
    Polynomial::Polynomial(double constant) {

        // add a single constant value
        emplace_back(PolyParam{0, constant});

    }


    /**
     * Creates a polynomial with the given order and the given parameter
     * @param order Order of the element
     * @param parameter Parameter of the element
     */
    Polynomial::Polynomial(long order, double parameter) {

        // add a single parameter
        add(order, parameter);

    }


    /**
     * Constructor to create polynomial from 0..n order by given parameters
     * @param parameters Parameters to be set
     */
    Polynomial::Polynomial(std::vector<double> &&parameters) {

        // init counter
        long i = -1;

        // iterate over parameters and set order from 0..n
        for(auto &p : parameters) {

            // increment
            ++i;

            // ignore when almost zero
            if(abs(p) <= DBL_EPSILON)
                continue;

            // add parameter
            this->emplace_back(PolyParam{i, p});

        }

    }


    /**
     * Calculates the polynomial at the given sample points
     * @param x Sample point
     * @return The evaluation of the polynomial
     */
    double Polynomial::operator()(double x) const {

        // init result
        double result = 0.0;

        // iterate over elements
        for (const auto &p : *this)
            result += p.parameter * std::pow(x, (double) p.order);

        // return result
        return result;

    }


    /**
     * Adds an element to the polynomial
     * @param order Order of the element
     * @param parameter Parameter of the element
     */
    void Polynomial::add(long order, double parameter) {

        // add a single parameter
        emplace_back(PolyParam{order, parameter});

    }

}

#endif //SIMCORE_POLYNOMIAL_H
