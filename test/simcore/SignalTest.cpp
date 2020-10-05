//
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
// Created by Jens Klimke on 2020-08-06.
//

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-pragmas"
#pragma ide diagnostic ignored "cert-err58-cpp"


#include <gtest/gtest.h>
#include <simcore/storage/Signal.h>

struct Complex {
    double real;
    double imag;

    Complex() = default;

    Complex(double r, double i) : real(r), imag(i) {}
};

class SignalTest : public ::testing::Test {

    void SetUp() override {
        _double = 0.0;
    }

protected:

    Signal<Complex> _complex{"Complex", "Signal"};
    Signal<double> _double{"Basic", "Double"};

};


TEST_F(SignalTest, Naming) {

    // check names
    EXPECT_EQ("Basic.Double", _double.getName());
    EXPECT_EQ("Complex.Signal", _complex.getName());

}


TEST_F(SignalTest, BasicType) {

    // set and check content
    _double = 1.0;
    EXPECT_DOUBLE_EQ(1.0, _double);

    // set and check content
    _double = 2.0;
    EXPECT_DOUBLE_EQ(2.0, _double);

}


TEST_F(SignalTest, ComplexType) {

    // create complex number
    _complex = Complex{1.0, 2.0};
    Complex c = _complex;

    // check content
    EXPECT_DOUBLE_EQ(1.0, c.real);
    EXPECT_DOUBLE_EQ(2.0, c.imag);

}


TEST_F(SignalTest, Callback) {


    // variable to store result
    Complex vp{};

    // define lambda function
    auto fnc = [&vp](const double &n, const double &o, const Signal<double> &signal) {

        // set values
        vp.real = n;
        vp.imag = o;

        // check signal
        EXPECT_EQ("Basic.Double", signal.getName());

    };

    // set callback
    _double.setUpdateCallback(fnc);

    // set value and check
    _double = 3.0;
    EXPECT_DOUBLE_EQ(3.0, vp.real);
    EXPECT_DOUBLE_EQ(0.0, vp.imag);

    // set value and check
    _double = 2.0;
    EXPECT_DOUBLE_EQ(2.0, vp.real);
    EXPECT_DOUBLE_EQ(3.0, vp.imag);

}


#pragma clang diagnostic pop