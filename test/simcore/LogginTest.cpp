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
// Created by Jens Klimke on 2021-03-27.
//

#include <gtest/gtest.h>
#include <simcore/BasicSimulation.h>
#include <simcore/data/FileReporter.h>
#include <simcore/data/CSVReporter.h>


class LoggingTest : public ::testing::Test, public simcore::BasicSimulation {

public:

    std::string filename = "test.csv";

    double valueA{};
    double valueB{};

    LoggingTest() = default;
    ~LoggingTest() override = default;


    void SetUp() override {

        // create sub
        auto a = createGenericComponent();
        a->stepFnc = [this] (double t, double dt) {
            valueA = 2.0 * t;
            valueB = dt;
        };

    }


    void TearDown() override {

        // read file
        std::fstream file;

        // open file (and check)
        file.open(filename, std::ios::in);
        EXPECT_TRUE(file.is_open());

        // read line by line
        std::string line;

        // first line
        getline(file, line);
        EXPECT_EQ("time,timeStepSize,a,b", line);

        // first line
        getline(file, line);
        EXPECT_EQ("0,0,0,0", line);

        // second line
        getline(file, line);
        EXPECT_EQ("1,1,2,0.1", line);

        // last line
        while(getline(file, line));
        EXPECT_EQ("3,1,6,0.1", line);

        // close file
        file.close();

        // delete file
        std::remove(filename.c_str());

    }

};

TEST_F(LoggingTest, writeCSV) {

    // create reporter
    FileReporter<CSVReporter> rep{};
    rep.setFilename(filename);
    rep.setTimeStepSize(1.0);

    // set values
    rep.addValue("a", &valueA);
    rep.addValue("b", &valueB);

    // add reporter
    addComponent(&rep);


    // run
    createAndRun(3.0, 0.1);

}


TEST_F(LoggingTest, Logger) {

    // enable logging
    auto rep = enableLogging(filename, 1.0);

    // set values
    rep->addValue("a", &valueA);
    rep->addValue("b", &valueB);

    // run
    createAndRun(3.0, 0.1);

}