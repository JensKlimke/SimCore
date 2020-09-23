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
// Created by Jens Klimke on 2020-09-23.
//

#include <cucumber-cpp/autodetect.hpp>
#include <Simulation.h>

#define EPS_VELOCITY 1e-3

using cucumber::ScenarioScope;

struct SimCtx {
    Simulation sim;
    double result;
};


GIVEN("^I have setup an initial speed (\\d+) for the vehicle$") {

    REGEX_PARAM(double, v);
    ScenarioScope<SimCtx> context;

    context->sim.setInitialSpeed(v / 3.6);
}


GIVEN("^I have setup a desired speed (\\d+) for the driver model$") {

    REGEX_PARAM(double, v);
    ScenarioScope<SimCtx> context;

    context->sim.setDesiredSpeed(v / 3.6);
}


WHEN("^I run the simulation for (\\d+) seconds$") {

    REGEX_PARAM(double, t);
    ScenarioScope<SimCtx> context;

    // set simulation time and execute
    context->sim.setSimulationTime(t);
    context->result = context->sim.execute();

}


THEN("^the final speed of the vehicle should be (.*)$") {

    REGEX_PARAM(double, expected);
    ScenarioScope<SimCtx> context;

    EXPECT_NEAR(expected / 3.6, context->result, EPS_VELOCITY);

}

