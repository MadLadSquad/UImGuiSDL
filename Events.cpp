#include "UImGuiSDL.hpp"
#include <SDL3/SDL.h>

static CKeys handleMouseButtonEvent(const uint8_t key)
{
    switch (key)
    {
    case SDL_BUTTON_LEFT:
        return Keys_MouseButtonLeft;
    case SDL_BUTTON_MIDDLE:
        return Keys_MouseButtonMiddle;
    case SDL_BUTTON_RIGHT:
        return Keys_MouseButtonRight;
    case SDL_BUTTON_X1:
        return Keys_MouseButton4;
    case SDL_BUTTON_X2:
        return Keys_MouseButton5;
    default:
        return Keys_None;
    }
}

static CKeys handleKeyEvents(const SDL_Scancode key)
{
    switch (key)
    {
    case SDL_SCANCODE_UNKNOWN:
        return Keys_UnknownKey;
    case SDL_SCANCODE_A:
        return Keys_A;
    case SDL_SCANCODE_B:
        return Keys_B;
    case SDL_SCANCODE_C:
        return Keys_C;
    case SDL_SCANCODE_D:
        return Keys_D;
    case SDL_SCANCODE_E:
        return Keys_E;
    case SDL_SCANCODE_F:
        return Keys_F;
    case SDL_SCANCODE_G:
        return Keys_G;
    case SDL_SCANCODE_H:
        return Keys_H;
    case SDL_SCANCODE_I:
        return Keys_I;
    case SDL_SCANCODE_J:
        return Keys_J;
    case SDL_SCANCODE_K:
        return Keys_K;
    case SDL_SCANCODE_L:
        return Keys_L;
    case SDL_SCANCODE_M:
        return Keys_M;
    case SDL_SCANCODE_N:
        return Keys_N;
    case SDL_SCANCODE_O:
        return Keys_O;
    case SDL_SCANCODE_P:
        return Keys_P;
    case SDL_SCANCODE_Q:
        return Keys_Q;
    case SDL_SCANCODE_R:
        return Keys_R;
    case SDL_SCANCODE_S:
        return Keys_S;
    case SDL_SCANCODE_T:
        return Keys_T;
    case SDL_SCANCODE_U:
        return Keys_U;
    case SDL_SCANCODE_V:
        return Keys_V;
    case SDL_SCANCODE_W:
        return Keys_W;
    case SDL_SCANCODE_X:
        return Keys_X;
    case SDL_SCANCODE_Y:
        return Keys_Y;
    case SDL_SCANCODE_Z:
        return Keys_Z;
    case SDL_SCANCODE_1:
        return Keys_One;
    case SDL_SCANCODE_2:
        return Keys_Two;
    case SDL_SCANCODE_3:
        return Keys_Three;
    case SDL_SCANCODE_4:
        return Keys_Four;
    case SDL_SCANCODE_5:
        return Keys_Five;
    case SDL_SCANCODE_6:
        return Keys_Six;
    case SDL_SCANCODE_7:
        return Keys_Seven;
    case SDL_SCANCODE_8:
        return Keys_Eight;
    case SDL_SCANCODE_9:
        return Keys_Nine;
    case SDL_SCANCODE_0:
        return Keys_Zero;
    case SDL_SCANCODE_RETURN:
        return Keys_Enter;
    case SDL_SCANCODE_ESCAPE:
        return Keys_Escape;
    case SDL_SCANCODE_BACKSPACE:
        return Keys_Backspace;
    case SDL_SCANCODE_TAB:
        return Keys_Tab;
    case SDL_SCANCODE_SPACE:
        return Keys_Space;
    case SDL_SCANCODE_MINUS:
        return Keys_Minus;
    case SDL_SCANCODE_EQUALS:
        return Keys_Equal;
    case SDL_SCANCODE_LEFTBRACKET:
        return Keys_LeftBracket;
    case SDL_SCANCODE_RIGHTBRACKET:
        return Keys_RightBracket;
    case SDL_SCANCODE_BACKSLASH:
        return Keys_Backslash;
    case SDL_SCANCODE_SEMICOLON:
        return Keys_Semicolon;
    case SDL_SCANCODE_APOSTROPHE:
        return Keys_Apostrophe;
    case SDL_SCANCODE_GRAVE:
        return Keys_GraveAccent;
    case SDL_SCANCODE_COMMA:
        return Keys_Comma;
    case SDL_SCANCODE_PERIOD:
        return Keys_Period;
    case SDL_SCANCODE_SLASH:
        return Keys_Slash;
    case SDL_SCANCODE_CAPSLOCK:
        return Keys_CapsLock;
    case SDL_SCANCODE_F1:
        return Keys_F1;
    case SDL_SCANCODE_F2:
        return Keys_F2;
    case SDL_SCANCODE_F3:
        return Keys_F3;
    case SDL_SCANCODE_F4:
        return Keys_F4;
    case SDL_SCANCODE_F5:
        return Keys_F5;
    case SDL_SCANCODE_F6:
        return Keys_F6;
    case SDL_SCANCODE_F7:
        return Keys_F7;
    case SDL_SCANCODE_F8:
        return Keys_F8;
    case SDL_SCANCODE_F9:
        return Keys_F9;
    case SDL_SCANCODE_F10:
        return Keys_F10;
    case SDL_SCANCODE_F11:
        return Keys_F11;
    case SDL_SCANCODE_F12:
        return Keys_F12;
    case SDL_SCANCODE_PRINTSCREEN:
        return Keys_PrintScreen;
    case SDL_SCANCODE_SCROLLLOCK:
        return Keys_ScrollLock;
    case SDL_SCANCODE_PAUSE:
        return Keys_Pause;
    case SDL_SCANCODE_INSERT:
        return Keys_Insert;
    case SDL_SCANCODE_HOME:
        return Keys_Home;
    case SDL_SCANCODE_PAGEUP:
        return Keys_PageUp;
    case SDL_SCANCODE_DELETE:
        return Keys_Delete;
    case SDL_SCANCODE_END:
        return Keys_End;
    case SDL_SCANCODE_PAGEDOWN:
        return Keys_PageDown;
    case SDL_SCANCODE_RIGHT:
        return Keys_RightArrow;
    case SDL_SCANCODE_LEFT:
        return Keys_LeftArrow;
    case SDL_SCANCODE_DOWN:
        return Keys_DownArrow;
    case SDL_SCANCODE_UP:
        return Keys_UpArrow;
    case SDL_SCANCODE_NUMLOCKCLEAR:
        return Keys_NumLock;
    case SDL_SCANCODE_KP_DIVIDE:
        return Keys_NumPadDivide;
    case SDL_SCANCODE_KP_MULTIPLY:
        return Keys_NumPadMultiply;
    case SDL_SCANCODE_KP_MINUS:
        return Keys_NumPadSubtract;
    case SDL_SCANCODE_KP_PLUS:
        return Keys_NumPadAdd;
    case SDL_SCANCODE_KP_ENTER:
        return Keys_NumPadEnter;
    case SDL_SCANCODE_KP_1:
        return Keys_NumPadOne;
    case SDL_SCANCODE_KP_2:
        return Keys_NumPadTwo;
    case SDL_SCANCODE_KP_3:
        return Keys_NumPadThree;
    case SDL_SCANCODE_KP_4:
        return Keys_NumPadFour;
    case SDL_SCANCODE_KP_5:
        return Keys_NumPadFive;
    case SDL_SCANCODE_KP_6:
        return Keys_NumPadSix;
    case SDL_SCANCODE_KP_7:
        return Keys_NumPadSeven;
    case SDL_SCANCODE_KP_8:
        return Keys_NumPadEight;
    case SDL_SCANCODE_KP_9:
        return Keys_NumPadNine;
    case SDL_SCANCODE_KP_0:
        return Keys_NumPadZero;
    case SDL_SCANCODE_KP_PERIOD:
        return Keys_NumPadDecimal;
    case SDL_SCANCODE_KP_EQUALS:
        return Keys_NumPadEqual;
    case SDL_SCANCODE_F13:
        return Keys_F13;
    case SDL_SCANCODE_F14:
        return Keys_F14;
    case SDL_SCANCODE_F15:
        return Keys_F15;
    case SDL_SCANCODE_F16:
        return Keys_F16;
    case SDL_SCANCODE_F17:
        return Keys_F17;
    case SDL_SCANCODE_F18:
        return Keys_F18;
    case SDL_SCANCODE_F19:
        return Keys_F19;
    case SDL_SCANCODE_F20:
        return Keys_F20;
    case SDL_SCANCODE_F21:
        return Keys_F21;
    case SDL_SCANCODE_F22:
        return Keys_F22;
    case SDL_SCANCODE_F23:
        return Keys_F23;
    case SDL_SCANCODE_F24:
        return Keys_F24;
    case SDL_SCANCODE_MENU:
        return Keys_Menu;
    case SDL_SCANCODE_LCTRL:
        return Keys_LeftControl;
    case SDL_SCANCODE_LSHIFT:
        return Keys_LeftShift;
    case SDL_SCANCODE_LALT:
        return Keys_LeftAlt;
    case SDL_SCANCODE_LGUI:
        return Keys_LeftSuper;
    case SDL_SCANCODE_RCTRL:
        return Keys_RightControl;
    case SDL_SCANCODE_RSHIFT:
        return Keys_RightShift;
    case SDL_SCANCODE_RALT:
        return Keys_RightAlt;
    case SDL_SCANCODE_RGUI:
        return Keys_RightSuper;
    default:
        return Keys_UnknownKey;
    }
}


void UImGuiSDL::WindowSDL::handleEvent(::SDL_Event& e) noexcept
{
    switch (e.type)
    {
    // -----------------------------------------------------------------------------------------------------------------
    // ------------------------------------------------- Window closure ------------------------------------------------
    // -----------------------------------------------------------------------------------------------------------------

    case SDL_EVENT_QUIT:
    case SDL_EVENT_TERMINATING:
        bShouldClose = true;
        break;

    case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
    case SDL_EVENT_WINDOW_DESTROYED:
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
        keys[handleKeyEvents(e.key.scancode)] = e.key.repeat ? KeyStateRepeat : KeyStatePressed;
        break;
    case SDL_EVENT_KEY_UP:
        keys[handleKeyEvents(e.key.scancode)] = KeyStateReleased;
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
        keys[handleMouseButtonEvent(e.button.button)] = KeyStatePressed;
        break;
    case SDL_EVENT_MOUSE_BUTTON_UP:
        keys[handleMouseButtonEvent(e.button.button)] = KeyStateReleased;
        break;

    case SDL_EVENT_MOUSE_WHEEL:
        if (e.wheel.x == 0)
        {
            keys[Keys_ScrollLeft] = KeyStateReleased;
            keys[Keys_ScrollRight] = KeyStateReleased;
        }
        else if (e.wheel.x > 0)
        {
            if (e.wheel.direction == SDL_MOUSEWHEEL_FLIPPED)
            {
                keys[Keys_ScrollRight] = KeyStatePressed;
                keys[Keys_ScrollLeft] = KeyStateReleased;
            }
            else
            {
                keys[Keys_ScrollLeft] = KeyStatePressed;
                keys[Keys_ScrollRight] = KeyStateReleased;
            }
        }
        else if (e.wheel.x < 0)
        {
            if (e.wheel.direction == SDL_MOUSEWHEEL_FLIPPED)
            {
                keys[Keys_ScrollRight] = KeyStateReleased;
                keys[Keys_ScrollLeft] = KeyStatePressed;
            }
            else
            {
                keys[Keys_ScrollLeft] = KeyStatePressed;
                keys[Keys_ScrollRight] = KeyStateReleased;
            }
        }

        if (e.wheel.y == 0)
        {
            keys[Keys_ScrollUp] = KeyStateReleased;
            keys[Keys_ScrollDown] = KeyStateReleased;
        }
        else if (e.wheel.y > 0)
        {
            if (e.wheel.direction == SDL_MOUSEWHEEL_FLIPPED)
            {
                keys[Keys_ScrollUp] = KeyStatePressed;
                keys[Keys_ScrollDown] = KeyStateReleased;
            }
            else
            {
                keys[Keys_ScrollDown] = KeyStatePressed;
                keys[Keys_ScrollUp] = KeyStateReleased;
            }
        }
        else if (e.wheel.y < 0)
        {
            if (e.wheel.direction == SDL_MOUSEWHEEL_FLIPPED)
            {
                keys[Keys_ScrollUp] = KeyStateReleased;
                keys[Keys_ScrollDown] = KeyStatePressed;
            }
            else
            {
                keys[Keys_ScrollUp] = KeyStatePressed;
                keys[Keys_ScrollDown] = KeyStateReleased;
            }
        }

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