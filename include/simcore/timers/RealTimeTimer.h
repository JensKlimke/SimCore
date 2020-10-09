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
// Created by Jens Klimke on 2019-03-27.
//

#ifndef SIMCORE_REAL_TIME_TIMER_H
#define SIMCORE_REAL_TIME_TIMER_H

#include <ctime>
#include <iostream>
#include <chrono>
#include <thread>
#include "BasicTimer.h"
#include "SynchronizedTimer.h"

namespace sim {

    class RealTimeTimer : public SynchronizedTimer {

    private:

        std::thread _timeThread{};
        bool _stopSync{};


    public:


        /*!< Constructor */
        RealTimeTimer() = default;

        /*!< Destructor */
        ~RealTimeTimer() override = default;


        /**
         * @brief Starts the timer
         * Starts a thread counting the time from now
         */
        void start() override {

            // set flag
            _stopSync = false;

            // reset reference time
            this->setReferenceTime(0.0);

            // run timing thread
            _timeThread = std::thread([this]() {
                this->realtimeStep();
            });

            // run super method
            SynchronizedTimer::start();

        }


        void stop() override {

            // set flag
            _stopSync = true;

            // wait for time thread to finish
            _timeThread.join();

            // run super method
            SynchronizedTimer::stop();

        }


        /**
         * Stores the state to the given JSON object
         * @param obj JSON object
         */
        void toJSON(nlohmann::json &obj) const override {
            BasicTimer::toJSON(obj);
            obj["stopSync"] = _stopSync;
        }


        /**
         * Sets the state given by the JSON object
         * @param obj JSON object
         */
        void fromJSON(const nlohmann::json &obj) override {
            BasicTimer::fromJSON(obj);
            obj["stopSync"].get_to(_stopSync);
        }


    protected:

        void realtimeStep() {

            // chrono namespace
            using namespace std::chrono;

            // loop until
            auto refTime = system_clock::now();
            while (!this->_stopSync) {

                // calculate real time
                auto elapsed = duration_cast<microseconds>(system_clock::now() - refTime);
                auto currTime = static_cast<double>(elapsed.count()) / 1e6;

                // set reference time
                this->setReferenceTime(currTime);

                // sleep
                std::this_thread::sleep_for(std::chrono::microseconds(10));

            }

        }

    };


}


#endif // SIMCORE_REAL_TIME_TIMER_H
