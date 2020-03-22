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


#ifndef UNIT_MODEL_DATA_H
#define UNIT_MODEL_DATA_H

#include <nlohmann/json.hpp>
#include "UnitInterface.h"

using json = nlohmann::json;

void to_json(json& j, const UnitInterface& o);
void from_json(const json& j, UnitInterface& o);

void to_json(json& j, const UnitInterface::Vector3& o);
void from_json(const json& j, UnitInterface::Vector3& o);
void to_json(json& j, const UnitInterface::State& o);
void from_json(const json& j, UnitInterface::State& o);
void to_json(json& j, const UnitInterface::Parameters& o);
void from_json(const json& j, UnitInterface::Parameters& o);


#endif // UNIT_MODEL_DATA_H
