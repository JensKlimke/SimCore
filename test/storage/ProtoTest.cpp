// Copyright (c) 2020 Jens Klimke.
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
// Created by Jens Klimke on 2020-10-04.
// Contributors:
//

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-pragmas"
#pragma ide diagnostic ignored "cert-err58-cpp"

#include <gtest/gtest.h>
#include <simcore/storage/Manager.h>
#include <simcore/testing/SimulationTest.h>


class SimDump : public sim::Model {

protected:

    void initialize(double t) override {

    }

    void step(double t, double dt) override {


    }

    void terminate(double t) override {

    }

};


class ProtoTest : public ::testing::Test, public ::sim::testing::SimulationTest<SimDump> {

public:

    ProtoTest() = default;
    ~ProtoTest() override = default;

};


TEST_F(ProtoTest, Dump) {

    sim::protobuf::Loop loop{};

    // TODO: create sim

    // save loop
    // sim::storage::Manager::saveLoop(loop, this->_loop.get());

}

#pragma clang diagnostic pop