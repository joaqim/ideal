#include "ModuleLoader.h"
#include <cstring>

namespace Ideal {
using namespace Corrade;

bool ModuleLoader::load(const char *moduleName) {
  if (!(_moduleManager.load(moduleName) & PluginManager::LoadState::Loaded)) {
    Error{} << "Module: " << moduleName << " can not be loaded.";
    return false;
  }
  _mpModules.emplace(moduleName, _moduleManager.instantiate(moduleName));
  _mpModules.at(moduleName)->load();
  _lastModuleName = moduleName;
  return true;
}

bool ModuleLoader::unload(const char *moduleName) {
  if (strcmp(moduleName, "")) {
    moduleName = _lastModuleName.c_str();
  }
  _mpModules.at(moduleName)->unload();
  _mpModules.at(moduleName).reset(nullptr);

  if (_moduleManager.unload(moduleName) &
      PluginManager::LoadState::UnloadFailed) {
    Error{} << "Module: " << moduleName << " failed to unload.";
    return false;
  }

  return true;
}

}; // namespace Ideal
