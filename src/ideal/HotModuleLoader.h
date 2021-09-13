#pragma once

#include "AbstractModuleLoader.h"
#include "Corrade/Utility/FileWatcher.h"

#include <Corrade/PluginManager/Manager.h>
#include <Corrade/Utility/Directory.h>

#include <utils/corrade/AbstractModule.h>

#include <efsw/efsw.hpp>

#include <cstdint>
#include <map>

namespace Ideal {

  namespace impl {
    class UpdateListener : public ::efsw::FileWatchListener {
      public:
        UpdateListener(AbstractModuleLoader *moduleLoader);
        void handleFileAction( ::efsw::WatchID watchid, const std::string& dir, const std::string& filename, ::efsw::Action action, std::string _oldFilename = "" );
      private:
        AbstractModuleLoader *_pModuleLoader;
        uint8_t _moveCount{0};
        std::map<std::string, uint8_t> _mMoveCount;
    };
  };

  using namespace Corrade::Utility;

  class HotModuleLoader : AbstractModuleLoader {
    public:
      HotModuleLoader();
      ~HotModuleLoader();
      bool load(const char *moduleName);
      bool unload(const char *moduleName = "");
      bool reload(const char *moduleName = "");
      bool update();
    private:
      Corrade::PluginManager::Manager<AbstractModule> _moduleManager{Directory::path(Directory::current()+"/lib/")};
      std::map<std::string, std::unique_ptr<AbstractModule>> _mpModules;
      /* std::map<std::string, std::pair<FileWatcher,std::unique_ptr<AbstractModule>>> _mpModules; */

      std::map<std::string, FileWatcher> _mWatchers;

      impl::UpdateListener _updateListener;
      efsw::FileWatcher _fileWatcher {};
      std::map<std::string, efsw::WatchID> _mWatchIDs;

      std::unique_ptr<AbstractModule> _pModule;
      std::string _lastModuleName = "";
  };
}
