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

#include <gtest/gtest.h>
#include <simcore/traffic/Unit.h>

class TrafficUnit : protected sim::traffic::Unit {

public:

    TrafficUnit() = default;

    explicit TrafficUnit(sim::id_type &&id) : Unit(id) {}

    [[nodiscard]] const sim::id_type &getID() const {
        return this->_id;
    }

    [[nodiscard]] const sim::traffic::Unit::Parameters &getParameters() const {
        return this->parameters;
    }

    [[nodiscard]] const sim::traffic::Unit::State &getState() const {
        return this->state;
    }

    [[nodiscard]] sim::traffic::Unit::Parameters &getParameters() {
        return this->parameters;
    }

    [[nodiscard]] sim::traffic::Unit::State &getState() {
        return this->state;
    }


};


TEST(TrafficUnitTest, CreateUnit) {

    using namespace sim::traffic;

    TrafficUnit unit1;
    TrafficUnit unit2;
    TrafficUnit unit3("3");

    EXPECT_EQ("", unit1.getID());
    EXPECT_EQ("", unit2.getID());
    EXPECT_EQ("3", unit3.getID());

}

TEST(TrafficUnitTest, SetData) {

    using namespace sim::traffic;

    // create unit
    TrafficUnit unit;

    // get a const pointer
    const TrafficUnit* ptr = &unit;

    // check default parameters
    EXPECT_DOUBLE_EQ(5.0, ptr->getParameters().size.x);
    EXPECT_DOUBLE_EQ(2.2, ptr->getParameters().size.y);
    EXPECT_DOUBLE_EQ(1.5, ptr->getParameters().size.z);
    EXPECT_DOUBLE_EQ(0.0, ptr->getParameters().reference.x);
    EXPECT_DOUBLE_EQ(0.0, ptr->getParameters().reference.y);
    EXPECT_DOUBLE_EQ(0.0, ptr->getParameters().reference.z);

    // check default state
    EXPECT_DOUBLE_EQ(0.0, ptr->getState().position.x);
    EXPECT_DOUBLE_EQ(0.0, ptr->getState().position.y);
    EXPECT_DOUBLE_EQ(0.0, ptr->getState().position.z);
    EXPECT_DOUBLE_EQ(0.0, ptr->getState().velocity);
    EXPECT_DOUBLE_EQ(0.0, ptr->getState().acceleration);
    EXPECT_DOUBLE_EQ(0.0, ptr->getState().yawAngle);
    EXPECT_DOUBLE_EQ(0.0, ptr->getState().yawRate);

    // set parameters
    unit.getParameters().size = {0.0, 0.0, 0.0};
    unit.getParameters().reference = {-1.5, 0.0, -0.75};

    // set states
    unit.getState().position = {100.0, 200.0, 300.0};

    // check parameters
    EXPECT_DOUBLE_EQ( 0.0,  ptr->getParameters().size.x);
    EXPECT_DOUBLE_EQ( 0.0,  ptr->getParameters().size.y);
    EXPECT_DOUBLE_EQ( 0.0,  ptr->getParameters().size.z);
    EXPECT_DOUBLE_EQ(-1.5,  ptr->getParameters().reference.x);
    EXPECT_DOUBLE_EQ( 0.0,  ptr->getParameters().reference.y);
    EXPECT_DOUBLE_EQ(-0.75, ptr->getParameters().reference.z);

    // check state
    EXPECT_DOUBLE_EQ(100.0, ptr->getState().position.x);
    EXPECT_DOUBLE_EQ(200.0, ptr->getState().position.y);
    EXPECT_DOUBLE_EQ(300.0, ptr->getState().position.z);

}
