//
// Created by Jens Klimke on 2019-03-19.
//

#ifndef SIMULATION_FRAMEWORK_PLUGIN_H
#define SIMULATION_FRAMEWORK_PLUGIN_H


#ifdef WIN32
# define PLUGIN_EXPORT __declspec(dllexport)
#else
# define PLUGIN_EXPORT // empty
#endif

namespace sim {

//** Plugin definitions for Timer plugins **//

    class IPlugin;

    extern "C" typedef ::sim::IPlugin *(*CreatePluginFunc)();
    extern "C" typedef void (*DestroyPluginFunc)(::sim::IPlugin *);

    struct PluginDetails {
        const char *classname;
        const char *pluginname;
        const char *pluginversion;
        CreatePluginFunc initializeFunc;
        DestroyPluginFunc terminateFunc;
    };


#define PLUGIN(classType, pluginName, pluginVersion)                      \
  extern "C" {                                                            \
      PLUGIN_EXPORT sim::IPlugin* create()                                \
      {                                                                   \
          auto *obj = new classType;                                      \
          return obj;                                                     \
      }                                                                   \
      PLUGIN_EXPORT void destroy(::sim::IPlugin *obj)                     \
      {                                                                   \
          delete obj;                                                     \
      }                                                                   \
      PLUGIN_EXPORT sim::PluginDetails exports =                          \
      {                                                                   \
          #classType,                                                     \
          pluginName,                                                     \
          pluginVersion,                                                  \
          create,                                                         \
          destroy                                                         \
      };                                                                  \
  }

};

#endif //SIMULATION_FRAMEWORK_PLUGIN_H
