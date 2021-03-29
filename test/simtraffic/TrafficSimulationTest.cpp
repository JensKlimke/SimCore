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
// Created by Jens Klimke on 2020-03-07.
//

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-pragmas"
#pragma ide diagnostic ignored "cert-err58-cpp"

#include <gtest/gtest.h>
#include <simcore/BasicSimulation.h>

class TrafficSimulationTest : public ::testing::Test, public simcore::BasicSimulation, public simcore::IComponent {

public:

    unsigned int _counter = 0;

    double _initTime = INFINITY;
    double _simTime = INFINITY;
    double _termTime = INFINITY;

    TrafficSimulationTest() = default;
    ~TrafficSimulationTest() override = default;


    void initialize(double initTime) override {

        _initTime = initTime;

    }

    void step(double simTime) override {

        bool valid = _counter == 1000;

        if(valid)
            _simTime = simTime;

        _counter++;

    }

    void terminate(double simTime) override {

        _termTime = simTime;

    }


};


TEST_F(TrafficSimulationTest, NonRealTime) {

    // create simulation
    create(10000.0, 0.01, false);

    // add this as component
    addComponent(this);

    // run simulation
    Loop::run();

    EXPECT_NEAR(0.0, _initTime, 1e-6);
    EXPECT_NEAR(10.0, _simTime, 1e-6);
    EXPECT_NEAR(10000.0, _termTime, 1e-6);

}

TEST_F(TrafficSimulationTest, RealTime) {

    // create simulation
    create(2.0, 0.001, true);

    // add this as component
    addComponent(this);

    // run simulation
    Loop::run();

    EXPECT_NEAR(0.0, _initTime, 1e-6);
    EXPECT_NEAR(1.0, _simTime, 1e-6);
    EXPECT_NEAR(2.0, _termTime, 1e-6);

    // TODO: check output

}

#pragma clang diagnostic pop