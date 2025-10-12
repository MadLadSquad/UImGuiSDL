#include "UImGuiSDL.hpp"
#include <SDL3/SDL.h>

float UImGuiSDL::WindowSDL::getAspectRatio() noexcept
{
    return windowSizeS.x / windowSizeS.y;
}

UImGui::FVector2 UImGuiSDL::WindowSDL::getCurrentWindowPosition() noexcept
{
    return windowCurrentPos;
}

UImGui::FVector2 UImGuiSDL::WindowSDL::getLastWindowPosition() noexcept
{
    return windowLastPos;
}

UImGui::FVector2 UImGuiSDL::WindowSDL::getWindowPositionChange() noexcept
{
    return
    {
        windowCurrentPos.x - windowLastPos.x,
        windowCurrentPos.y - windowLastPos.y
    };
}

void UImGuiSDL::WindowSDL::setCurrentWindowPosition(const UImGui::FVector2 pos) noexcept
{
    windowCurrentPos = pos;
    SDL_SetWindowPosition(window, CAST(int, pos.x), CAST(int, pos.y));
}

void UImGuiSDL::WindowSDL::pushWindowPositionChangeCallback(const std::function<void(UImGui::FVector2)>& f) noexcept
{
    windowPositionChangeCallbackList.push_back(f);
}

UImGui::FVector2 UImGuiSDL::WindowSDL::getWindowSize() noexcept
{
    return windowSizeS;
}

UImGui::FVector2& UImGuiSDL::WindowSDL::getWindowSizeSetting() noexcept
{
    return windowSizeS;
}

void UImGuiSDL::WindowSDL::setWindowSizeInScreenCoords(const UImGui::FVector2 sz) noexcept
{
    windowSizeInScreenCoords = sz;
    SDL_SetWindowSize(window, CAST(int, sz.x), CAST(int, sz.y));
}

UImGui::FVector2 UImGuiSDL::WindowSDL::getWindowSizeInScreenCoords() noexcept
{
    return windowSizeInScreenCoords;
}

void UImGuiSDL::WindowSDL::pushWindowResizedInScreenCoordsCallback(const std::function<void(int, int)>& f) noexcept
{
    windowResizeInScreenCoordCallbackList.push_back(f);
}

void UImGuiSDL::WindowSDL::setWindowResizeable(const bool bResizeable) noexcept
{
    SDL_SetWindowResizable(window, bResizeable);
}

bool& UImGuiSDL::WindowSDL::getWindowResizeableSetting() noexcept
{
    return windowData.bResizeable;
}

bool UImGuiSDL::WindowSDL::getWindowCurrentlyResizeable() noexcept
{
    return (SDL_GetWindowFlags(window) & SDL_WINDOW_RESIZABLE);
}

void UImGuiSDL::WindowSDL::pushWindowResizeCallback(const std::function<void(int, int)>& f) noexcept
{
    windowResizeCallbackList.push_back(f);
}

UImGui::FVector2 UImGuiSDL::WindowSDL::getWindowContentScale() noexcept
{
    const auto result = SDL_GetWindowDisplayScale(window);
    return UImGui::FVector2(result, result);
}

void UImGuiSDL::WindowSDL::pushWindowContentScaleCallback(const std::function<void(UImGui::FVector2)>& f) noexcept
{
    windowContentScaleChangeCallbackList.push_back(f);
}

void UImGuiSDL::WindowSDL::setSizeLimits(const UImGui::FVector2 min, const UImGui::FVector2 max) noexcept
{
    SDL_SetWindowMinimumSize(window, min.x, min.y);
    SDL_SetWindowMaximumSize(window, max.x, max.y);
}

void UImGuiSDL::WindowSDL::setSizeLimitByAspectRatio(const UImGui::FVector2 ratio) noexcept
{
    SDL_SetWindowAspectRatio(window, ratio.x / ratio.y, ratio.x / ratio.y);
}

UImGui::FVector4& UImGuiSDL::WindowSDL::getSizeLimits() noexcept
{
    return windowData.sizeLimits;
}

UImGui::FVector2& UImGuiSDL::WindowSDL::getAspectRatioSizeLimits() noexcept
{
    return windowData.aspectRatioSizeLimit;
}
