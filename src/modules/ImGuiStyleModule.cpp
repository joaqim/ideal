#include "../utils/corrade/AbstractModule.h"
#include "../utils/imgui/ImGuiStyle.hpp"
#include <Corrade/Containers/Tags.h>
#include <Corrade/PluginManager/AbstractManager.h>
#include <Corrade/Utility/Debug.h>

#include <memory>

#include <imgui.h>

struct ImGuiStyleState {
  ImGuiStyle *style;// = ::ImGui::GetStyle();
};

class ImGuiStyleModule : public AbstractModule {
public:
  using Debug = Corrade::Utility::Debug;

  ImGuiStyleModule(Corrade::PluginManager::AbstractManager &manager,
                   const std::string &plugin)
      : AbstractModule(manager, plugin) {}

        static std::string pluginInterface() {
          return MODULE_PLUGIN_ID;
        }

        virtual void load(Corrade::Containers::Array<void *> && previousState) override {
          Debug{} << "ImGuiStyleModule::load()";

          if(previousState.empty()) {
            Debug{} << "Initializing state";
            _state.reset(new ImGuiStyleState);
            _state->style = &::ImGui::GetStyle();
            Utils::ImGui::setImGuiStyle(1.142f);
          } else {
            _state.reset(static_cast<ImGuiStyleState *>(*previousState.release()));
            Utils::ImGui::setImGuiStyle(1.142f);
          }
        }

        virtual Corrade::Containers::Array<void *> && unload(bool shutdown) override {
          Debug{} << "ImGuiStyleModule::unload(" << shutdown << ")";

          ImGuiStyleState* state = _state.release();
          Debug{} << "Sizeof state:" << sizeof(ImGuiStyleState);

          return std::move(Corrade::Containers::Array<void *>{Corrade::Containers::InPlaceInit, {static_cast<void *>(state)}});

        }

    private:
        std::unique_ptr<ImGuiStyleState> _state;

};

CORRADE_PLUGIN_REGISTER(ImGuiStyleModule, ImGuiStyleModule, MODULE_PLUGIN_ID);
