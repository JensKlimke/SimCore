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
// Created by Jens Klimke on 2019-04-16.
//

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-pragmas"
#pragma ide diagnostic ignored "cert-err58-cpp"


#include <gtest/gtest.h>
#include <map>
#include <simcore/storage/Storable.h>


class StorageTest : public ::testing::Test, public sim::storage::Storable, protected sim::storage::DataNode {

public:


    struct Complex {
        double a;
        double b;
    };

    struct Vector2 {
        double x;
        double y;
    };

    double signal01{};
    int signal02{};

    Complex complex01{};
    Vector2 complex02{};


    void registerSignals(DataNode &node) override {

        const std::string alias = "alias";

        REGISTER_SIGNAL(signal01);
        REGISTER_SIGNAL_ALIAS(signal02, alias);
        REGISTER_SIGNAL_CONTEXT(a, complex01);
        REGISTER_SIGNAL_CONTEXT_ALIAS(b, complex01, "field");
        REGISTER_SIGNAL(complex02);

    }

    void fromNode(const DataNode &node) override {

        const std::string alias = "alias";

        WRITE_SIGNAL(signal01);
        WRITE_SIGNAL_ALIAS(signal02, alias);
        WRITE_SIGNAL_CONTEXT(a, complex01);
        WRITE_SIGNAL_CONTEXT_ALIAS(b, complex01, "field");
        WRITE_SIGNAL(complex02);

    }


    void SetUp() override {

        // register
        registerSignals(*this);

    }

};


TEST_F(StorageTest, CreateStorage) {

    // define a key in a variable
    const std::string alias = "alias";

    // check signals
    EXPECT_EQ(5, this->signals.size());

    // compare pointers
    EXPECT_EQ(&this->signal01, this->get<double>("signal01"));
    EXPECT_EQ(&this->signal02, this->get<int>(alias));
    EXPECT_EQ(&this->complex01.a, this->get<double>("a"));
    EXPECT_EQ(&this->complex01.b, this->get<double>("field"));
    EXPECT_EQ(&this->complex02, this->get<Vector2>("complex02"));


}


TEST_F(StorageTest, SetValuesByAttribute) {

    // set values
    this->signal01 = 1.0;
    this->signal02 = 2.0;
    this->complex01 = {3.0, 4.0};
    this->complex02 = {5.0, 6.0};

    // compare pointers
    EXPECT_DOUBLE_EQ(1.0, *this->get<double>("signal01"));
    EXPECT_EQ(2,          *this->get<int>("alias"));
    EXPECT_DOUBLE_EQ(3.0, *this->get<double>("a"));
    EXPECT_DOUBLE_EQ(4.0, *this->get<double>("field"));
    EXPECT_DOUBLE_EQ(5.0, this->get<Vector2>("complex02")->x);
    EXPECT_DOUBLE_EQ(6.0, this->get<Vector2>("complex02")->y);


}


TEST_F(StorageTest, SetValuesBySignal) {

    // set values
    this->signal01 = 1.0;
    this->signal02 = 2.0;
    this->complex01 = {3.0, 4.0};
    this->complex02 = {5.0, 6.0};

    // compare attributes
    EXPECT_DOUBLE_EQ(1.0, this->signal01);
    EXPECT_EQ(2,          this->signal02);
    EXPECT_DOUBLE_EQ(3.0, this->complex01.a);
    EXPECT_DOUBLE_EQ(4.0, this->complex01.b);
    EXPECT_DOUBLE_EQ(5.0, this->complex02.x);
    EXPECT_DOUBLE_EQ(6.0, this->complex02.y);

}


TEST_F(StorageTest, SetValuesByNode) {

    // define local values
    double s1 = 1.0;
    int s2 = 2;
    double ca = 3.0;
    double cb = 4.0;

    // define vector
    struct {
        double field01;
        double field02;
    } vec = {5.0, 6.0};

    // create fake node
    sim::storage::DataNode node{};
    node.create("signal01", &s1);
    node.create("alias", &s2);
    node.create("a", &ca);
    node.create("field", &cb);
    node.create("complex02", &vec);

    // write
    fromNode(node);

    // unset local values
    s1 = 0.0;
    s2 = 0;
    ca = 0.0;
    cb = 0.0;
    vec = {0.0, 0.0};

    // compare pointers
    EXPECT_DOUBLE_EQ(1.0, *this->get<double>("signal01"));
    EXPECT_EQ(2,          *this->get<int>("alias"));
    EXPECT_DOUBLE_EQ(3.0, *this->get<double>("a"));
    EXPECT_DOUBLE_EQ(4.0, *this->get<double>("field"));
    EXPECT_DOUBLE_EQ(5.0, this->get<Vector2>("complex02")->x);
    EXPECT_DOUBLE_EQ(6.0, this->get<Vector2>("complex02")->y);

    // compare attributes
    EXPECT_DOUBLE_EQ(1.0, this->signal01);
    EXPECT_EQ(2,          this->signal02);
    EXPECT_DOUBLE_EQ(3.0, this->complex01.a);
    EXPECT_DOUBLE_EQ(4.0, this->complex01.b);
    EXPECT_DOUBLE_EQ(5.0, this->complex02.x);
    EXPECT_DOUBLE_EQ(6.0, this->complex02.y);

}


TEST_F(StorageTest, Hierarchy) {

    // creates nodes
    sim::storage::DataNode node;
    sim::storage::DataNode root;

    // setup node
    double first = 1.0, second = 2.0;
    node.create("first", &first);
    node.create("second", &second);

    // setup root
    int i = 3;
    root.create("i", &i);

    // add nodes
    root.addNode("this", this);
    root.addNode("node", &node);

    // access by root
    *root.getNode("this")->get<double>("signal01") = 11.0;
    root.getNode("this")->get<Vector2>("complex02")->x = 12.0;

    // check
    EXPECT_DOUBLE_EQ(11.0, signal01);
    EXPECT_DOUBLE_EQ(12.0, complex02.x);

}


#pragma clang diagnostic pop