//
// Created by Jens Klimke on 2019-03-19.
//

#ifndef SIMCORE_PLUGIN_H
#define SIMCORE_PLUGIN_H


#ifdef WIN32
# define PLUGIN_EXPORT __declspec(dllexport)
#else
# define PLUGIN_EXPORT // empty
#endif

namespace sim {
namespace plugin {


    extern "C" typedef ::sim::plugin::IPlugin *(*CreatePluginFunc)();
    extern "C" typedef void (*DestroyPluginFunc)(::sim::plugin::IPlugin *);

    struct PluginDetails {
        const char *classname;
        const char *pluginname;
        const char *pluginversion;
        CreatePluginFunc initializeFunc;
        DestroyPluginFunc terminateFunc;
    };


#define PLUGIN(className, pluginName, pluginVersion)                      \
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
          #className,                                                     \
          pluginName,                                                     \
          pluginVersion,                                                  \
          create,                                                         \
          destroy                                                         \
      }                                                                   \
  }

}} // namespace ::sim::plugin

#endif //SIMCORE_PLUGIN_H
