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

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-pragmas"
#pragma ide diagnostic ignored "modernize-use-auto"
#pragma ide diagnostic ignored "cert-err58-cpp"

#include <gtest/gtest.h>
#include <cucumber-cpp/autodetect.hpp>
#include <string>
#include "VehicleSimulation.h"
#include "helper.h"


using cucumber::ScenarioScope;


BEFORE() {}

AFTER() {}

GIVEN ("^the vehicle model with time step size ([\\.0-9]*) s$") {

    // get parameter
    REGEX_PARAM(double, tss);

    // get context
    ScenarioScope<VehicleSimulation> context;

    // set speed
    context->setTimeStepSize(tss);

}



GIVEN("^(the vehicle has )?a speed of (.*) m/s$") {

    // get parameter
    REGEX_PARAM(std::string, d); // dummy
    REGEX_PARAM(double, v);

    // get context
    ScenarioScope<VehicleSimulation> context;

    // set speed
    context->_state->velocity = v;

}


GIVEN ("^(the vehicle has )?a speed of ([\\.0-9]*) mph$") {

    // get parameter
    REGEX_PARAM(std::string, d); // dummy
    REGEX_PARAM(double, v);

    // get context
    ScenarioScope<VehicleSimulation> context;

    // set speed
    context->_state->velocity = v;

}


GIVEN ("^the vehicle's shifter is set to (\\w)$") {

    // get parameter
    REGEX_PARAM(std::string, pos);

    // get context
    ScenarioScope<VehicleSimulation> context;

    // set speed
    if (pos == "D") {
        context->setShifter(D);
        EXPECT_EQ(D, context->_state->shifterPosition);
    } else if (pos == "R") {
        context->setShifter(R);
        EXPECT_EQ(R, context->_state->shifterPosition);
    } else if (pos == "N") {
        context->setShifter(N);
        EXPECT_EQ(N, context->_state->shifterPosition);
    } else if (pos == "P") {
        context->setShifter(P);
        EXPECT_EQ(P, context->_state->shifterPosition);
    } else {
        FAIL() << "Shifter position " << pos << " not defined";
    }

}


GIVEN ("^the vehicle at position \\(([\\.0-9]*) m, ([\\.0-9]*) m\\)$") {

    // get parameters
    REGEX_PARAM(double, x);
    REGEX_PARAM(double, y);

    // get context and set position
    ScenarioScope<VehicleSimulation> context;
    context->setPosition(x, y);

}


GIVEN ("^(the vehicle has )?a yaw angle of ([\\.0-9]*) rad$") {

    // get parameter
    REGEX_PARAM(std::string, d);
    REGEX_PARAM(double, yaw);

    // get context and set yaw angle
    ScenarioScope<VehicleSimulation> context;
    context->_state->yawAngle = yaw;

}


GIVEN ("^(the vehicle has )?a yaw angle of ([\\.0-9]*) degrees") {

    // get parameter
    REGEX_PARAM(std::string, d);
    REGEX_PARAM(double, yaw);

    // get context and set yaw angle
    ScenarioScope<VehicleSimulation> context;
    context->_state->yawAngle = yaw * M_PI / 180.0;

}


GIVEN ("^the vehicle's external relative force is ([\\.0-9]*) N/kg") {

    // get parameter
    REGEX_PARAM(double, ext);

    // get context and set external force
    ScenarioScope<VehicleSimulation> context;
    context->_params->externalRelForce = ext;

}


WHEN ("^the simulation has been ran for ([\\.0-9]*) seconds$") {

    // get parameter
    REGEX_PARAM(double, t);

    // get context
    ScenarioScope<VehicleSimulation> context;

    // execute simulation for t seconds
    context->execute(t);

}


WHEN ("^the simulation is executed for ([\\.0-9]*) seconds$") {

    // get parameter
    REGEX_PARAM(double, t);

    // get context
    ScenarioScope<VehicleSimulation> context;

    // execute simulation for t seconds
    context->execute(t);

}


THEN ("^the position of the vehicle shall be x=([\\.0-9]*) m and y=([\\.0-9]*) m$") {

    // get parameters
    REGEX_PARAM(double, x);
    REGEX_PARAM(double, y);

    // get context
    ScenarioScope<VehicleSimulation> context;

    // check position
    EXPECT_NEAR(x, context->_state->xPosition, EPS_DISTANCE);
    EXPECT_NEAR(y, context->_state->yPosition, EPS_DISTANCE);

}


THEN ("^the vehicle's (\\w+) profile shall be shaped as (\\w+)\\((.*)\\)$") {

    // get parameters
    REGEX_PARAM(std::string, sig);
    REGEX_PARAM(std::string, fnc);
    REGEX_PARAM(std::string, paramStr);

    // get all doubles from string
    auto parameters = getParameters<double>(paramStr);

    // get context
    ScenarioScope<VehicleSimulation> context;
    const VehicleSimulation *sim = context.get();

    // get signal
    sim::storage::DataNode node;
    context->registerSignals(node);

    // signal
    double *d = node.get<double>(sig);

    if (fnc == "poly") {

        context->addPostCallback([sim, d, parameters]() {

            std::cout << "Checking..." << std::endl;

            // get time
            auto time = sim->getTimeStep().time;

            // calculate polynomial
            double sum = 0.0;
            for (const auto &p : parameters)
                sum = sum * time + p;

            EXPECT_NEAR(*d, sum, 1e-3);

        });

    }

}


#pragma clang diagnostic pop