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
// Created by Jens Klimke on 2020-10-04.
// Contributors:
//

#include "Manager.h"

namespace sim {

    void Manager::saveLoop(sim::dump::Simulation &sim, const sim::Loop *_loop) {

        sim::dump::Loop loop{};

        loop.set_id(1);
        loop.set_stop(_loop->_stop);

        // status
        switch(_loop->_status) {
            case Loop::Status::INITIALIZED:
                loop.set_status(sim::dump::Loop_Status_INITIALIZED);
            case Loop::Status::RUNNING:
                loop.set_status(sim::dump::Loop_Status_RUNNING);
            case Loop::Status::STOPPED:
                loop.set_status(sim::dump::Loop_Status_STOPPED);
        }

        sim.set_allocated_loop(&loop);

    }

}