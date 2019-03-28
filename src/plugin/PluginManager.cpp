//
// Created by klimke on 21.03.2019.
//

#include "PluginManager.h"
#include "ITimer.h"
#include "IModel.h"
#include "IPlugin.h"
#include "Plugin.h"
#include <dlfcn.h>
#include <sstream>


namespace sim {

    PluginManager::~PluginManager() {

        // iterate over instances and delete
        for (auto inst : _instances)
            inst.second->terminateFunc(inst.first);

    }


    bool PluginManager::loadPlugin(const std::string &name, const std::string &lib) {

        // load library
        void *handle = dlopen(lib.c_str(), RTLD_LAZY);

        // check handle
        if (handle == nullptr)
            return false;

        // add to map
        auto config = (PluginDetails *) dlsym(handle, "exports");
        _plugins[name] = config;

        // success
        return true;

    }


    IPlugin *PluginManager::instance(const std::string &name) {

        // get plugin information and create timer
        auto p = _plugins.at(name);
        auto t = p->initializeFunc();

        // add timer to the instances vector
        _instances[t] = p;

        return t;

    }


    std::string PluginManager::pluginList() const {

        std::stringstream ss;
        ss << "{\"timers\":{";

        // iterate over timer plugins
        unsigned int i = 0;
        for (auto &pl : _plugins)
            ss << (i++ == 0 ? "" : ",") << "\"" << pl.first << "\":{"
               << R"("classname":")" << pl.second->classname << "\","
               << R"("pluginname":")" << pl.second->pluginname << "\","
               << R"("pluginversion":")" << pl.second->pluginversion << "\",";

        ss << "}};";
        return ss.str();

    }

}; // namespace ::sim