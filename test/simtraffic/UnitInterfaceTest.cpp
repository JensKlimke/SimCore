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
#include <simtraffic/Unit.h>
#include <simtraffic/UnitData.h>
#include <nlohmann/json.hpp>

TEST(TrafficUnitTest, UnitExport) {

    // set an empty unit
    Unit unit;
    nlohmann::json j = unit;

    // check against empty (elements stored are in alphabetical order)
    std::string res = R"({"acceleration":0.0,"curvature":0.0,"distance":0.0,"heading":{"x":1.0,"y":0.0,"z":0.0},"position":{"x":0.0,"y":0.0,"z":0.0},"velocity":0.0,"yawRate":0.0})";
    EXPECT_EQ(res, j.dump());

    // define a sample json
    nlohmann::json ji = {
      {"acceleration", 0.1},
      {"curvature", 0.01},
      {"distance", 120.0},
      {"position", {
           {"x", 10.0},
           {"y",  0.5},
           {"z",  0.0}
      }},
      {"velocity", 10.0},
      {"yawAngle", 0.2},
      {"yawRate", 0.02}
    };

    // merge with json
    j.merge_patch(ji);

    // create new unit
    Unit unit2 = j;

    // check data
    EXPECT_DOUBLE_EQ(  0.1,  unit2.acceleration);
    EXPECT_DOUBLE_EQ(  0.01, unit2.curvature);
    EXPECT_DOUBLE_EQ(120.0,  unit2.distance);
    EXPECT_DOUBLE_EQ( 10.0,  unit2.position.x);
    EXPECT_DOUBLE_EQ(  0.5,  unit2.position.y);
    EXPECT_DOUBLE_EQ(  0.0,  unit2.position.z);
    EXPECT_DOUBLE_EQ( 10.0,  unit2.velocity);
    EXPECT_DOUBLE_EQ(  1.0,  unit2.heading.x);
    EXPECT_DOUBLE_EQ(  0.0,  unit2.heading.y);
    EXPECT_DOUBLE_EQ(  0.0,  unit2.heading.z);
    EXPECT_DOUBLE_EQ(  0.02, unit2.yawRate);

}


#pragma clang diagnostic pop