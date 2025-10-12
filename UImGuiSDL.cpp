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

    window = SDL_CreateWindow(windowData.name.c_str(), static_cast<int>(windowSizeS.x), static_cast<int>(windowSizeS.y), windowFlags);
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

    Logger::log("Window was created successfully", ULOG_LOG_TYPE_SUCCESS);

    // Set framebuffer size
    int x = 0, y = 0;
    SDL_GetWindowSizeInPixels(window, &x, &y);
    windowSizeS.x = x;
    windowSizeS.y = y;

    for (auto& a : windowResizeCallbackList)
        a(x, y);

    SDL_GetWindowSize(window, &x, &y);
    windowSizeInScreenCoords.x = x;
    windowSizeInScreenCoords.y = y;
    for (auto& a : windowResizeInScreenCoordCallbackList)
        a(x, y);

    UImGui::RendererUtils::getRenderer()->setupPostWindowCreation();

    SDL_SetWindowSize(window, x + 1, y + 1);
    SDL_SyncWindow(window);
    SDL_SetWindowSize(window, x - 1, y - 1);
    SDL_SyncWindow(window);
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
