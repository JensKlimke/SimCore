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

TEST_F(DumpTest, DataManager) {

    using namespace sim;

    DataManager::registerOwner(&this->_loop, "loop");
    DataManager::registerOwner(&this->_basicTimer, "timer");

    for (auto &it : DataManager::index) {

        // get owner and entry
        auto owner = it.first;
        auto entry = it.second;

        // check entry name
        if (!entry.name.empty())
            std::cout << "\"" << entry.name << "\": " << std::endl;
        else
            std::cout << "<empty>:" << std::endl;

        for (auto &ite : entry.signals) {

            // get signal and name
            auto sig = ite.second;
            auto sigName = ite.first;

            // get json
            nlohmann::json j{};
            sig->toJson(j);

            // output
            std::cout << "- \"" << sigName << "\":" << std::endl;
            std::cout << "   " << j << std::endl;

        }

    }

}


#pragma clang diagnostic pop