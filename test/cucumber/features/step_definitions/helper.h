// Copyright (c) 2020 Jens Klimke (jens.klimke@rwth-aachen.de). All rights reserved.
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
// Created by Jens Klimke on 2020-10-02
//

#ifndef SIMCORE_CUCUMBER_HELPER
#define SIMCORE_CUCUMBER_HELPER

template<class T>
inline std::vector<T> getParameters(std::string str) {

    // pattern
    boost::regex regex("([\\.0-9]+)");

    // create iterators
    boost::sregex_token_iterator iter(str.begin(), str.end(), regex, 0);
    boost::sregex_token_iterator end;

    // get results
    std::vector<T> results{};
    for (; iter != end; ++iter) {

        // put result into stream
        std::stringstream ss;
        ss << *iter;

        // get result from stream
        T a;
        ss >> a;

        // put to vector
        results.emplace_back(std::move(a));

    }

    return results;

}

#endif // SIMCORE_CUCUMBER_HELPER