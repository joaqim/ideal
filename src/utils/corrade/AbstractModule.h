#pragma once
#include "Corrade/PluginManager/AbstractManager.h"
#include <Corrade/PluginManager/Manager.h>
#include <Corrade/PluginManager/AbstractPlugin.h>
#include <Corrade/Containers/Array.h>

#define MODULE_PLUGIN_ID "xyz.joaqim.ideal.utils.corrade.AbstractModule/1.0"

/**
@brief Hot reloadable module
*/

class AbstractModule: public Corrade::PluginManager::AbstractPlugin {
  //CORRADE_PLUGIN_INTERFACE(MODULE_PLUGIN_ID);
  template<class T> using Array =  Corrade::Containers::Array<T>;

  public:
  AbstractModule(Corrade::PluginManager::AbstractManager &manager, const std::string &plugin):
    AbstractPlugin{manager, plugin} {}

  static std::string pluginInterface() {
    return MODULE_PLUGIN_ID;
  }


  /**
   * @brief Method called to load this module.
   * @param previousState Potential previous state of this plugin, if hot
   *                      reloaded, may be empty if first load or
   *                      implementation of `unload()` returned an empty
   *                      array.
   *
   * Use `previousState` with release to prevent data from being destroyed:
   *
   * @code
   *   _myState = static_cast<MyState*>(previousState.release());
   * @endcode
   */
  virtual void load(Array<void *> &&previousState) = 0;
  virtual void load() { load(Array<void *>{nullptr}); }
  /* virtual void load() { load(std::move(Containers::Array<void>{nullptr})); } */

  /**
   * @brief Method called when this module is unloaded.
   * @param shutdown `true` if the application is shutting down and no
   *                 state should be retained. `false` in case of hot
   *                 reload.
   * @returns State of the plugin to pass onto a hot reloaded version of
   *          itself or an empty array, if no state exists or state
   *          retention is not supported by this module.
   *
   * Warning: While it is possible to pass on a deleter with the Array,
   * this deleter will get unloaded with the module, if it isn't kept
   * around. Whether that works or not will therefore depend on how you
   * handle unloading.
   */
  virtual Array<void*>&& unload(bool shutdown=true) = 0;
};

//CORRADE_PLUGIN_REGISTER(AbstractModule, AbstractModule, "xyz.joaqim.ideal.AbstractModule/1.0")
