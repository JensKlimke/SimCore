//
// Copyright (c) 2019 Jens Klimke <jens.klimke@rwth-aachen.de>. All rights reserved.
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
// Created by Jens Klimke on 2019-03-19
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
