#include "HotModuleLoader.h"
#include "Corrade/Containers/Array.h"
#include "Corrade/Containers/Tags.h"
#include "Corrade/Utility/Debug.h"
#include "Corrade/Utility/FileWatcher.h"

#include <cstring>
#include <chrono> // milliseconds
#include <thread> // this_thread::sleep_for

namespace Ideal {

impl::UpdateListener::UpdateListener(AbstractModuleLoader *moduleLoader)
    : _pModuleLoader{moduleLoader}, _moveCount{0} {}

void impl::UpdateListener::handleFileAction(efsw::WatchID watchid,
                                            const std::string &dir,
                                            const std::string &filename,
                                            efsw::Action action,
                                            std::string _oldFilename) {
  switch (action) {
#if 1
  case efsw::Actions::Add:
      Debug{} << "DIR (" << dir.c_str() << ") FILE (" << filename.c_str() << ") has event Added" ;
      break;
    case efsw::Actions::Delete:
      Debug{} << "DIR (" << dir.c_str() << ") FILE (" << filename.c_str() << ") has event Delete";
      break;
    case efsw::Actions::Modified:
      Debug{} << "DIR (" << dir.c_str() << ") FILE (" << filename.c_str() << ") has event Modified";
      break;
    case efsw::Actions::Moved:
      Debug{} << "DIR (" << dir.c_str() << ") FILE (" << filename.c_str() << ") has event Moved from (" << _oldFilename.c_str() << ")";
#else
    case efsw::Actions::Moved:
#endif
      Debug{} << "DIR (" << dir.c_str() << ") FILE (" << filename.c_str() << ") has event Moved from (" << _oldFilename.c_str() << ")";
      _moveCount++;
      if (_moveCount >= 2) {
        const std::string moduleName = Corrade::Utility::Directory::splitExtension(filename).first;
        _pModuleLoader->reload(moduleName.c_str());
        _moveCount = 0;
      }
      break;
    default:
      break;
  }
}

using namespace Corrade;
using namespace Corrade::Utility;

HotModuleLoader::HotModuleLoader() : _updateListener{this} {};
HotModuleLoader::~HotModuleLoader() {
  for(auto const &watcher : _mWatchIDs) {
    unload(watcher.first.c_str());
  }
}

bool HotModuleLoader::load(const char *moduleName) {
  /* Directory::write("lib/ImGuiStyleModule_Runtime.so", Directory::read("lib/ImGuiStyleModule.so")); */
  //moduleName = std::string(std::string{moduleName} + "_Runtime").c_str();  

  if (!(_moduleManager.load(moduleName) & PluginManager::LoadState::Loaded)) {
    Error{} << "Module: " << moduleName << " can not be loaded.";
    return false;
  }

  const auto watchID = _fileWatcher.addWatch(
#if 0
      Directory::path(Directory::current() + "/lib/" + moduleName + ".so").c_str(),
#else
      Directory::path(Directory::current() + "/lib/").c_str(),
#endif
      &_updateListener,
      false);

  _mpModules.try_emplace(moduleName, _moduleManager.instantiate(moduleName));
  _mWatchIDs.emplace(moduleName, watchID);

  //_mWatchers.try_emplace(moduleName, FileWatcher{Directory::path(
  //Directory::current() + "/lib/" + moduleName)});

  _mpModules.at(moduleName)->load();
  _lastModuleName = moduleName;

  return true;
}

bool HotModuleLoader::reload(const char *moduleName) {
  if (strcmp(moduleName, "")) {
    moduleName = _lastModuleName.c_str();
  }

  _mpModules.at(moduleName)->unload();
  _mpModules.at(moduleName).reset(nullptr);
  if (_moduleManager.unload(moduleName) &
      PluginManager::LoadState::UnloadFailed) {
    Error{} << "Module:" << moduleName << "failed to unload.";
    return false;
  }

  std::this_thread::sleep_for(std::chrono::milliseconds(300));

  if (!(_moduleManager.load(moduleName) & PluginManager::LoadState::Loaded)) {
    Error{} << "Module: " << moduleName << " can not be loaded.";
    return false;
  }


  _mpModules.try_emplace(moduleName, _moduleManager.instantiate(moduleName));
  //_mpModules.at(moduleName)->load();
  _lastModuleName = moduleName;


  return true;
}

bool HotModuleLoader::unload(const char *moduleName) {
  if (strcmp(moduleName, "")) {
    moduleName = _lastModuleName.c_str();
  }
  _mpModules.at(moduleName)->unload();
  _mpModules.at(moduleName).reset(nullptr);

  _fileWatcher.removeWatch(_mWatchIDs.at(moduleName));
  _mWatchIDs.erase(moduleName);

  if (_moduleManager.unload(moduleName) &
      PluginManager::LoadState::UnloadFailed) {
    Error{} << "Module:" << moduleName << "failed to unload.";
    return false;
  }

  return true;
}

bool HotModuleLoader::update() {

  _fileWatcher.watch();

  return true;
};

}; // namespace Ideal
