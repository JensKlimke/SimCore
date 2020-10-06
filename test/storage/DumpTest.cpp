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
// Created by Jens Klimke on 2020-08-06.
//

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-pragmas"
#pragma ide diagnostic ignored "cert-err58-cpp"


#include <gtest/gtest.h>
#include <simcore/Model.h>
#include <simcore/testing/SimulationTest.h>
#include <simcore/storage/Manager.h>

class Stepper : public sim::Model {
protected:

    void initialize(double t) override {

    }

    void step(double t, double dt) override {

    }

    void terminate(double t) override {

    }

};

class DumpTest : public ::testing::Test, public sim::testing::SimulationTest<Stepper> {

protected:

    void SetUp() override {
    }

    double value = 10.0;

};


TEST_F(DumpTest, ToJSON) {

    // create simulation
    create(10.0, 0.01, 1.0, 20.0);

    // json
    nlohmann::json loop{}, timer{}, stop{};

    // to json
    to_json(loop, *this->_loop);

    // basic timer
    if (this->_basicTimer)
        to_json(timer, *this->_basicTimer);

    // rt timer
    if (this->_rtTimer)
        to_json(timer, *this->_rtTimer);

    // stop timer
    to_json(stop, *this->_stopTimer);

    // check loop
    EXPECT_TRUE(loop["stopFlag"]);
    EXPECT_EQ(2, loop["status"]);
    EXPECT_EQ(1, loop["components"][0]);
    EXPECT_EQ(2, loop["components"][1]);
    EXPECT_EQ(3, loop["stopConditions"][0]);

    // check timer
    EXPECT_DOUBLE_EQ(20.0, timer["acceleration"]);
    EXPECT_DOUBLE_EQ(0.0, timer["refTime"]);
    EXPECT_DOUBLE_EQ(0.0, timer["startRefTime"]);
    EXPECT_DOUBLE_EQ(1.0, timer["startTime"]);
    EXPECT_DOUBLE_EQ(0.01, timer["stepSize"]);
    EXPECT_DOUBLE_EQ(0.0, timer["time"]);
    EXPECT_EQ(0, timer["steps"]);

    // check stop timer
    EXPECT_EQ(0, stop["code"]);
    EXPECT_DOUBLE_EQ(10.0, stop["stopTime"]);

}


#pragma clang diagnostic pop