// Copyright (c) 2020 Jens Klimke <jens.klimke@rwth-aachen.de>. All rights reserved.
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
// 
// Created by Jens Klimke on 2020-03-22.
//


#include <simcore/traffic/UnitData.h>

void from_json(const json& j, UnitInterface::Vector3& o) {
    j.at("x").get_to(o.x);
    j.at("y").get_to(o.y);
    j.at("z").get_to(o.z);
}

void to_json(json& j, const UnitInterface::Vector3& o) {
    j = json{
        {"x", o.x},
        {"y", o.y},
        {"z", o.z}
    };
}


void from_json(const json& j, UnitInterface::State& o) {
    j.at("position").get_to(o.position);
    j.at("velocity").get_to(o.velocity);
    j.at("acceleration").get_to(o.acceleration);
    j.at("yawAngle").get_to(o.yawAngle);
    j.at("yawRate").get_to(o.yawRate);
    j.at("wheelAngle").get_to(o.wheelAngle);
}

void to_json(json& j, const UnitInterface::State& o) {
    j = json{
        {"position", o.position},
        {"velocity", o.velocity},
        {"acceleration", o.acceleration},
        {"yawAngle", o.yawAngle},
        {"yawRate", o.yawRate},
        {"wheelAngle", o.wheelAngle}
    };
}


void from_json(const json& j, UnitInterface::Parameters& o) {
    j.at("wheelBase").get_to(o.wheelBase);
    j.at("size").get_to(o.size);
    j.at("driverPosition").get_to(o.driverPosition);
}

void to_json(json& j, const UnitInterface::Parameters& o) {
    j = json{
        {"wheelBase", o.wheelBase},
        {"size", o.size},
        {"driverPosition", o.driverPosition}
    };
}


void from_json(const json& j, UnitInterface& o) {
    j.at("id").get_to(*o.getID_ptr());
    j.at("state").get_to(*o.getState());
    j.at("parameters").get_to(*o.getParameters());
}

void to_json(json& j, const UnitInterface& o) {
    j = json{
        {"id", *o.getID_ptr()},
        {"state", *o.getState()},
        {"parameters", *o.getParameters()}
    };
}


