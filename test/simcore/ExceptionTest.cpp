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
#pragma clang diagnostic ignored "-Wunused-result"
#pragma clang diagnostic ignored "performance-unnecessary-copy-initialization"
#pragma ide diagnostic ignored "cert-err58-cpp"

#include <gtest/gtest.h>
#include <simcore/utils/exceptions.h>

TEST(ExpectionTest, Basic) {

    sim::ProcessException pro1("An exception");
    sim::ProcessException pro2(pro1);
    sim::ProcessException pro3; pro3 = pro1;
    sim::ProcessException pro4;

    EXPECT_EQ("An exception", std::string(pro1.what()));
    EXPECT_EQ("An exception", std::string(pro2.what()));
    EXPECT_EQ("An exception", std::string(pro3.what()));
    EXPECT_EQ("", std::string(pro4.what()));

    sim::SetupException set1("An setup exception");
    sim::SetupException set2(set1);
    sim::SetupException set3; set3 = set1;
    sim::SetupException set4;

    EXPECT_EQ("An setup exception", std::string(set1.what()));
    EXPECT_EQ("An setup exception", std::string(set2.what()));
    EXPECT_EQ("An setup exception", std::string(set3.what()));
    EXPECT_EQ("", std::string(set4.what()));

}


#pragma clang diagnostic pop