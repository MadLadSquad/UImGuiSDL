#include "UImGuiSDL.hpp"
#include <SDL3/SDL.h>

#include <Core/Components/Instance.hpp>
#include <Renderer/RendererUtils.hpp>
#include <Renderer/GenericRenderer/GenericRenderer.hpp>

#include <ThirdParty/source-libraries/stb_image.h>

UImGuiSDL::WindowSDL::WindowSDL() noexcept
{
    windowFlags = SDL_WINDOW_HIGH_PIXEL_DENSITY;
    monitor = dynamic_cast<UImGui::GenericMonitor*>(&sdlMonitor);
    // TODO: Implement
    //keys.fill(KeyStateReleased);
}

// Hack needed because SDL3 does not send a window resize event when the window gets launched with a size different than
// the initial canvas size
#ifdef __EMSCRIPTEN__
EM_JS(int, get_bounding_client_rect, (
  const char* selector,
  double* x, double* y, double* width, double* height,
  double* left, double* top, double* right, double* bottom
), {
  const sel = UTF8ToString(selector);
  const el = document.querySelector(sel);
  if (!el) return 0;
  const r = el.getBoundingClientRect();
  setValue(x,      r.x,      'double');
  setValue(y,      r.y,      'double');
  setValue(width,  r.width,  'double');
  setValue(height, r.height, 'double');
  setValue(left,   r.left,   'double');
  setValue(top,    r.top,    'double');
  setValue(right,  r.right,  'double');
  setValue(bottom, r.bottom, 'double');
  return 1;
});

EM_JS(void, set_element_clip_rect_viewport, (const char* selector, int x, int y, int w, int h), {
  const sel = UTF8ToString(selector);
  const el  = document.querySelector(sel);
  if (!el) return;

  // Create wrapper once
  if (!el.__clipWrapper) {
    const wrap = document.createElement('div');
    wrap.style.position = 'fixed';
    wrap.style.overflow = 'hidden';
    wrap.style.pointerEvents = getComputedStyle(el).pointerEvents;
    el.__clipWrapper = wrap;
    // Move el inside wrapper, and ensure el is positioned relative to wrapper
    const p = el.parentNode;
    p && p.replaceChild(wrap, el);
    wrap.appendChild(el);
    el.style.position = 'absolute';
    // Keep top-left of el aligned to wrapper by default
    el.style.left = '0px';
    el.style.top  = '0px';
  }
  const wrap = el.__clipWrapper;
  wrap.style.left   = x + 'px';
  wrap.style.top    = y + 'px';
  wrap.style.width  = w + 'px';
  wrap.style.height = h + 'px';
});

#endif

void UImGuiSDL::WindowSDL::updateEmscriptenRect() noexcept
{
#ifdef __EMSCRIPTEN__
    double x,y,w,h,l,t,r,b;
    if (get_bounding_client_rect(UImGui::Renderer::data().emscriptenCanvas, &x,&y,&w,&h,&l,&t,&r,&b)) {
        set_element_clip_rect_viewport(UImGui::Renderer::data().emscriptenCanvas, x, y, w, h);
        SDL_SetWindowSize(window, w, h);
        // numbers are in CSS pixels, relative to the viewport
    }
#endif
}

void UImGuiSDL::WindowSDL::createWindow() noexcept
{
    // Load all config we need
    openConfig();

    // Init SDL
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        Logger::log("SDL initialisation failed! Error: ", ULOG_LOG_TYPE_ERROR, SDL_GetError());
        return;
    }
    Logger::log("Setting up the window", ULOG_LOG_TYPE_NOTE);

    // This is used to set up window hints for the renderer. Read up on creating custom renderers.
    UImGui::RendererUtils::getRenderer()->setupWindowIntegration();

    SDL_SetHint(SDL_HINT_APP_ID, UImGui::Instance::get()->applicationName.c_str());
    SDL_SetHint(SDL_HINT_APP_NAME, UImGui::Instance::get()->applicationName.c_str());
    SDL_SetHint(SDL_HINT_VIDEO_WAYLAND_ALLOW_LIBDECOR, "1");
    SDL_SetHint(SDL_HINT_VIDEO_WAYLAND_PREFER_LIBDECOR, "1");
    SDL_SetHint(SDL_HINT_EMSCRIPTEN_CANVAS_SELECTOR, UImGui::Renderer::data().emscriptenCanvas);

    Logger::log("Window settings configured", ULOG_LOG_TYPE_NOTE);

    if (windowData.fullscreen)
        windowFlags |= SDL_WINDOW_FULLSCREEN;
    if (windowData.bResizeable)
        windowFlags |= SDL_WINDOW_RESIZABLE;
    if (windowData.bSurfaceTransparent)
        windowFlags |= SDL_WINDOW_TRANSPARENT;
    if (windowData.bHidden)
        windowFlags |= SDL_WINDOW_HIDDEN;
    if (windowData.bFocused)
        windowFlags |= SDL_WINDOW_INPUT_FOCUS;
    if (!windowData.bDecorated)
        windowFlags |= SDL_WINDOW_BORDERLESS;
    if (windowData.bMaximised)
        windowFlags |= SDL_WINDOW_MAXIMIZED;

    const float scale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());

    window = SDL_CreateWindow(
        windowData.name.c_str(),
        static_cast<int>(windowSizeS.x * scale),
        static_cast<int>(windowSizeS.y * scale),
        windowFlags
    );
    if (!window)
    {
        Logger::log("SDL window creation failed! Error: ", ULOG_LOG_TYPE_ERROR, SDL_GetError());
        SDL_Quit();
        return;
    }
    Logger::log("Created window", ULOG_LOG_TYPE_NOTE);

    // Load window icon
    setIcon((UImGui::Instance::get()->initInfo.contentDir + windowData.iconLocation).c_str());
    setSizeLimits({ windowData.sizeLimits.x, windowData.sizeLimits.y }, { windowData.sizeLimits.z, windowData.sizeLimits.w });
    setSizeLimitByAspectRatio(windowData.aspectRatioSizeLimit);
    SDL_SyncWindow(window);

    updateEmscriptenRect();

    Logger::log("Window was created successfully", ULOG_LOG_TYPE_SUCCESS);
    UImGui::RendererUtils::getRenderer()->setupPostWindowCreation();
}

void UImGuiSDL::WindowSDL::destroyWindow() noexcept
{
    // We need to manually clean up with SDL
    if (windowFlags & SDL_WINDOW_OPENGL)
        OpenGL_destroyContext(OpenGL_getCurrentContext());

    SDL_DestroyWindow(window);
    SDL_Quit();
}

void UImGuiSDL::WindowSDL::setTitle(const UImGui::String name) noexcept
{
    SDL_SetWindowTitle(window, name);
}

UImGui::String UImGuiSDL::WindowSDL::getTitle() noexcept
{
    return SDL_GetWindowTitle(window);
}

void UImGuiSDL::WindowSDL::setTitleSetting(const UImGui::String name) noexcept
{
    windowData.name = name;
}

UImGui::String UImGuiSDL::WindowSDL::getTitleSetting() noexcept
{
    return windowData.name.c_str();
}

void UImGuiSDL::WindowSDL::setIcon(const UImGui::String name) noexcept
{
    int width = 0;
    int height = 0;

    const auto pixels = stbi_load(name, &width, &height, nullptr, 4);
    if (pixels != nullptr)
    {
        SDL_Surface* surface = SDL_CreateSurfaceFrom(width, height, SDL_PIXELFORMAT_RGBA32, pixels, 4 * width);
        if (surface != nullptr)
        {
            currentIconLocation = name;
            if (!SDL_SetWindowIcon(window, surface))
                Logger::log("Couldn't set the window icon at location: ", ULOG_LOG_TYPE_WARNING, name, "... Error: ", SDL_GetError());
            SDL_DestroySurface(surface);
        }
        else
            Logger::log("Couldn't create the surface for the window icon at location: ", ULOG_LOG_TYPE_WARNING, name, "... Error: ", SDL_GetError());

        stbi_image_free(pixels);
    }
    else
        Logger::log("Couldn't load the window icon at location: ", ULOG_LOG_TYPE_WARNING, name, "... Error: ", SDL_GetError());
}

UImGui::String UImGuiSDL::WindowSDL::getIconLocation() noexcept
{
    return currentIconLocation.c_str();
}

UImGui::String UImGuiSDL::WindowSDL::getIconLocationSetting() noexcept
{
    return windowData.iconLocation.c_str();
}

void UImGuiSDL::WindowSDL::setIconLocationSetting(const UImGui::String location) noexcept
{
    windowData.iconLocation = location;
}


void* UImGuiSDL::WindowSDL::getInternal() noexcept
{
    return window;
}

UImGui::GenericWindow* UImGuiSDL::WindowSDL::get() noexcept
{
    return this;
}
