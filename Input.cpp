#include "UImGuiSDL.hpp"
#include <SDL3/SDL.h>

CKeyState UImGuiSDL::WindowSDL::getKey(const CKeys key) noexcept
{
    return keys[key];
}

void UImGuiSDL::WindowSDL::setCursorVisibility(const UImGui::CursorVisibilityState visibility) noexcept
{
    switch (visibility)
    {
    case UIMGUI_CURSOR_VISIBILITY_STATE_NORMAL:
        SDL_ShowCursor();
        break;
    case UIMGUI_CURSOR_VISIBILITY_STATE_HIDDEN:
        SDL_HideCursor();
        break;
    }
}

void UImGuiSDL::WindowSDL::setRawMouseMotion(const bool) noexcept
{
}

bool UImGuiSDL::WindowSDL::getRawMouseMotion() noexcept
{
    return false;
}
