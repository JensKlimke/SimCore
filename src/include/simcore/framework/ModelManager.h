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
// Created by Jens Klimke on $date.get('yyyy-M-d').
// Contributors:
//


#ifndef SIMCORE_MODELMANAGER_H
#define SIMCORE_MODELMANAGER_H

#include <yaml-cpp/yaml.h>
#include <functional>
#include "../Model.h"

namespace sim::framework {

    typedef YAML::Node Config;
    typedef std::function<sim::IComponent *(const Config &)> CreatorFunction;


    class ModelManager {

        static std::map<std::string, CreatorFunction> _index;
        std::vector<sim::IComponent*> _creations{};

    public:

        /**
         * Default constructor
         */
        ModelManager();


        /**
         * @brief Destructor of the model manager
         * Deletes all created models
         */
        virtual ~ModelManager();


        /**
         * Registers the model type
         * @param type Type of the model
         * @param factory Factory function
         */
        static void registerModelType(const std::string &type, CreatorFunction &&factory);


        /**
         * Creates a component of the given type
         * @param type Type of the model
         * @param parameters Parameters of the model
         * @return The instance of the component
         */
        sim::IComponent *create(const std::string &type, const Config &parameters);

    };

}

#endif //SIMCORE_MODELMANAGER_H
