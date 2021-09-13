#pragma once

#include "AbstractModuleLoader.h"

#include <Corrade/PluginManager/Manager.h>
#include <Corrade/Utility/Directory.h>

#include <utils/corrade/AbstractModule.h>

#include <cstdint>
#include <map>

namespace Ideal {
  using namespace Corrade::Utility;
  class ModuleLoader : AbstractModuleLoader {
    public:

      bool load(const char *moduleName);
      bool unload(const char *moduleName = "");

    private:

      Corrade::PluginManager::Manager<AbstractModule> _moduleManager{Directory::path(Directory::current()+"/lib/")};
      std::map<std::string, std::unique_ptr<AbstractModule>> _mpModules;

      std::unique_ptr<AbstractModule> _pModule;
      std::string _lastModuleName = "";
  };
}
