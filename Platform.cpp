#include "UImGuiSDL.hpp"
#include <SDL3/SDL.h>

#ifdef _WIN32
#elifdef __APPLE__
    #include <OpenGL/GL.h>
    #include "Framework/Window/GenericWindow/macOS/MacOSWindowPlatform.h"
#elifndef __EMSCRIPTEN__
    #if __has_include(<wayland-client.h>)
        #include <wayland-client.h>
    #endif
    #if __has_include(<X11/Xatom.h>)
        #include <X11/Xlib.h>
        #include <X11/extensions/Xrandr.h>
        #include <X11/Xatom.h>
        #define _NET_WM_STATE_ADD 1
    #endif
#endif
#include <cstring>
#include <Interfaces/RendererInterface.hpp>

void UImGuiSDL::WindowSDL::Platform_setWindowAlwaysOnTop() noexcept
{
    SDL_SetWindowAlwaysOnTop(window, true);
}

void UImGuiSDL::WindowSDL::Platform_setWindowAlwaysOnBottom() noexcept
{
#ifdef _WIN32
    auto win = glfwGetWin32Window(window);
    SetWindowPos(win, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
    //DefWindowProcA(win, WM_WINDOWPOSCHANGING, 0, 0);
#elifdef __APPLE__
    const auto props = SDL_GetWindowProperties(window);
    void* win = nullptr;
    win = SDL_GetPointerProperty(props, SDL_PROP_WINDOW_COCOA_WINDOW_POINTER, win);
    UImGui::MacOSWindow::setWindowAlwaysBelow(win);
#elifndef __EMSCRIPTEN__
    if (Platform_getCurrentWindowPlatform() == UIMGUI_WINDOW_PLATFORM_X11)
    {
        const SDL_PropertiesID props = SDL_GetWindowProperties(window);
        Display* display = nullptr;
        ::Window win;

        display = static_cast<Display*>(SDL_GetPointerProperty(props, SDL_PROP_WINDOW_X11_DISPLAY_POINTER, display));
        win = reinterpret_cast<Window>(SDL_GetPointerProperty(props, SDL_PROP_WINDOW_X11_WINDOW_NUMBER, &win));

        const ::Window root = DefaultRootWindow(display);

        const Atom wmNetWmState = XInternAtom(display, "_NET_WM_STATE", 1);
        if (wmNetWmState == None)
        {
            Logger::log("Couldn't find the \"_NET_WM_STATE\" Atom!", ULOG_LOG_TYPE_ERROR);
            return;
        }

        const auto wmStateSticky = XInternAtom(display, "_NET_WM_STATE_STICKY", 1);
        if (wmStateSticky == None)
        {
            Logger::log("Couldn't find the \"_NET_WM_STATE_STICKY\" Atom!", ULOG_LOG_TYPE_ERROR);
            return;
        }

        const Atom wmStateBelow = XInternAtom(display, "_NET_WM_STATE_BELOW", 1);
        if (wmStateBelow == None)
        {
            Logger::log("Couldn't find the \"_NET_WM_STATE_BELOW\" Atom!", ULOG_LOG_TYPE_ERROR);
            return;
        }

        XClientMessageEvent xclient = {};

        xclient.type = ClientMessage;
        xclient.window = win;
        xclient.message_type = wmNetWmState;
        xclient.format = 32;
        xclient.data.l[0] = _NET_WM_STATE_ADD;
        xclient.data.l[1] = static_cast<long>(wmStateBelow);

        XSendEvent(display, root, False, SubstructureRedirectMask | SubstructureNotifyMask, reinterpret_cast<XEvent*>(&xclient));

        xclient.type = ClientMessage;
        xclient.window = win;
        xclient.message_type = wmNetWmState;
        xclient.format = 32;
        xclient.data.l[0] = _NET_WM_STATE_ADD;
        xclient.data.l[1] = static_cast<long>(wmStateSticky);

        XSendEvent(display, root, False, SubstructureRedirectMask | SubstructureNotifyMask, reinterpret_cast<XEvent*>(&xclient));
        XFlush(display);
        return;
    }

    if (Platform_getCurrentWindowPlatform() == UIMGUI_WINDOW_PLATFORM_WAYLAND)
    {
        // TODO: Implement for post-2.0
    }
#endif
}

void UImGuiSDL::WindowSDL::Platform_setWindowShowingOnPager(const bool bShowInPager) noexcept
{
    this->bShowOnPager = bShowInPager;
#ifdef _WIN32
    if (!bShowOnPager)
    {
        auto win = glfwGetWin32Window(window);
        LONG_PTR style = GetWindowLongPtr(win, GWL_EXSTYLE);
        SetWindowLongPtr(win, GWL_EXSTYLE, (style & ~WS_EX_APPWINDOW) | WS_EX_TOOLWINDOW);
    }
    else
    {
        auto win = glfwGetWin32Window(window);
        LONG_PTR style = GetWindowLongPtr(win, GWL_EXSTYLE);
        SetWindowLongPtr(win, GWL_EXSTYLE, (style & WS_EX_APPWINDOW) | ~WS_EX_TOOLWINDOW);
    }
#elif !defined(__APPLE__) && !defined(__EMSCRIPTEN__)
    if (Platform_getCurrentWindowPlatform() == UIMGUI_WINDOW_PLATFORM_X11 && !bShowOnPager)
    {
        const SDL_PropertiesID props = SDL_GetWindowProperties(window);
        Display* display = nullptr;
        ::Window win;

        display = static_cast<Display*>(SDL_GetPointerProperty(props, SDL_PROP_WINDOW_X11_DISPLAY_POINTER, display));
        win = reinterpret_cast<Window>(SDL_GetPointerProperty(props, SDL_PROP_WINDOW_X11_WINDOW_NUMBER, &win));

        const ::Window root = DefaultRootWindow(display);

        const Atom wmNetWmState = XInternAtom(display, "_NET_WM_STATE", 1);
        if (wmNetWmState == None)
        {
            Logger::log("Couldn't find the \"_NET_WM_STATE\" Atom!", ULOG_LOG_TYPE_ERROR);
            return;
        }

        const Atom wmStateSkipPager = XInternAtom(display, "_NET_WM_STATE_SKIP_PAGER", 1);
        if (wmStateSkipPager == None)
        {
            Logger::log("Couldn't find the \"_NET_WM_STATE_SKIP_PAGER\" Atom!", ULOG_LOG_TYPE_ERROR);
            return;
        }
        XClientMessageEvent xclient = {};

        xclient.type = ClientMessage;
        xclient.window = win;
        xclient.message_type = wmNetWmState;
        xclient.format = 32;
        xclient.data.l[0] = _NET_WM_STATE_ADD;
        xclient.data.l[1] = static_cast<long>(wmStateSkipPager);

        XSendEvent(display, root, False, SubstructureRedirectMask | SubstructureNotifyMask, reinterpret_cast<XEvent*>(&xclient));
        XFlush(display);
        return;
    }
#endif
}

bool UImGuiSDL::WindowSDL::Platform_getWindowShowingOnPager() noexcept
{
    return bShowOnPager;
}

void UImGuiSDL::WindowSDL::Platform_setWindowShowingOnTaskbar(const bool bShowOnTaskbar) noexcept
{
    this->bShowOnTaskbar = bShowOnTaskbar;
#ifdef _WIN32
    if (!bShowOnPager)
    {
        auto win = glfwGetWin32Window(window);
        LONG_PTR style = GetWindowLongPtr(win, GWL_EXSTYLE);
        SetWindowLongPtr(win, GWL_EXSTYLE, (style & ~WS_EX_APPWINDOW) | WS_EX_TOOLWINDOW);
    }
    else
    {
        auto win = glfwGetWin32Window(window);
        LONG_PTR style = GetWindowLongPtr(win, GWL_EXSTYLE);
        SetWindowLongPtr(win, GWL_EXSTYLE, (style & WS_EX_APPWINDOW) | ~WS_EX_TOOLWINDOW);
    }
#elif !defined(__APPLE__) && !defined(__EMSCRIPTEN__)
    if (Platform_getCurrentWindowPlatform() == UIMGUI_WINDOW_PLATFORM_X11 && !bShowOnTaskbar)
    {
        const SDL_PropertiesID props = SDL_GetWindowProperties(window);
        Display* display = nullptr;
        ::Window win;

        display = static_cast<Display*>(SDL_GetPointerProperty(props, SDL_PROP_WINDOW_X11_DISPLAY_POINTER, display));
        win = reinterpret_cast<Window>(SDL_GetPointerProperty(props, SDL_PROP_WINDOW_X11_WINDOW_NUMBER, &win));

        const ::Window root = DefaultRootWindow(display);

        const Atom wmNetWmState = XInternAtom(display, "_NET_WM_STATE", 1);
        if (wmNetWmState == None)
        {
            Logger::log("Couldn't find the \"_NET_WM_STATE\" Atom!", ULOG_LOG_TYPE_ERROR);
            return;
        }

        const Atom wmStateSkipTaskbar = XInternAtom(display, "_NET_WM_STATE_SKIP_TASKBAR", 1);
        if (wmStateSkipTaskbar == None)
        {
            Logger::log("Couldn't find the \"NET_WM_STATE_SKIP_TASKBAR\" Atom!", ULOG_LOG_TYPE_ERROR);
            return;
        }

        XClientMessageEvent xclient = {};

        xclient.type = ClientMessage;
        xclient.window = win;
        xclient.message_type = wmNetWmState;
        xclient.format = 32;
        xclient.data.l[0] = _NET_WM_STATE_ADD;
        xclient.data.l[1] = static_cast<long>(wmStateSkipTaskbar);

        XSendEvent(display, root, False, SubstructureRedirectMask | SubstructureNotifyMask, reinterpret_cast<XEvent*>(&xclient));
        XFlush(display);
    }
#endif
}

bool UImGuiSDL::WindowSDL::Platform_getWindowShowingOnTaskbar() noexcept
{
    return bShowOnPager;
}

void UImGuiSDL::WindowSDL::Platform_setWindowType(UImGui::String type) noexcept
{
#if !defined(_WIN32) && !defined(__APPLE__) && !defined(__EMSCRIPTEN__)
    if (Platform_getCurrentWindowPlatform() == UIMGUI_WINDOW_PLATFORM_X11)
    {
        const SDL_PropertiesID props = SDL_GetWindowProperties(window);
        Display* display = nullptr;
        ::Window win;

        display = static_cast<Display*>(SDL_GetPointerProperty(props, SDL_PROP_WINDOW_X11_DISPLAY_POINTER, display));
        win = reinterpret_cast<Window>(SDL_GetPointerProperty(props, SDL_PROP_WINDOW_X11_WINDOW_NUMBER, &win));

        const Atom windowType = XInternAtom(display, "_NET_WM_WINDOW_TYPE", 1);
        if (windowType == None)
        {
            Logger::log("Couldn't find atom of type \"_NET_WM_WINDOW_TYPE\"!", ULOG_LOG_TYPE_ERROR);
            return;
        }

        Atom windowTypeT = XInternAtom(display, type, 1);
        if (windowTypeT == None)
        {
            Logger::log("Couldn't find atom of type \"", ULOG_LOG_TYPE_ERROR, type, "\"!");
            return;
        }

        XChangeProperty(display, win, windowType, XA_ATOM, 32, PropModeReplace, reinterpret_cast<unsigned char*>(&windowTypeT), 1);
    }
#endif
}

size_t UImGuiSDL::WindowSDL::Platform_getWindowID() noexcept
{
#ifdef _WIN32
    return GetWindowLong(glfwGetWin32Window(window), GWL_ID);
#elifdef __APPLE__
    const auto props = SDL_GetWindowProperties(window);
    void* win = nullptr;
    return reinterpret_cast<intptr_t>(SDL_GetPointerProperty(props, SDL_PROP_WINDOW_COCOA_WINDOW_POINTER, win));
#elifndef __EMSCRIPTEN__
    if (Platform_getCurrentWindowPlatform() == UIMGUI_WINDOW_PLATFORM_X11)
    {
        const SDL_PropertiesID props = SDL_GetWindowProperties(window);
        Display* display = nullptr;
        ::Window win;

        display = static_cast<Display*>(SDL_GetPointerProperty(props, SDL_PROP_WINDOW_X11_DISPLAY_POINTER, display));
        win = reinterpret_cast<Window>(SDL_GetPointerProperty(props, SDL_PROP_WINDOW_X11_WINDOW_NUMBER, &win));

        const Atom pid = XInternAtom(display, "_NET_WM_PID", 1);
        if (pid == None)
        {
            Logger::log("Couldn't find the \"_NET_WM_PID\" Atom!", ULOG_LOG_TYPE_ERROR);
            return -1;
        }
        Atom type;
        int format;
        uint64_t nItems;
        uint64_t bytesAfter;
        unsigned char* propPID = nullptr;

        const auto result = XGetWindowProperty(display, win, pid, 0, 1, False, XA_CARDINAL, &type, &format, &nItems, &bytesAfter, &propPID);
        if (result == Success && propPID != nullptr)
        {
            const uint64_t xid = *reinterpret_cast<uint64_t*>(propPID);
            XFree(propPID);
            return xid;
        }
        Logger::log("Failed to get \"_NET_WM_PID\" of the current window or the XID is output by XGetWindowProperty is null.", ULOG_LOG_TYPE_ERROR);
        return -1;
    }

    if (Platform_getCurrentWindowPlatform() == UIMGUI_WINDOW_PLATFORM_WAYLAND)
    {
        const SDL_PropertiesID props = SDL_GetWindowProperties(window);
        void* id = nullptr;
        id = SDL_GetPointerProperty(props, SDL_PROP_WINDOW_WAYLAND_XDG_TOPLEVEL_EXPORT_HANDLE_STRING, id);
        if (id == nullptr)
            return -1;
        return reinterpret_cast<uintptr_t>(id);
    }
#endif
    return 0;
}

void* UImGuiSDL::WindowSDL::Platform_getNativeWindowHandle() noexcept
{
#ifdef __APPLE__
    const auto props = SDL_GetWindowProperties(window);
    void* win = nullptr;
    return SDL_GetPointerProperty(props, SDL_PROP_WINDOW_COCOA_WINDOW_POINTER, win);
#elifdef _WIN32
    return glfwGetWin32Window(window);
#elifdef __EMSCRIPTEN__
    return (void*)UImGui::Renderer::data().emscriptenCanvas;
#else
    const SDL_PropertiesID props = SDL_GetWindowProperties(window);
    void* result = nullptr;
    result = SDL_GetPointerProperty(props, SDL_PROP_WINDOW_WAYLAND_SURFACE_POINTER, result);
    if (result != nullptr)
        return result;
    return SDL_GetPointerProperty(props, SDL_PROP_WINDOW_X11_WINDOW_NUMBER, result);

#endif
}

UImGui::WindowPlatform UImGuiSDL::WindowSDL::Platform_getCurrentWindowPlatform() noexcept
{
#ifdef __APPLE__
    return UIMGUI_WINDOW_PLATFORM_COCOA;
#elifdef _WIN32
    return UIMGUI_WINDOW_PLATFORM_WIN32;
#elifdef __EMSCRIPTEN__
    return UIMGUI_WINDOW_PLATFORM_EMSCRIPTEN;
#else
    if (SDL_strcmp(SDL_GetCurrentVideoDriver(), "wayland") == 0)
        return UIMGUI_WINDOW_PLATFORM_WAYLAND;
    if (SDL_strcmp(SDL_GetCurrentVideoDriver(), "x11") == 0)
        return UIMGUI_WINDOW_PLATFORM_X11;
    return UIMGUI_WINDOW_PLATFORM_OTHER;
#endif
}

void* UImGuiSDL::WindowSDL::Platform_getNativeDisplay() noexcept
{
#if defined(__EMSCRIPTEN__) || defined(__APPLE__) || defined(_WIN32)
    return nullptr;
#else
    const SDL_PropertiesID props = SDL_GetWindowProperties(window);
    void* result = nullptr;
    result = SDL_GetPointerProperty(props, SDL_PROP_WINDOW_WAYLAND_DISPLAY_POINTER, result);
    if (result != nullptr)
        return result;
    return SDL_GetPointerProperty(props, SDL_PROP_WINDOW_X11_DISPLAY_POINTER, result);
#endif
}
