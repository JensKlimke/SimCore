// Copyright (c) 2020 Jens Klimke (jens.klimke@rwth-aachen.de). All rights reserved.
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
// Created by Jens Klimke on $YEAR-$MONTH-23.
//


#ifndef SIMCORE_SCENARIO_TEST_SIMULATION_H
#define SIMCORE_SCENARIO_TEST_SIMULATION_H


class Simulation {

private:

    double _init_speed;
    double _desired_speed;

public:

    /**
     * Sets the initial speed of the ego vehicle
     * @param v Speed to be set in kph
     */
    void setInitialSpeed(double v);


    /**
     * Sets the desired speed of the ego driver
     * @param v Speed to be set in kph
     */
    void setDesiredSpeed(double v);


    /**
     * Runs the simulation
     */
    double run();

};


#endif // SIMCORE_SCENARIO_TEST_SIMULATION_H
