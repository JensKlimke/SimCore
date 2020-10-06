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

#include <simcore/storage/Manager.h>

namespace sim::storage {

    std::map<const IComponent*, unsigned long> Manager::componentIds = std::map<const IComponent*, unsigned long>{};

    void Manager::saveLoop(sim::protobuf::Loop &p, const sim::Loop *l) {

        // set ID
        p.set_id(1);

        // stop flag
        p.set_stop(l->_stop);

        // status
        switch(l->_status) {
            case Loop::Status::INITIALIZED:
                p.set_status(sim::protobuf::Loop_Status_INITIALIZED);
            case Loop::Status::RUNNING:
                p.set_status(sim::protobuf::Loop_Status_RUNNING);
            case Loop::Status::STOPPED:
                p.set_status(sim::protobuf::Loop_Status_STOPPED);
        }

        // create IDs for components
        unsigned long cid = 0;
        for (const auto &c : l->_components)
            componentIds.emplace(&c, cid);


    }

    /**
         * Converts the loop to json
         * @param j JSON object
         */
    virtual void to_json(nlohmann::json &j) const {

        j = nlohmann::json({
                                   {"status",         _status}, // TODO
                                   {"stop",           _stop},
                                   {"components",     {}}, // TODO
                                   {"stopConditions", {}} // TODO
                           });

    }


    /**
     * Reads loop attributes from json
     * @param j JSON object
     */
    virtual void from_json(const nlohmann::json &j) {

        j.get_to(_status);
        j.get_to(_stop);
        // TODO: components
        // TODO: stopConditions

    }

}