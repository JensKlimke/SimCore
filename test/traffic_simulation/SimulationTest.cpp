// Copyright (c) 2019 Institute for Automotive Engineering (ika), RWTH Aachen University. All rights reserved.
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

#include <gtest/gtest.h>
#include <TrafficSimulation/BasicSimulation.h>

class SimulationTest : public ::testing::Test, public BasicSimulation, public sim::IComponent {

public:

    unsigned int _counter = 0;

    double _initTime = INFINITY;
    double _simTime = INFINITY;
    double _termTime = INFINITY;

    SimulationTest() = default;
    ~SimulationTest() override = default;


    void initialize(double initTime) override {

        _initTime = initTime;

    }

    bool step(double simTime) override {

        bool valid = _counter == 1000;

        if(valid)
            _simTime = simTime;

        _counter++;

        return valid;

    }

    void terminate(double simTime) override {

        _termTime = simTime;

    }


};


TEST_F(SimulationTest, NonRealTime) {

    // create simulation
    create(10000.0, 0.01, false);

    // add vehicle as component
    addComponent(this);

    // run simulation
    run();

    EXPECT_NEAR(0.0, _initTime, 1e-6);
    EXPECT_NEAR(10.0, _simTime, 1e-6);
    EXPECT_NEAR(10000.0, _termTime, 1e-6);

}

TEST_F(SimulationTest, RealTime) {

    // create simulation
    create(2.0, 0.001, true);

    // add vehicle as component
    addComponent(this);

    // run simulation
    run();

    EXPECT_NEAR(0.0, _initTime, 1e-6);
    EXPECT_NEAR(1.0, _simTime, 1e-6);
    EXPECT_NEAR(2.0, _termTime, 1e-6);

}

