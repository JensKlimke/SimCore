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
// Created by Jens Klimke on 2021-03-14.
//

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-pragmas"
#pragma ide diagnostic ignored "cert-err58-cpp"

#include <gtest/gtest.h>
#include <simbasic/Polynomial.h>

class PolynomialTest : public ::testing::Test, public simbasic::Polynomial {

    void SetUp() override {

    }

};

TEST_F(PolynomialTest, CreateContant) {

    auto p = Polynomial(2.0);

    EXPECT_DOUBLE_EQ(2.0, p.front().parameter);
    EXPECT_EQ(0, p.front().order);

}


TEST_F(PolynomialTest, Derivative) {

    // add elements
    add(-2, 1.0);
    add(-1, 2.0);
    add( 0, 3.0);
    add( 1, 4.0);
    add( 2, 5.0);
    add( 3, 6.0);

    // calculate derivative
    auto der = derivative();

    // check
    EXPECT_EQ(-3, der[0].order);
    EXPECT_DOUBLE_EQ(-2.0, der[0].parameter);
    EXPECT_EQ(-2, der[1].order);
    EXPECT_DOUBLE_EQ(-2.0, der[1].parameter);
    EXPECT_EQ( 0, der[3].order);
    EXPECT_DOUBLE_EQ(4.0, der[3].parameter);
    EXPECT_EQ( 1, der[4].order);
    EXPECT_DOUBLE_EQ(10.0, der[4].parameter);
    EXPECT_EQ( 2, der[5].order);
    EXPECT_DOUBLE_EQ(18.0, der[5].parameter);

}

#pragma clang diagnostic pop