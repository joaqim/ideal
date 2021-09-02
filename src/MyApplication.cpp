#include "utils/corrade/AbstractModule.h"
#include "utils/imgui/ImGuiStyle.hpp"

#include <Magnum/FileCallback.h>
#include <Magnum/Text/AbstractFont.h>
#include <Magnum/Text/DistanceFieldGlyphCache.h>
#include <Magnum/Text/Renderer.h>

#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/GL.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/ImGuiIntegration/Context.hpp>
#include <Magnum/Math/Color.h>

#include <Corrade/Containers/Array.h>
#include <Corrade/PluginManager/Manager.h>
#include <Corrade/Utility/Debug.h>
#include <Corrade/Utility/DebugStl.h>
#include <Corrade/Utility/Directory.h>
#include <Corrade/Utility/FileWatcher.h>
#include <Corrade/Utility/FormatStl.h>
#include <Corrade/Utility/Resource.h>
#include <Corrade/Utility/System.h>

#include <imgui.h>

#ifdef CORRADE_TARGET_ANDROID
#include <Magnum/Platform/AndroidApplication.h>
#elif defined(CORRADE_TARGET_EMSCRIPTEN)
#include <Magnum/Platform/EmscriptenApplication.h>
#else
#include <Magnum/Platform/Sdl2Application.h>
#endif

namespace Magnum {
namespace Examples {

using namespace Math::Literals;

class ImGuiExample : public Platform::Application {
public:
  explicit ImGuiExample(const Arguments &arguments);

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
  PluginManager::Manager<AbstractModule> _moduleManager{Corrade::Utility::Directory::path(Corrade::Utility::Directory::current() + "/modules/")};

  Text::DistanceFieldGlyphCache _cache;

  Corrade::Utility::FileWatcher _watcher;

  bool _showDemoWindow = true;
  bool _showAnotherWindow = false;
  Color4 _clearColor = 0x72909aff_rgbaf;
  Float _floatValue = 0.0f;
};

ImGuiExample::ImGuiExample(const Arguments &arguments)
    : Platform::Application{arguments,
                            Configuration{}
                                .setTitle("Magnum ImGui Example - float")
                                .setWindowFlags(
                                    Configuration::WindowFlag::Resizable)},
      _cache{Vector2i{2048}, Vector2i{384}, 18},
      _watcher{Corrade::Utility::Directory::path(
          Corrade::Utility::Directory::current() +
          "/modules/ImGuiStyleModule.so")} {

  /* Test Module */
  using namespace Corrade::Utility;
  /* Set the plugin manager to load plugins from same directory as this app */
  /* PluginManager::Manager<AbstractModule> manager{ Directory::path(Directory::current() + "/modules/")}; */
  std::unique_ptr<AbstractModule> module;
  /* Copy DLL so that the original can be overwritten with never version */
  Directory::write("modules/ImGuiStyleModule_Runtime.so", Directory::read("modules/ImGuiStyleModule.so"));
  /* First load of the copied plugin */

  if (!(_moduleManager.load("ImGuiStyleModule") & PluginManager::LoadState::Loaded)) {
    Error{} << "ImGuiStyleModule can not be loaded.";
  }

  const auto size = Vector2{windowSize()} / dpiScaling();

  _imgui = ImGuiIntegration::Context(size, windowSize(), framebufferSize());

  const float dpiScaleFactor = framebufferSize().x() / size.x();
  Corrade::Utility::Debug() << "DPI: " << dpiScaleFactor << "\n";
  // Utils::ImGui::setImGuiStyle(dpiScaleFactor);

  module = _moduleManager.instantiate("ImGuiStyleModule");
  module->load();
  /* module->unload(); */

  Utility::Resource rs{"assets"};
  Containers::ArrayView<const char> font =
    rs.getRaw("SourceSansPro-Regular.ttf");
  /* Containers::ArrayView<const char> font = rs.getRaw("verdana.ttf");  */

  const float font_size = 18.f * dpiScaleFactor;

  _font = _manager.loadAndInstantiate("StbTrueTypeFont");
#if 1
  struct Data {
    std::unordered_map<
      std::string,
      Containers::Array<const char, Utility::Directory::MapDeleter>>
        files;
  } data;

  _font->setFileCallback(
      [](const std::string &filename, InputFileCallbackPolicy policy,
        Data &data)
      -> Containers::Optional<Containers::ArrayView<const char>> {
      auto found = data.files.find(filename);

      /* Discard the memory mapping, if not needed anymore */
      if (policy == InputFileCallbackPolicy::Close) {
      if (found != data.files.end())
      data.files.erase(found);
      return {};
      }

      /* Load if not there yet */
      if (found == data.files.end())
      found = data.files
      .emplace(filename, Utility::Directory::mapRead(filename))
      .first;

      return Containers::arrayView(found->second);
      },
    data);

  //_font->openFile("assets/fonts.conf", 18.f);
  /* #else */
  if (!_font || !_font->openData(font, font_size))
    Fatal{} << "Cannot open font file";
#endif

  Corrade::Utility::Debug() << "\tBuilding Glyph Cache\n";
  /* Glyphs we need to render everything */
  _font->fillGlyphCache(
      _cache, "abcdefghijklmnopqrstuvwxyz"
      "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
      //      "0123456789;:-+,.!? " /* "~$%&[{}(=*)+]#`'<>@^|_" */
      );

  Corrade::Utility::Debug() << "\tFinished loading font\n";

  ImFontConfig fontConfig;
  fontConfig.FontDataOwnedByAtlas = false;
  ImGui::GetIO().Fonts->AddFontFromMemoryTTF(
      const_cast<char *>(font.data()), font.size(), font_size, &fontConfig);

  /* Set up proper blending to be used by ImGui. There's a great chance
     you'll need this exact behavior for the rest of your scene. If not, set
     this only for the drawFrame() call. */
#if 0
  GL::Renderer::setBlendEquation(GL::Renderer::BlendEquation::Add,
      GL::Renderer::BlendEquation::Add);
  GL::Renderer::setBlendFunction(
      GL::Renderer::BlendFunction::SourceAlpha,
      GL::Renderer::BlendFunction::OneMinusSourceAlpha);
#else

  GL::Renderer::enable(GL::Renderer::Feature::Blending);
  GL::Renderer::enable(GL::Renderer::Feature::ScissorTest);
  GL::Renderer::setBlendEquation(GL::Renderer::BlendEquation::Add,
      GL::Renderer::BlendEquation::Add);
  GL::Renderer::setBlendFunction(
      GL::Renderer::BlendFunction::SourceAlpha,
      GL::Renderer::BlendFunction::OneMinusSourceAlpha);
#endif

  /* Set up premultiplied alpha blending to avoid overlapping text characters
     to cut into each other */
  /* GL::Renderer::enable(GL::Renderer::Feature::Blending); */
  /* GL::Renderer::setBlendFunction(GL::Renderer::BlendFunction::One,
   * GL::Renderer::BlendFunction::OneMinusSourceAlpha); */
  /* GL::Renderer::setBlendEquation(GL::Renderer::BlendEquation::Add,
   * GL::Renderer::BlendEquation::Add); */

#if !defined(MAGNUM_TARGET_WEBGL) && !defined(CORRADE_TARGET_ANDROID)
  /* Have some sane speed, please */
  setMinimalLoopPeriod(16);
#endif
      }

void ImGuiExample::drawEvent() {

  std::unique_ptr<AbstractModule> module;
  if(_watcher.hasChanged()) {
    /* Corrade::Utility::System::sleep(1000); */
    /* Tell module that it is being unloaded and retrieve its state for
     * state transfer, if supported. */
    //auto state{module->unload(false)};
    /* Remove references to the plugin, otherwise we cannot unload it */
    module.reset(nullptr);

    if (_moduleManager.unload("ImGuiStyleModule") & PluginManager::LoadState::UnloadFailed) {
      Error{} << "ImGuiStyleModule failed to unload.";
    }

    Corrade::Utility::Directory::write("modules/ImGuiStyleModule_Runtime.so", Corrade::Utility::Directory::read("modules/ImGuiStyleModule.so"));
    Corrade::Utility::System::sleep(500);

    if (!(_moduleManager.load("ImGuiStyleModule") & PluginManager::LoadState::Loaded)) {
      Error{} << "ImGuiStyleModule can not be loaded.";
    }

    module = _moduleManager.instantiate("ImGuiStyleModule");
    module->load();
    //module->load(std::move(state));

  }


  GL::defaultFramebuffer.clear(GL::FramebufferClear::Color);

  _imgui.newFrame();

  /* Enable text input, if needed */
  if (ImGui::GetIO().WantTextInput && !isTextInputActive())
    startTextInput();
  else if (!ImGui::GetIO().WantTextInput && isTextInputActive())
    stopTextInput();

  /* 1. Show a simple window.
Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets appear in
a window called "Debug" automatically */
  {
    ImGui::Text("Hello, world!");
    ImGui::SliderFloat("Float", &_floatValue, 0.0f, 1.0f);
    if (ImGui::ColorEdit3("Clear Color", _clearColor.data()))
      GL::Renderer::setClearColor(_clearColor);
    if (ImGui::Button("Test Window"))
      _showDemoWindow ^= true;
    if (ImGui::Button("Another Window"))
      _showAnotherWindow ^= true;
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
        1000.0 / Double(ImGui::GetIO().Framerate),
        Double(ImGui::GetIO().Framerate));
  }

  /* 2. Show another simple window, now using an explicit Begin/End pair */
  if (_showAnotherWindow) {
    ImGui::SetNextWindowSize(ImVec2(500, 100), ImGuiCond_FirstUseEver);
    ImGui::Begin("Another Window", &_showAnotherWindow);
    ImGui::Text("Hello");
    ImGui::End();
  }

  /* 3. Show the ImGui demo window. Most of the sample code is in
     ImGui::ShowDemoWindow() */
  if (_showDemoWindow) {
    ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver);
    ImGui::ShowDemoWindow();
  }

  /* Update application cursor */
  _imgui.updateApplicationCursor(*this);

  /* Set appropriate states. If you only draw ImGui, it is sufficient to
     just enable blending and scissor test in the constructor. */
  GL::Renderer::enable(GL::Renderer::Feature::Blending);
  GL::Renderer::enable(GL::Renderer::Feature::ScissorTest);
  GL::Renderer::disable(GL::Renderer::Feature::FaceCulling);
  GL::Renderer::disable(GL::Renderer::Feature::DepthTest);

  _imgui.drawFrame();

  /* Reset state. Only needed if you want to draw something else with
     different state after. */
  GL::Renderer::enable(GL::Renderer::Feature::DepthTest);
  GL::Renderer::enable(GL::Renderer::Feature::FaceCulling);
  GL::Renderer::disable(GL::Renderer::Feature::ScissorTest);
  GL::Renderer::disable(GL::Renderer::Feature::Blending);

  swapBuffers();
  redraw();
}

void ImGuiExample::viewportEvent(ViewportEvent &event) {
  GL::defaultFramebuffer.setViewport({{}, event.framebufferSize()});

  _imgui.relayout(Vector2{event.windowSize()} / event.dpiScaling(),
      event.windowSize(), event.framebufferSize());
}

void ImGuiExample::keyPressEvent(KeyEvent &event) {
  if (_imgui.handleKeyPressEvent(event))
    return;
}

void ImGuiExample::keyReleaseEvent(KeyEvent &event) {
  if (_imgui.handleKeyReleaseEvent(event))
    return;
}

void ImGuiExample::mousePressEvent(MouseEvent &event) {
  if (_imgui.handleMousePressEvent(event))
    return;
}

void ImGuiExample::mouseReleaseEvent(MouseEvent &event) {
  if (_imgui.handleMouseReleaseEvent(event))
    return;
}

void ImGuiExample::mouseMoveEvent(MouseMoveEvent &event) {
  if (_imgui.handleMouseMoveEvent(event))
    return;
}

void ImGuiExample::mouseScrollEvent(MouseScrollEvent &event) {
  if (_imgui.handleMouseScrollEvent(event)) {
    /* Prevent scrolling the page */
    event.setAccepted();
    return;
  }
}

void ImGuiExample::textInputEvent(TextInputEvent &event) {
  if (_imgui.handleTextInputEvent(event))
    return;
}

} // namespace Examples
} // namespace Magnum

MAGNUM_APPLICATION_MAIN(Magnum::Examples::ImGuiExample)
