#include "IdealApplication.h"
#include "Corrade/Utility/Directory.h"

#include <Corrade/Utility/Resource.h>
#include <Corrade/Utility/System.h>
#include <Magnum/FileCallback.h>
#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/GL/Renderer.h>
#include <Magnum/ImGuiIntegration/Context.hpp>

#include <imgui.h>

#include <filesystem>

namespace Ideal {
/*  */
/*   std::string getLibraryDir(){ */
/*     auto currentDir = Directory::current(); */
/*     //if(Directory::exists(Directory::path(currentDir + "./lib"))) { */
/*     if(Directory::exists("./lib")) { */
/*       //return currentDir + "/lib/"; */
/*       return "./lib/"; */
/*     } */
/*     return "../lib/"; */
/*     //return std::filesystem::path(currentDir).remove_filename().append("../lib/").string(); */
/* } */

IdealApplication::IdealApplication(const Arguments &arguments)
    : Platform::Application{arguments,
                            Configuration{}
                                .setTitle("Magnum ImGui Example")
                                .setWindowFlags(
                                    Configuration::WindowFlag::Resizable)},
      _cache{Vector2i{2048}, Vector2i{384}, 18} {
      /* _watcher{Directory::path(getLibraryDir() + */
                               /* "ImGuiStyleModule.so")} { */

  /* const auto libDir = getLibraryDir(); */
  /* Debug{} << "LibDir:" << libDir.c_str(); */
  /* Directory::write(libDir+"ImGuiStyleModule_Runtime.so", Directory::read(libDir+"ImGuiStyleModule.so")); */
#if 0
  /* Test Module */
  std::unique_ptr<AbstractModule> module;
  /* Copy DLL so that the original can be overwritten with never version */
  Directory::write("lib/ImGuiStyleModule_Runtime.so", Directory::read("lib/ImGuiStyleModule.so"));
  /* First load of the copied plugin */
  if (!(_moduleManager.load("ImGuiStyleModule") &
        PluginManager::LoadState::Loaded)) {
    Error{} << "ImGuiStyleModule can not be loaded.";
  }
#endif

  const auto size = Vector2{windowSize()} / dpiScaling();

  _imgui = ImGuiIntegration::Context(size, windowSize(), framebufferSize());

  const float dpiScaleFactor = framebufferSize().x() / size.x();
  Debug() << "DPI: " << dpiScaleFactor << "\n";
  // Utils::ImGui::setImGuiStyle(dpiScaleFactor);
 
  _moduleLoader.load("ImGuiStyleModule_Runtime");
  /* _moduleLoader.load("ImGuiStyleModule"); */
  //_moduleLoader.unload("ImGuiStyleModule");

  //_module = _moduleManager.instantiate("ImGuiStyleModule");
  //_module->load();
  //module->unload();

  Utility::Resource rs{"assets"};
  Containers::ArrayView<const char> font =
      rs.getRaw("SourceSansPro-Regular.ttf");
  /* Containers::ArrayView<const char> font = rs.getRaw("verdana.ttf");  */

  const float font_size = 18.f * dpiScaleFactor;

  _font = _manager.loadAndInstantiate("StbTrueTypeFont");
#if 1
  struct Data {
    std::unordered_map<std::string,
                       Containers::Array<const char, Directory::MapDeleter>>
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
          found =
              data.files.emplace(filename, Directory::mapRead(filename)).first;

        return Containers::arrayView(found->second);
      },
      data);

  //_font->openFile("assets/fonts.conf", 18.f);
#endif
  if (!_font || !_font->openData(font, font_size * 10.f))
    Fatal{} << "Cannot open font file";

  Debug() << "\tBuilding Glyph Cache\n";
  /* Glyphs we need to render everything */
  _font->fillGlyphCache(
      _cache, "abcdefghijklmnopqrstuvwxyz"
              "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
              " ."
      //      "0123456789;:-+,.!? " /* "~$%&[{}(=*)+]#`'<>@^|_" */
  );

  _dynamicText.reset(
      new Text::Renderer2D(*_font, _cache, 32.0f, Text::Alignment::TopRight));
  _dynamicText->reserve(40, GL::BufferUsage::DynamicDraw,
                        GL::BufferUsage::StaticDraw);
  _dynamicText->render("This is the text rendered with Magnum");

  Debug() << "\tFinished loading font\n";

  _transformationProjection =
      Matrix3::projection(size) * Matrix3::translation(size * 0.5f);

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
#elif 1

  GL::Renderer::enable(GL::Renderer::Feature::Blending);
  GL::Renderer::enable(GL::Renderer::Feature::ScissorTest);
  GL::Renderer::setBlendEquation(GL::Renderer::BlendEquation::Add,
                                 GL::Renderer::BlendEquation::Add);
  GL::Renderer::setBlendFunction(
      GL::Renderer::BlendFunction::SourceAlpha,
      GL::Renderer::BlendFunction::OneMinusSourceAlpha);
#else
  /* Set up premultiplied alpha blending to avoid overlapping text characters
     to cut into each other */
  GL::Renderer::enable(GL::Renderer::Feature::Blending);
  GL::Renderer::setBlendFunction(
      GL::Renderer::BlendFunction::One,
      GL::Renderer::BlendFunction::OneMinusSourceAlpha);
  GL::Renderer::setBlendEquation(GL::Renderer::BlendEquation::Add,
                                 GL::Renderer::BlendEquation::Add);
#endif

#if !defined(MAGNUM_TARGET_WEBGL) && !defined(CORRADE_TARGET_ANDROID)
  /* Have some sane speed, please */
  setMinimalLoopPeriod(16);
#endif
  _timeline.start();
}

void IdealApplication::drawEvent() {

  _moduleLoader.update();

#if 0
  if (_watcher.hasChanged()) {
    /* ::System::sleep(1000); */
    /* Tell module that it is being unloaded and retrieve its state for
     * state transfer, if supported. */
    // auto state{module->unload(false)};
    _module->unload();
    /* Remove references to the plugin, otherwise we cannot unload it */
    _module.reset(nullptr);

    if (_moduleManager.unload("ImGuiStyleModule") &
        PluginManager::LoadState::UnloadFailed) {
      Error{} << "ImGuiStyleModule failed to unload.";
    }

    Directory::write("lib/ImGuiStyleModule_Runtime.so",
                     Directory::read("lib/ImGuiStyleModule.so"));
    System::sleep(500);

    if (!(_moduleManager.load("ImGuiStyleModule") &
          PluginManager::LoadState::Loaded)) {
      Error{} << "ImGuiStyleModule can not be loaded.";
    }

    _module = _moduleManager.instantiate("ImGuiStyleModule");
    _module->load();
    // module->load(std::move(state));
  }
#endif

  GL::defaultFramebuffer.clear(GL::FramebufferClear::Color);
#if 1

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

  /* _imgui.drawFrame(); */

  /* Reset state. Only needed if you want to draw something else with
     different state after. */
  /* GL::Renderer::enable(GL::Renderer::Feature::DepthTest); */
  /* GL::Renderer::enable(GL::Renderer::Feature::FaceCulling); */
  /* GL::Renderer::disable(GL::Renderer::Feature::ScissorTest); */
  /* GL::Renderer::disable(GL::Renderer::Feature::Blending); */

#endif

  _shader.bindVectorTexture(_cache.texture());
  _shader.setColor(0xffffff_rgbf)
      .setTransformationProjectionMatrix(_transformationProjection)
      .setOutlineRange(0.5f, 1.0f)
      .setSmoothness(0.075f)
      .draw(_dynamicText->mesh());

  _imgui.drawFrame();

  swapBuffers();
  redraw();
  _timeline.nextFrame();
}

void IdealApplication::viewportEvent(ViewportEvent &event) {
  GL::defaultFramebuffer.setViewport({{}, event.framebufferSize()});

  _imgui.relayout(Vector2{event.windowSize()} / event.dpiScaling(),
                  event.windowSize(), event.framebufferSize());
  _transformationProjection =
      Matrix3::projection(Vector2{windowSize()}) *
      Matrix3::translation(Vector2{windowSize()} * 0.5f);
}

void IdealApplication::keyPressEvent(KeyEvent &event) {
  if (_imgui.handleKeyPressEvent(event))
    return;
}

void IdealApplication::keyReleaseEvent(KeyEvent &event) {
  if (_imgui.handleKeyReleaseEvent(event))
    return;
}

void IdealApplication::mousePressEvent(MouseEvent &event) {
  if (_imgui.handleMousePressEvent(event))
    return;
}

void IdealApplication::mouseReleaseEvent(MouseEvent &event) {
  if (_imgui.handleMouseReleaseEvent(event))
    return;
}

void IdealApplication::mouseMoveEvent(MouseMoveEvent &event) {
  if (_imgui.handleMouseMoveEvent(event))
    return;
}

void IdealApplication::mouseScrollEvent(MouseScrollEvent &event) {
  if (_imgui.handleMouseScrollEvent(event)) {
    /* Prevent scrolling the page */
    event.setAccepted();
    return;
  }
}

void IdealApplication::textInputEvent(TextInputEvent &event) {
  if (_imgui.handleTextInputEvent(event))
    return;
}
} // namespace Ideal

MAGNUM_APPLICATION_MAIN(Ideal::IdealApplication)
