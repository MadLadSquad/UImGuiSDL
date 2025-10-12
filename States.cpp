#include "UImGuiSDL.hpp"
#include <SDL3/SDL.h>

#include <imgui.h>

void UImGuiSDL::WindowSDL::focusWindow() noexcept
{
    SDL_RaiseWindow(window);
}

bool UImGuiSDL::WindowSDL::getWindowFullscreen() noexcept
{
    return (SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN);
}

bool& UImGuiSDL::WindowSDL::getWindowFullscreenSetting() noexcept
{
    return windowData.fullscreen;
}

void UImGuiSDL::WindowSDL::setWindowFullscreen(const bool bFullscreen) noexcept
{
    SDL_SetWindowFullscreen(window, bFullscreen);
}

bool& UImGuiSDL::WindowSDL::getWindowFocusedSetting() noexcept
{
    return windowData.bFocused;
}

bool UImGuiSDL::WindowSDL::getWindowCurrentlyFocused() noexcept
{
    return (SDL_GetWindowFlags(window) & SDL_WINDOW_INPUT_FOCUS);
}

void UImGuiSDL::WindowSDL::pushWindowFocusCallback(const std::function<void(bool)>& f) noexcept
{
    windowFocusCallbackList.push_back(f);
}

void UImGuiSDL::WindowSDL::iconifyWindow() noexcept
{
    SDL_MinimizeWindow(window);
    bCurrentlyMaximised = false;
    bCurrentlyMinimised = true;
}

void UImGuiSDL::WindowSDL::restoreWindowState() noexcept
{
    SDL_RestoreWindow(window);
    bCurrentlyMaximised = false;
    bCurrentlyMinimised = false;
}

void UImGuiSDL::WindowSDL::pushWindowIconifyCallback(const std::function<void(bool)>& f) noexcept
{
    windowIconifiedCallbackList.push_back(f);
}

bool UImGuiSDL::WindowSDL::getWindowIconified() noexcept
{
    return (SDL_GetWindowFlags(window) & SDL_WINDOW_MINIMIZED);
}

bool UImGuiSDL::WindowSDL::getCurrentWindowDecoratedState() noexcept
{
    return !(SDL_GetWindowFlags(window) & SDL_WINDOW_BORDERLESS);
}

bool& UImGuiSDL::WindowSDL::getWindowDecoratedSetting() noexcept
{
    return windowData.bDecorated;
}

void UImGuiSDL::WindowSDL::setWindowDecorated(const bool bDecorated) noexcept
{
    SDL_SetWindowBordered(window, bDecorated);
}

UImGui::FVector4 UImGuiSDL::WindowSDL::getWindowDecorationFrameDistances() noexcept
{
    int x = 0, y = 0, z = 0, w = 0;
    SDL_GetWindowBordersSize(window, &y, &x, &w, &z);
    const UImGui::FVector4 val = { CAST(float, x), CAST(float, y), CAST(float, z), CAST(float, w) };
    return val;
}

void UImGuiSDL::WindowSDL::pushWindowRefreshCallback(const std::function<void()>& f) noexcept
{
    windowRefreshCallbackList.push_back(f);
}

void UImGuiSDL::WindowSDL::maximiseWindow() noexcept
{
    SDL_MaximizeWindow(window);
    bCurrentlyMaximised = true;
    bCurrentlyMinimised = false;
}

bool& UImGuiSDL::WindowSDL::getWindowMaximisedSetting() noexcept
{
    return windowData.bMaximised;
}

void UImGuiSDL::WindowSDL::pushWindowMaximiseCallback(const std::function<void(bool)>& f) noexcept
{
    windowMaximisedCallbackList.push_back(f);
}

bool UImGuiSDL::WindowSDL::getWindowCurrentlyMaximised() noexcept
{
    return (SDL_GetWindowFlags(window) & SDL_WINDOW_MAXIMIZED);
}

void UImGuiSDL::WindowSDL::requestWindowAttention() noexcept
{
    SDL_FlashWindow(window, SDL_FLASH_UNTIL_FOCUSED);
}

void UImGuiSDL::WindowSDL::hideWindow() noexcept
{
    SDL_HideWindow(window);
}

void UImGuiSDL::WindowSDL::showWindow() noexcept
{
    SDL_ShowWindow(window);
}

bool& UImGuiSDL::WindowSDL::getWindowHiddenSetting() noexcept
{
    return windowData.bHidden;
}

bool UImGuiSDL::WindowSDL::getWindowCurrentlyHidden() noexcept
{
    return (SDL_GetWindowFlags(window) & SDL_WINDOW_HIDDEN);
}

void UImGuiSDL::WindowSDL::saveSettings(const bool bSaveKeybinds) noexcept
{
    GenericWindow::saveSettings(bSaveKeybinds);
}

void UImGuiSDL::WindowSDL::refreshSettings() noexcept
{
    setTitle(windowData.name.c_str());
    setIcon(windowData.iconLocation.c_str());

    setWindowSizeInScreenCoords(windowSizeS);
    setWindowFullscreen(windowData.fullscreen);
    setWindowResizeable(windowData.bResizeable);

    setWindowSurfaceTransparent(windowData.bSurfaceTransparent);
    if (windowData.bSurfaceTransparent)
        SDL_SetWindowOpacity(window, ImGui::GetStyle().Colors[ImGuiCol_WindowBg].w);

    windowData.bHidden ? hideWindow() : showWindow();
    if (windowData.bFocused)
        focusWindow();

    setSizeLimitByAspectRatio(windowData.aspectRatioSizeLimit);
    const auto sizeLimits = windowData.sizeLimits;
    setSizeLimits({ sizeLimits.x, sizeLimits.y }, { sizeLimits.z, sizeLimits.w });

    setWindowDecorated(windowData.bDecorated);
    if (windowData.bMaximised)
        maximiseWindow();
    else
        restoreWindowState();
}

void UImGuiSDL::WindowSDL::close() noexcept
{
    bShouldClose = true;
}

void UImGuiSDL::WindowSDL::pushWindowCloseCallback(const std::function<void()>& f) noexcept
{
    windowCloseCallbackList.push_back(f);
}

bool UImGuiSDL::WindowSDL::getWindowSurfaceTransparent() noexcept
{
    return (SDL_GetWindowFlags(window) & SDL_WINDOW_TRANSPARENT);
}

void UImGuiSDL::WindowSDL::setWindowSurfaceTransparent(const bool bTransparent) noexcept
{
}

bool& UImGuiSDL::WindowSDL::getWindowSurfaceTransparentSetting() noexcept
{
    return windowData.bSurfaceTransparent;
}

void UImGuiSDL::WindowSDL::pushWindowOSDragDropCallback(const std::function<void(const UImGui::FString&)>& f) noexcept
{
    dragDropPathCallbackList.emplace_back(f);
}
