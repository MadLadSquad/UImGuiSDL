#include "UImGuiSDL.hpp"
#include <SDL3/SDL.h>

#ifdef _WIN32
#elif __APPLE__
#elif !defined(__EMSCRIPTEN__)
    #if __has_include(<wayland-client.h>)
    #endif
    #if __has_include(<X11/Xatom.h>)
        #include <X11/Xatom.h>
        #include <X11/extensions/Xrandr.h>
        #define _NET_WM_STATE_ADD 1
    #endif
#endif
#include <Interfaces/RendererInterface.hpp>

UImGui::Monitor UImGuiSDL::WindowSDL::getWindowMonitor() noexcept
{
    UImGui::Monitor m{};
#ifndef __EMSCRIPTEN__
    m.get().id = SDL_GetDisplayForWindow(window);
#endif
    return m;
}

UImGui::Monitor UImGuiSDL::WindowSDL::getPrimaryMonitor() noexcept
{
    UImGui::Monitor m{};
#ifndef __EMSCRIPTEN__
    m.get().id = SDL_GetPrimaryDisplay();
#endif
    return m;
}

const UImGui::TVector<UImGui::Monitor>& UImGuiSDL::WindowSDL::getMonitors() noexcept
{
#ifndef __EMSCRIPTEN__
    int count = 0;
    const auto* displays = SDL_GetDisplays(&count);
    if (displays != nullptr)
    {
        monitors.clear();
        for (int i = 0; i < count; i++)
        {
            auto m = monitors.emplace_back();
            m.get().id = displays[i];
        }
    }
#endif
    return monitors;
}

UImGui::FVector4 UImGuiSDL::MonitorSDL::getWorkArea(UImGui::MonitorData& data) noexcept
{
    int x, y, z, w;
#ifndef __EMSCRIPTEN__
    SDL_Rect rect{};
    SDL_GetDisplayUsableBounds(data.id, &rect);

    x = rect.x;
    y = rect.y;
    z = rect.w;
    w = rect.h;
#endif
    return { CAST(float, x), CAST(float, y), CAST(float, z), CAST(float, w) };
}

double UImGuiSDL::MonitorSDL::getContentScale(UImGui::MonitorData& data) noexcept
{
    double scale = 0.0f;
#ifndef __EMSCRIPTEN__
    scale = SDL_GetDisplayContentScale(data.id);
#endif
    return scale;
}

UImGui::String UImGuiSDL::MonitorSDL::getName(UImGui::MonitorData& data) noexcept
{
#ifndef __EMSCRIPTEN__
    return SDL_GetDisplayName(data.id);
#else
    return "";
#endif
}

UImGui::FVector2 UImGuiSDL::MonitorSDL::getSize(UImGui::MonitorData& data) noexcept
{
    const auto area = getWorkArea(data);
    return { CAST(float, area.z), CAST(float, area.w) };
}

float UImGuiSDL::MonitorSDL::getPixelDensity(UImGui::MonitorData& data) noexcept
{
    return 1.0f;
}

void* UImGuiSDL::MonitorSDL::getPlatformHandle(UImGui::MonitorData& data) noexcept
{
#ifdef __EMSCRIPTEN__
    return (void*)UImGui::Renderer::data().emscriptenCanvas;
#elifdef _WIN32
    return reinterpret_cast<void*>(
               const_cast<char*>(
                   glfwGetWin32Monitor(
                       reinterpret_cast<GLFWmonitor*>(
                           data.id
                       )
                   )
               )
    );
#elifdef __APPLE__
    return reinterpret_cast<void*>(data.id);
#else
    if (UImGui::Window::Platform::getCurrentWindowPlatform() == UIMGUI_WINDOW_PLATFORM_WAYLAND)
    {
        const auto props = SDL_GetDisplayProperties(data.id);
        void* result = nullptr;
        result = SDL_GetPointerProperty(props, SDL_PROP_DISPLAY_WAYLAND_WL_OUTPUT_POINTER, &result);
        if (result != nullptr)
            return result;
    }
    if (UImGui::Window::Platform::getCurrentWindowPlatform() == UIMGUI_WINDOW_PLATFORM_X11)
    {
        const auto props = SDL_GetWindowProperties(static_cast<SDL_Window*>(UImGui::Window::getInternal()));
        void* result = nullptr;
        return SDL_GetPointerProperty(props, SDL_PROP_WINDOW_X11_SCREEN_NUMBER, &result);
    }
    return nullptr;
#endif
}
