#pragma once
#include "../Types.h"
#include "../utils/corrade/AbstractModule.h"

#include <Corrade/PluginManager/Manager.h>
#include <Corrade/Utility/Directory.h>
#include <Corrade/Utility/FileWatcher.h>

#include <Magnum/Text/AbstractFont.h>
#include <Magnum/Text/DistanceFieldGlyphCache.h>
#include <Magnum/Text/Renderer.h>
#include <Magnum/Shaders/DistanceFieldVector.h>
#include <Magnum/Timeline.h>

#include <Magnum/ImGuiIntegration/Context.h>

#ifdef CORRADE_TARGET_ANDROID
#include <Magnum/Platform/AndroidApplication.h>
#elif defined(CORRADE_TARGET_EMSCRIPTEN)
#include <Magnum/Platform/EmscriptenApplication.h>
#else
#include <Magnum/Platform/Sdl2Application.h>
#endif

namespace Ideal {

using namespace Magnum;
using namespace Magnum::Math::Literals;
using namespace Corrade;
using namespace Corrade::Utility;

class IdealApplication : public Platform::Application {
public:
  explicit IdealApplication(const Arguments &arguments);

  void drawEvent() override;

  void viewportEvent(ViewportEvent &event) override;

  void keyPressEvent(KeyEvent &event) override;
  void keyReleaseEvent(KeyEvent &event) override;

  void mousePressEvent(MouseEvent &event) override;
  void mouseReleaseEvent(MouseEvent &event) override;
  void mouseMoveEvent(MouseMoveEvent &event) override;
  void mouseScrollEvent(MouseScrollEvent &event) override;
  void textInputEvent(TextInputEvent &event) override;

private:
  ImGuiIntegration::Context _imgui{NoCreate};

  PluginManager::Manager<Text::AbstractFont> _manager;
  Containers::Pointer<Text::AbstractFont> _font;
  PluginManager::Manager<AbstractModule> _moduleManager{
      Directory::path(Directory::current() + "/lib/")};

  std::unique_ptr<AbstractModule> _module;

  Matrix3 _transformationProjection;

  Shaders::DistanceFieldVector2D _shader;
  Containers::Pointer<Text::Renderer2D> _dynamicText;
  Text::DistanceFieldGlyphCache _cache;

  FileWatcher _watcher;

  Magnum::Timeline _timeline;


  bool _showDemoWindow = true;
  bool _showAnotherWindow = false;
  Color4 _clearColor = 0x72909aff_rgbaf;
  Float _floatValue = 0.0f;
};
} // namespace Ideal
