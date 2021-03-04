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
#include <simtraffic/Unit.h>
#include <simtraffic/UnitData.h>
#include <nlohmann/json.hpp>

TEST(TrafficUnitTest, UnitExport) {

    Unit unit{};
    nlohmann::json j = unit;

    // std::cout << j.dump() << std::endl;
    EXPECT_EQ("{\"id\":\"unit-0\",\"parameters\":{\"driverPosition\":{\"x\":0.5,\"y\":0.5,\"z\":1.1},\"size\":{\"x\":5.0,\"y\":2.2,\"z\":1.5},\"wheelBase\":3.0},\"state\":{\"acceleration\":0.0,\"position\":{\"x\":0.0,\"y\":0.0,\"z\":0.0},\"velocity\":0.0,\"wheelAngle\":0.0,\"yawAngle\":0.0,\"yawRate\":0.0}}", j.dump());

    nlohmann::json ji = {
        {"id", "unit-0"},
        {"state", {
              {"position", {
                   {"x", 10.0},
                   {"y",  0.5},
                   {"z",  0.0}
              }},
              {"velocity", 10.0},
              {"wheelAngle", 0.5},
              {"yawAngle", 0.2}
        }}
    };

    Unit unit2{};
    nlohmann::json j2 = unit;
    j2.merge_patch(ji);

    unit2 = j2;
    EXPECT_DOUBLE_EQ(10.0, unit2.getState()->velocity);
    EXPECT_DOUBLE_EQ(0.5, unit2.getState()->wheelAngle);

}

