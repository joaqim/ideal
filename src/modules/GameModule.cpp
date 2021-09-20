#include "../utils/corrade/AbstractModule.h"
#include <Corrade/Containers/Tags.h>
#include <Corrade/PluginManager/AbstractManager.h>
#include <Corrade/Utility/Debug.h>

#include <memory>

struct GameModuleState {
  void *gameState;
};

class GameModule : public AbstractModule {
public:
  using Debug = Corrade::Utility::Debug;

  GameModule(Corrade::PluginManager::AbstractManager &manager,
                   const std::string &plugin)
      : AbstractModule(manager, plugin) {}

  static std::string pluginInterface() { return MODULE_PLUGIN_ID; }

  virtual void
  load(Corrade::Containers::Array<void *> &&previousState) override {
    Debug{} << "ImGuiStyleModule::load()";

    if (previousState.empty()) {
      Debug{} << "Initializing state";
      _state.reset(new GameModuleState);
    } else {
      _state.reset(static_cast<GameModuleState *>(*previousState.release()));
    }
  }

  virtual Corrade::Containers::Array<void *> &&unload(bool shutdown) override {
    Debug{} << "ImGuiStyleModule::unload(" << shutdown << ")";

    GameModuleState *state = _state.release();
    Debug{} << "Sizeof state:" << sizeof(GameModuleState);

    return std::move(Corrade::Containers::Array<void *>{
        Corrade::Containers::InPlaceInit, {static_cast<void *>(state)}});
  }

private:
  std::unique_ptr<GameModuleState> _state;
};

CORRADE_PLUGIN_REGISTER(GameModule, GameModule, MODULE_PLUGIN_ID)
