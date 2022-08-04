//
// Copyright (c) 2019-2022 Jens Klimke <jens.klimke@rwth-aachen.de>
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

#include <gtest/gtest.h>
#include <simcore/BasicSimulation.h>
#include <simcore/data/DataReporter.h>


class DataReporterTest : public ::testing::Test, public DataReporter {

public:

    simcore::BasicSimulation simulation{};

    double valueA{};
    double valueB{};

    DataReporterTest() = default;
    ~DataReporterTest() override = default;

    void SetUp() override {

        // create sub
        auto a = simulation.createGenericComponent();
        a->stepFnc = [this] (double t, double dt) {
            valueA = 2.0 * t;
            valueB = dt;
        };

        // create reporter
        DataReporter::setTimeStepSize(1.0);

        // set values
        addValue("a", &valueA);
        addValue("b", &valueB);

        // add reporter
        simulation.addComponent(this);

        // run
        simulation.createAndRun(3.0, 0.1);

    }

};

TEST_F(DataReporterTest, WriteData) {

    // check elements
    EXPECT_NEAR(0.0, _data[0].at("time"), 1e-9);
    EXPECT_NEAR(0.0, _data[0].at("timeStepSize"), 1e-9);
    EXPECT_NEAR(0.0, _data[0].at("a"), 1e-9);
    EXPECT_NEAR(0.0, _data[0].at("b"), 1e-9);

    EXPECT_NEAR(1.0, _data[1].at("time"), 1e-9);
    EXPECT_NEAR(1.0, _data[1].at("timeStepSize"), 1e-9);
    EXPECT_NEAR(2.0, _data[1].at("a"), 1e-9);
    EXPECT_NEAR(0.1, _data[1].at("b"), 1e-9);

    EXPECT_NEAR(2.0, _data[2].at("time"), 1e-9);
    EXPECT_NEAR(1.0, _data[2].at("timeStepSize"), 1e-9);
    EXPECT_NEAR(4.0, _data[2].at("a"), 1e-9);
    EXPECT_NEAR(0.1, _data[2].at("b"), 1e-9);

    EXPECT_NEAR(3.0, _data[3].at("time"), 1e-9);
    EXPECT_NEAR(1.0, _data[3].at("timeStepSize"), 1e-9);
    EXPECT_NEAR(6.0, _data[3].at("a"), 1e-9);
    EXPECT_NEAR(0.1, _data[3].at("b"), 1e-9);

}

TEST_F(DataReporterTest, all) {

    // filter
    auto data = all();

    // check size
    EXPECT_EQ(4, data.size());

    // check elements
    EXPECT_NEAR(0.0, data[0].at("time"), 1e-9);
    EXPECT_NEAR(0.0, data[0].at("timeStepSize"), 1e-9);
    EXPECT_NEAR(0.0, data[0].at("a"), 1e-9);
    EXPECT_NEAR(0.0, data[0].at("b"), 1e-9);

    EXPECT_NEAR(1.0, data[1].at("time"), 1e-9);
    EXPECT_NEAR(1.0, data[1].at("timeStepSize"), 1e-9);
    EXPECT_NEAR(2.0, data[1].at("a"), 1e-9);
    EXPECT_NEAR(0.1, data[1].at("b"), 1e-9);

    EXPECT_NEAR(2.0, data[2].at("time"), 1e-9);
    EXPECT_NEAR(1.0, data[2].at("timeStepSize"), 1e-9);
    EXPECT_NEAR(4.0, data[2].at("a"), 1e-9);
    EXPECT_NEAR(0.1, data[2].at("b"), 1e-9);

    EXPECT_NEAR(3.0, data[3].at("time"), 1e-9);
    EXPECT_NEAR(1.0, data[3].at("timeStepSize"), 1e-9);
    EXPECT_NEAR(6.0, data[3].at("a"), 1e-9);
    EXPECT_NEAR(0.1, data[3].at("b"), 1e-9);

}

TEST_F(DataReporterTest, FilterData) {

    // filter
    auto data = filter([] (const DataMap &element) {
        return (element.at("time") < 2.5 && element.at("a") > 1.0);
    });

    // check size
    EXPECT_EQ(2, data.size());

    // check elements
    EXPECT_NEAR(1.0, data[0].at("time"), 1e-9);
    EXPECT_NEAR(1.0, data[0].at("timeStepSize"), 1e-9);
    EXPECT_NEAR(2.0, data[0].at("a"), 1e-9);
    EXPECT_NEAR(0.1, data[0].at("b"), 1e-9);

    EXPECT_NEAR(2.0, data[1].at("time"), 1e-9);
    EXPECT_NEAR(1.0, data[1].at("timeStepSize"), 1e-9);
    EXPECT_NEAR(4.0, data[1].at("a"), 1e-9);
    EXPECT_NEAR(0.1, data[1].at("b"), 1e-9);

}

TEST_F(DataReporterTest, LastElement) {

    // filter
    auto data = last();

    // check size
    EXPECT_EQ(1, data.size());

    // check elements
    EXPECT_NEAR(3.0, data[0].at("time"), 1e-9);
    EXPECT_NEAR(1.0, data[0].at("timeStepSize"), 1e-9);
    EXPECT_NEAR(6.0, data[0].at("a"), 1e-9);
    EXPECT_NEAR(0.1, data[0].at("b"), 1e-9);

}
