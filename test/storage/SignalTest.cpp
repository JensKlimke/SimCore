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


void to_json(nlohmann::json &j, const Complex &c) {
    j = nlohmann::json{{"real", c.real},
                       {"imag", c.imag}};
}

void from_json(const nlohmann::json &j, Complex &c) {
    j.at("name").get_to(c.real);
    j.at("address").get_to(c.imag);
}


class SignalTest : public ::testing::Test {

    void SetUp() override {
    }


protected:

    sim::storage::Signal<Complex> _complex{this, "signal"};
    SIG_DEFINE_DOUBLE(double)
    SIG_DEFINE_BOOL(bool)
    SIG_DEFINE_FLOAT(float)
    SIG_DEFINE_CHAR(char)
    SIG_DEFINE_UCHAR(unsigned_char)
    SIG_DEFINE_SHORT(short)
    SIG_DEFINE_USHORT(unsigned_short)
    SIG_DEFINE_LONGLONG(long_long)
    SIG_DEFINE_ULONGLONG(unsigned_long_long)
    SIG_DEFINE_INT(int)
    SIG_DEFINE_UINT(unsigned_int)
    SIG_DEFINE_LONG(long)
    SIG_DEFINE_ULONG(unsigned_long)
    SIG_DEFINE_STRING(string)

};


TEST_F(SignalTest, NamingAndOwnership) {

    // check names
    EXPECT_EQ("double", _double.getName());
    EXPECT_EQ("signal", _complex.getName());

    // check owners
    EXPECT_EQ(this, _double.getOwner());
    EXPECT_EQ(this, _complex.getOwner());

}


TEST_F(SignalTest, Manager) {

    // a variable
    int a = 0;

    // out of def definition
    sim::storage::Signal<double> time(&a, "time");
    time = 10.0;

    // set value
    _double = -2.0;

    // check pointer
    EXPECT_EQ(&time, sim::storage::signal<double>(&a, "time"));
    EXPECT_EQ(&_double, sim::storage::signal<double>(this, "double"));

    // check values
    EXPECT_DOUBLE_EQ(10.0, sim::storage::value<double>(&a, "time"));
    EXPECT_DOUBLE_EQ(-2.0, sim::storage::value<double>(this, "double"));

}


TEST_F(SignalTest, BasicTypes) {

    // set content
    _bool = true;
    _double = -1.0;
    _float = -1.0f;
    _char = -12;
    _unsigned_char = 12;
    _short = -12345;
    _unsigned_short = 12345;
    _int = -1234567890;
    _unsigned_int = 1234567890;
    _long = -1234567890;
    _unsigned_long = 1234567890;
    _long_long = -1234567890;
    _unsigned_long_long = 1234567890;
    _string = "Hello World";

    // check content
    EXPECT_TRUE(_bool);
    EXPECT_DOUBLE_EQ(-1.0, _double);
    EXPECT_FLOAT_EQ(-1.0f, _float);
    EXPECT_EQ(-12, _char);
    EXPECT_EQ(12, _unsigned_char);
    EXPECT_EQ(-12345, _short);
    EXPECT_EQ(12345, _unsigned_short);
    EXPECT_EQ(-1234567890, _int);
    EXPECT_EQ(1234567890, _unsigned_int);
    EXPECT_EQ(-1234567890, _long);
    EXPECT_EQ(1234567890, _unsigned_long);
    EXPECT_EQ(-1234567890, _long_long);
    EXPECT_EQ(1234567890, _unsigned_long_long);
    EXPECT_EQ("Hello World", (std::string) _string);

    // set double again
    _double = 2.0;
    EXPECT_DOUBLE_EQ(2.0, _double);

}


TEST_F(SignalTest, Calculations) {

    _double = -1.0;
    _double += 1.0;

    EXPECT_DOUBLE_EQ(0.0, _double);

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
    auto fnc = [this, &vp](const double &n, const double &o, const sim::storage::Double &signal) {

        // set values
        vp.real = n;
        vp.imag = o;

        // check signal
        EXPECT_EQ("double", signal.getName());
        EXPECT_EQ(this, signal.getOwner());

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