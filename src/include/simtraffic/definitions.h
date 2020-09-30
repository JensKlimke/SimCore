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
// Created by Jens Klimke on $date.get('yyyy-M-d').
// Contributors:
//


#ifndef SIMTRAFFIC_VIRTUAL_VEHICLE_DEFINITIONS_H
#define SIMTRAFFIC_VIRTUAL_VEHICLE_DEFINITIONS_H


#ifndef EPS_SIM_TIME
#define EPS_SIM_TIME 1e-9
#endif

#ifndef EPS_VELOCITY
#define EPS_VELOCITY 1e-6
#endif

#ifndef ZERO_VELOCITY
#define ZERO_VELOCITY 1e-3
#endif

#ifndef EPS_DISTANCE
#define EPS_DISTANCE 1e-3
#endif

#ifndef EPS_ANGLE
#define EPS_ANGLE 1e-6
#endif

#ifndef range
#define range(X, MIN, MAX) (X < MIN ? MIN : (X < MAX ? X : MAX))
#endif

#endif //SIMTRAFFIC_VIRTUAL_VEHICLE_DEFINITIONS_H
