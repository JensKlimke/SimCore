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


#include <simtraffic/UnitData.h>

void from_json(const json& j, Unit::Vector3& o) {
    j.at("x").get_to(o.x);
    j.at("y").get_to(o.y);
    j.at("z").get_to(o.z);
}

void to_json(json& j, const Unit::Vector3& o) {
    j = json{
        {"x", o.x},
        {"y", o.y},
        {"z", o.z}
    };
}


void from_json(const json& j, Unit& o) {
    j.at("position").get_to(o.position);
    j.at("distance").get_to(o.distance);
    j.at("velocity").get_to(o.velocity);
    j.at("acceleration").get_to(o.acceleration);
    j.at("yawAngle").get_to(o.yawAngle);
    j.at("yawRate").get_to(o.yawRate);
    j.at("curvature").get_to(o.curvature);
}

void to_json(json& j, const Unit& o) {
    j = json{
        {"position", o.position},
        {"distance", o.distance},
        {"velocity", o.velocity},
        {"acceleration", o.acceleration},
        {"yawAngle", o.yawAngle},
        {"yawRate", o.yawRate},
        {"curvature", o.curvature}
    };
}
