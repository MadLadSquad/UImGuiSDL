#include "UImGuiSDL.hpp"

#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>

#include "imgui_impl_sdl3.h"

#ifndef __EMSCRIPTEN__
void UImGuiSDL::WindowSDL::ImGuiInitFor_Vulkan() noexcept
{
    ImGui_ImplSDL3_InitForVulkan(window);
}
#endif

void UImGuiSDL::WindowSDL::ImGuiInitFor_OpenGL() noexcept
{
    ImGui_ImplSDL3_InitForOpenGL(window, SDL_GL_GetCurrentContext());
}

void UImGuiSDL::WindowSDL::ImGuiInitFor_Other() noexcept
{
    ImGui_ImplSDL3_InitForOther(window);
}

void UImGuiSDL::WindowSDL::ImGuiInstallCallbacks() noexcept
{
}

void UImGuiSDL::WindowSDL::ImGuiNewFrame() noexcept
{
    ImGui_ImplSDL3_NewFrame();
}

void UImGuiSDL::WindowSDL::ImGuiShutdown() noexcept
{
    ImGui_ImplSDL3_Shutdown();
}

bool UImGuiSDL::WindowSDL::shouldRender() noexcept
{
    return bShouldClose;
}

void UImGuiSDL::WindowSDL::pollEvents(double& now, double& deltaTime, double& lastTime) noexcept
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        ImGui_ImplSDL3_ProcessEvent(&event);
        handleEvent(event);
    }

    SDL_Time time;
    SDL_GetCurrentTime(&time);

    now = time;
    deltaTime = now - lastTime;
    lastTime = now;

    // TODO: Implement
    //for (auto& a : inputActionList)
    //{
    //    if (!a.keyCodes.empty())
    //    {
    //        for (const auto& f : a.keyCodes)
    //            if (keys[f] != keys[a.keyCodes[0]])
    //                goto finish_inner_loop;
    //        a.state = keys[a.keyCodes[0]];
    //    }
    //    finish_inner_loop:;
    //}

    // Needs to be updated at every frame
    if (windowData.bSurfaceTransparent)
        SDL_SetWindowOpacity(window, ImGui::GetStyle().Colors[ImGuiCol_WindowBg].w);
}

void UImGuiSDL::WindowSDL::waitEventsTimeout(const double timeout) noexcept
{
    SDL_Event event;
    SDL_WaitEventTimeout(&event, timeout);
    handleEvent(event);
}

void UImGuiSDL::WindowSDL::waitEvents() noexcept
{
    SDL_Event event;
    SDL_WaitEvent(&event);
    handleEvent(event);
}

void UImGuiSDL::WindowSDL::OpenGL_swapBuffers() noexcept
{
    SDL_GL_SwapWindow(window);
}

UImGui::RendererUtils::OpenGL::Context* UImGuiSDL::WindowSDL::OpenGL_createContext() noexcept
{
    return SDL_GL_CreateContext(window);
}

UImGui::RendererUtils::OpenGL::Context* UImGuiSDL::WindowSDL::OpenGL_getCurrentContext() noexcept
{
    return SDL_GL_GetCurrentContext();
}

void UImGuiSDL::WindowSDL::OpenGL_setCurrentContext(UImGui::RendererUtils::OpenGL::Context* ctx) noexcept
{
    SDL_GL_MakeCurrent(window, static_cast<SDL_GLContext>(ctx));
}

void UImGuiSDL::WindowSDL::OpenGL_destroyContext(UImGui::RendererUtils::OpenGL::Context* ctx) noexcept
{
    SDL_GL_DestroyContext(static_cast<SDL_GLContext>(ctx));
}

void UImGuiSDL::WindowSDL::OpenGL_setSwapInterval(const int interval) noexcept
{
    SDL_GL_SetSwapInterval(interval);
}

void UImGuiSDL::WindowSDL::OpenGL_setHints(const int majorVersion, const int minorVersion, const UImGui::RendererClientAPI clientApi, const UImGui::RendererUtils::OpenGL::Profile profile, const bool bForwardCompatible, const uint32_t samples) noexcept
{
    UNUSED(clientApi);

    SDL_GL_ResetAttributes();

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, majorVersion);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, minorVersion);

    switch (profile)
    {
    case UIMGUI_OPENGL_PROFILE_ANY:
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
        break;
    case UIMGUI_OPENGL_PROFILE_CORE:
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        break;
    case UIMGUI_OPENGL_PROFILE_COMPAT:
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
        break;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, bForwardCompatible ? SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG : 0);

    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    // Sample counts above 16 seem to break on Wayland
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, CAST(int, (samples >= 8 ? 8 : samples)));

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    windowFlags |= SDL_WINDOW_OPENGL;
}

UImGui::RendererUtils::OpenGL::GetProcAddressFun UImGuiSDL::WindowSDL::OpenGL_getProcAddressFunction() noexcept
{
    return SDL_GL_GetProcAddress;
}

void UImGuiSDL::WindowSDL::setupManualRenderingHints() noexcept
{
    switch (UImGui::RendererUtils::getRenderer()->type)
    {
    case UIMGUI_RENDERER_API_TYPE_HINT_VULKAN:
        windowFlags |= SDL_WINDOW_VULKAN;
        break;
    case UIMGUI_RENDERER_API_TYPE_HINT_METAL:
        windowFlags |= SDL_WINDOW_METAL;
        break;
    default:
        break;
    }
}

#ifndef __EMSCRIPTEN__
VkResult UImGuiSDL::WindowSDL::Vulkan_createWindowSurface(const VkInstance instance, const VkAllocationCallbacks* allocator, VkSurfaceKHR* surface) noexcept
{
    return (SDL_Vulkan_CreateSurface(window, instance, allocator, surface)) ? VK_SUCCESS : VK_ERROR_UNKNOWN;
}

void UImGuiSDL::WindowSDL::Vulkan_fillInstanceAndLayerExtensions(UImGui::TVector<const char*>& instanceExtensions, UImGui::TVector<const char*>& instanceLayers) noexcept
{
    uint32_t extensionCount = 0;
    char const* const* extensions = SDL_Vulkan_GetInstanceExtensions(&extensionCount);

    instanceExtensions.resize(extensionCount);
    for (size_t i = 0; i < extensionCount; i++)
        instanceExtensions[i] = extensions[i];
}
#endif
