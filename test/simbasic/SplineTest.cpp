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
// Created by Jens Klimke on 2021-03-10.
//

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-pragmas"
#pragma ide diagnostic ignored "cert-err58-cpp"

#include <gtest/gtest.h>
#include <simbasic/Spline.h>

class SplineTest : public ::testing::Test, public simbasic::Spline {

    void SetUp() override {

    }

};


TEST_F(SplineTest, Spline) {

    using namespace simbasic;

    // parameters
    double a = -2.6239e-4;
    std::vector<double> pl{0.5, -0.3, 5.9737e-2, -0.004};
    std::vector<double> ph{10.0, 0.0, 1.0390e-1, -1.8807e-2, 7.2337e-4};


    // define first polynomial
    Polynomial pLow0(std::move(pl));
    Polynomial pLow1{};

    // a v^2
    pLow1.emplace_back(PolyParam{2, a});

    // define second polynomial
    Polynomial pHigh0(std::move(ph));
    Polynomial pHigh1{};

    // p/v + a v^2
    pHigh1.emplace_back(PolyParam{-1, 90.0});
    pHigh1.emplace_back(PolyParam{2, a});

    // define spline
    Spline spLow{};
    spLow.emplace_back(SplineSegment{5.0, pLow0});
    spLow.emplace_back(SplineSegment{INFINITY, pLow1});

    // define spline
    Spline spHigh{};
    spHigh.emplace_back(SplineSegment{12.0, pHigh0});
    spHigh.emplace_back(SplineSegment{INFINITY, pHigh1});

    EXPECT_NEAR( 0.5,    spLow(0.0),  1e-4);
    EXPECT_NEAR( 0.0296, spLow(3.0),  1e-4);
    EXPECT_NEAR(-0.0065, spLow(5.0),  1e-4);
    EXPECT_NEAR(-0.1049, spLow(20.0), 1e-4);
    EXPECT_NEAR(-1.2857, spLow(70.0), 1e-4);
    EXPECT_NEAR(-1.6375, spLow(79.0), 1e-4);

    EXPECT_NEAR(10.0,    spHigh(0.0),  1e-4);
    EXPECT_NEAR(10.4859, spHigh(3.0),  1e-4);
    EXPECT_NEAR(10.6987, spHigh(5.0),  1e-4);
    EXPECT_NEAR( 4.3950, spHigh(20.0), 1e-4);
    EXPECT_NEAR( 0.0,    spHigh(70.0), 1e-4);
    EXPECT_NEAR(-0.4983, spHigh(79.0), 1e-4);

}


TEST_F(SplineTest, Create) {

    // TODO

}


#pragma clang diagnostic pop