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

// SDL uses 0.0f for no limit instead of -1.0f as in GLFW
static void sanitiseSizeLimits(UImGui::FVector2& size)
{
    size.x = size.x < 1 ? 0.0f : size.x;
    size.y = size.y < 1 ? 0.0f : size.y;
}

void UImGuiSDL::WindowSDL::setSizeLimits(UImGui::FVector2 min, UImGui::FVector2 max) noexcept
{
    sanitiseSizeLimits(min);
    sanitiseSizeLimits(max);

    SDL_SetWindowMinimumSize(window, CAST(int, min.x), CAST(int, min.y));
    SDL_SetWindowMaximumSize(window, CAST(int, max.x), CAST(int, max.y));
}

void UImGuiSDL::WindowSDL::setSizeLimitByAspectRatio(UImGui::FVector2 ratio) noexcept
{
    sanitiseSizeLimits(ratio);
    float result = ratio.x / ratio.y;
    if (result != result)
        result = 0.0f;
    SDL_SetWindowAspectRatio(window, result, result);
}

UImGui::FVector4& UImGuiSDL::WindowSDL::getSizeLimits() noexcept
{
    return windowData.sizeLimits;
}

UImGui::FVector2& UImGuiSDL::WindowSDL::getAspectRatioSizeLimits() noexcept
{
    return windowData.aspectRatioSizeLimit;
}
