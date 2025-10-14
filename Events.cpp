#include "UImGuiSDL.hpp"
#include <SDL3/SDL.h>

void UImGuiSDL::WindowSDL::handleEvent(::SDL_Event& e) noexcept
{
    switch (e.type)
    {
    // -----------------------------------------------------------------------------------------------------------------
    // ------------------------------------------------- Window closure ------------------------------------------------
    // -----------------------------------------------------------------------------------------------------------------

    case SDL_EVENT_QUIT:
    case SDL_EVENT_TERMINATING:
    case SDL_EVENT_WINDOW_DESTROYED:
        bShouldClose = true;
        break;

    case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
        if (e.window.windowID == SDL_GetWindowID(window))
            bShouldClose = true;
        break;

    // -----------------------------------------------------------------------------------------------------------------
    // ------------------------------------------------ Window movement ------------------------------------------------
    // -----------------------------------------------------------------------------------------------------------------

    case SDL_EVENT_WINDOW_MOVED:

        windowLastPos.x = windowCurrentPos.x;
        windowLastPos.y = windowCurrentPos.y;

        windowCurrentPos.x = static_cast<float>(e.window.data1);
        windowCurrentPos.y = static_cast<float>(e.window.data2);

        for (auto& a : windowPositionChangeCallbackList)
            a(UImGui::FVector2{ CAST(float, e.window.data1), CAST(float, e.window.data2) });
        break;


    // -----------------------------------------------------------------------------------------------------------------
    // ------------------------------------------------- Window resized ------------------------------------------------
    // -----------------------------------------------------------------------------------------------------------------

    case SDL_EVENT_WINDOW_RESIZED:
        windowSizeInScreenCoords = { CAST(float, e.window.data1), CAST(float, e.window.data2) };
        for (auto& a : windowResizeInScreenCoordCallbackList)
            a(e.window.data1, e.window.data2);
        break;

    case SDL_EVENT_WINDOW_METAL_VIEW_RESIZED:
    case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
        updateEmscriptenRect();
        windowSizeS.x = static_cast<float>(e.window.data1);
        windowSizeS.y = static_cast<float>(e.window.data2);

        for (auto& a : windowResizeCallbackList)
            a(e.window.data1, e.window.data2);
        break;

    // -----------------------------------------------------------------------------------------------------------------
    // --------------------------------------- Window maximisation/iconification ---------------------------------------
    // -----------------------------------------------------------------------------------------------------------------

    case SDL_EVENT_WINDOW_MINIMIZED:
        for (auto& a : windowIconifiedCallbackList)
            a(true);
        bCurrentlyMaximised = false;
        bCurrentlyMinimised = true;
        break;
    case SDL_EVENT_WINDOW_MAXIMIZED:
        for (auto& a : windowMaximisedCallbackList)
            a(true);
        bCurrentlyMaximised = true;
        bCurrentlyMinimised = false;
        break;
    case SDL_EVENT_WINDOW_RESTORED:
        if (bCurrentlyMaximised)
            for (auto& a : windowMaximisedCallbackList)
                a(false);
        else
            for (auto& a : windowIconifiedCallbackList)
                a(false);

        bCurrentlyMaximised = false;
        bCurrentlyMinimised = false;
        break;

    // -----------------------------------------------------------------------------------------------------------------
    // -------------------------------------------------- Window focus -------------------------------------------------
    // -----------------------------------------------------------------------------------------------------------------

    case SDL_EVENT_WINDOW_FOCUS_GAINED:
    case SDL_EVENT_WINDOW_FOCUS_LOST:
        for (auto& a : windowFocusCallbackList)
            a(!(e.type - SDL_EVENT_WINDOW_FOCUS_GAINED));
        break;


    // -----------------------------------------------------------------------------------------------------------------
    // ---------------------------------------------- Window content scale ---------------------------------------------
    // -----------------------------------------------------------------------------------------------------------------

    case SDL_EVENT_WINDOW_DISPLAY_SCALE_CHANGED:
        {
            const auto result = SDL_GetWindowDisplayScale(window);
            for (auto& a : windowContentScaleChangeCallbackList)
                a(UImGui::FVector2(result, result));
            break;
        }


    // -----------------------------------------------------------------------------------------------------------------
    // ----------------------------------------------------- Input -----------------------------------------------------
    // -----------------------------------------------------------------------------------------------------------------

    case SDL_EVENT_KEY_DOWN:
    case SDL_EVENT_KEY_UP:
        break;

    case SDL_EVENT_MOUSE_MOTION:
        {
            static bool bFirst = true;
            if (bFirst)
            {
                mouseLastPos.x = e.motion.x;
                mouseLastPos.y = e.motion.y;
                bFirst = false;
            }

            mouseOffset.x = e.motion.x - mouseLastPos.x;
            mouseOffset.y = mouseLastPos.y - e.motion.y;

            mouseLastPos.x = e.motion.x;
            mouseLastPos.y = e.motion.y;
        }
        break;

    case SDL_EVENT_MOUSE_BUTTON_DOWN:
    case SDL_EVENT_MOUSE_BUTTON_UP:
        break;

    case SDL_EVENT_MOUSE_WHEEL:
        scroll = { e.wheel.x, e.wheel.y };
        break;

    // -----------------------------------------------------------------------------------------------------------------
    // -------------------------------------------------- Drag & drop --------------------------------------------------
    // -----------------------------------------------------------------------------------------------------------------

    case SDL_EVENT_DROP_FILE:
    case SDL_EVENT_DROP_TEXT:
    case SDL_EVENT_DROP_COMPLETE:
        for (auto& a : dragDropPathCallbackList)
            a(e.drop.data);

    default:
        break;
    }
}