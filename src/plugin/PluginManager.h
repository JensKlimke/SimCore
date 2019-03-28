//
// Created by Jens Klimke on 2019-03-20.
//

#ifndef SIMULATION_FRAMEWORK_PLUGINMANAGER_H
#define SIMULATION_FRAMEWORK_PLUGINMANAGER_H

#include <unordered_map>
#include <string>

#ifdef __APPLE__
#define PLUGIN_EXTENSION "dylib"
#elif  __linux__
#define PLUGIN_EXTENSION "so"
#elif _WIN32
#define PLUGIN_EXTENSION "dll"
#endif


namespace sim {


    // forward class declaration for PluginManager
    class IPlugin;
    class PluginDetails;


    class PluginManager {

        std::unordered_map<IPlugin*, PluginDetails*> _instances{};
        std::unordered_map<std::string, PluginDetails*> _plugins{};


    public:


        /**
         * Default constructor
         */
        PluginManager() = default;


        /**
         * Destructor
         */
        virtual ~PluginManager();


        /**
         * Loader for plugins
         * @param lib Library to be loaded
         * @return Success flag
         */
        bool loadPlugin(const std::string &name, const std::string &lib);


        /**
         * Returns a pointer to a instance of a plugin
         * @param name Name of the plugin
         * @return Pointer to the instance
         */
        IPlugin *instance(const std::string &name);


        /**
         * Returns a json string with the plugin names
         * @return JSON string
         */
        std::string pluginList() const;


    };



}; // namespace ::sim

#endif //SIMULATION_FRAMEWORK_PLUGINMANAGER_H
