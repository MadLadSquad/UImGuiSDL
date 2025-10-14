#pragma once
#include <Framework.hpp>

struct SDL_Window;
union SDL_Event;
namespace UImGuiSDL
{
    class UIMGUI_PUBLIC_API MonitorSDL final : public UImGui::GenericMonitor
    {
    public:
        UImGui::FVector4 getWorkArea(UImGui::MonitorData& data) noexcept override;
        UImGui::FVector2 getSize(UImGui::MonitorData& data) noexcept override;

        UImGui::String getName(UImGui::MonitorData& data) noexcept override;

        double getContentScale(UImGui::MonitorData& data) noexcept override;
        float getPixelDensity(UImGui::MonitorData& data) noexcept override;

        void* getPlatformHandle(UImGui::MonitorData& data) noexcept override;
    };

    class UIMGUI_PUBLIC_API WindowSDL final : public UImGui::GenericWindow
    {
    public:
        WindowSDL() noexcept;
        ~WindowSDL() noexcept override = default;

        void createWindow() noexcept override;
        void destroyWindow() noexcept override;

        // -------------------------------------------------------------------------------------------------------------
        // ---------------------------------------- Renderer and UI integration ----------------------------------------
        // -------------------------------------------------------------------------------------------------------------

#ifndef __EMSCRIPTEN__
        void ImGuiInitFor_Vulkan() noexcept override;
#endif
        void ImGuiInitFor_OpenGL() noexcept override;
        void ImGuiInitFor_Other() noexcept override;

        void ImGuiInstallCallbacks() noexcept override;
        void ImGuiNewFrame() noexcept override;
        void ImGuiShutdown() noexcept override;

        bool shouldRender() noexcept override;
        void pollEvents(double& now, double& deltaTime, double& lastTime) noexcept override;
        void waitEventsTimeout(double timeout) noexcept override;
        void waitEvents() noexcept override;

        UImGui::RendererUtils::OpenGL::Context* OpenGL_createContext() noexcept override;
        UImGui::RendererUtils::OpenGL::Context* OpenGL_getCurrentContext() noexcept override;
        void OpenGL_setCurrentContext(UImGui::RendererUtils::OpenGL::Context* ctx) noexcept override;
        void OpenGL_destroyContext(UImGui::RendererUtils::OpenGL::Context* ctx) noexcept override;

        void OpenGL_swapBuffers() noexcept override;
        void OpenGL_setSwapInterval(int interval) noexcept override;
        void OpenGL_setHints(int majorVersion, int minorVersion, UImGui::RendererClientAPI clientApi, UImGui::RendererUtils::OpenGL::Profile profile, bool bForwardCompatible, uint32_t samples) noexcept override;
        UImGui::RendererUtils::OpenGL::GetProcAddressFun OpenGL_getProcAddressFunction() noexcept override;

        void setupManualRenderingHints() noexcept override;

#ifndef __EMSCRIPTEN__
        VkResult Vulkan_createWindowSurface(VkInstance instance, const VkAllocationCallbacks* allocator, VkSurfaceKHR* surface) noexcept override;
        void Vulkan_fillInstanceAndLayerExtensions(UImGui::TVector<const char*>& instanceExtensions, UImGui::TVector<const char*>& instanceLayers) noexcept override;
#endif

        // -------------------------------------------------------------------------------------------------------------
        // --------------------------------------------- Input integration ---------------------------------------------
        // -------------------------------------------------------------------------------------------------------------

        CKeyState getKey(CKeys key) noexcept override;

        void setCursorVisibility(UImGui::CursorVisibilityState visibility) noexcept override;

        void setRawMouseMotion(bool bEnable) noexcept override;
        bool getRawMouseMotion() noexcept override;

        // -------------------------------------------------------------------------------------------------------------
        // ---------------------------------------------- Window interface ---------------------------------------------
        // -------------------------------------------------------------------------------------------------------------

        void setTitle(UImGui::String name) noexcept override;
        UImGui::String getTitle() noexcept override;
        void setTitleSetting(UImGui::String name) noexcept override;
        UImGui::String getTitleSetting() noexcept override;

        void setIcon(UImGui::String name) noexcept override;
        UImGui::String getIconLocation() noexcept override;
        UImGui::String getIconLocationSetting() noexcept override;
        void setIconLocationSetting(UImGui::String location) noexcept override;

        float getAspectRatio() noexcept override;

        UImGui::FVector2 getCurrentWindowPosition() noexcept override;
        UImGui::FVector2 getLastWindowPosition() noexcept override;
        UImGui::FVector2 getWindowPositionChange() noexcept override;

        void setCurrentWindowPosition(UImGui::FVector2 pos) noexcept override;
        void pushWindowPositionChangeCallback(const TFunction<void(UImGui::FVector2)>& f) noexcept override;

        UImGui::FVector2 getWindowSize() noexcept override;
        UImGui::FVector2& getWindowSizeSetting() noexcept override;

        bool getWindowFullscreen() noexcept override;
        bool& getWindowFullscreenSetting() noexcept override;
        void setWindowFullscreen(bool bFullscreen) noexcept override;

        void saveSettings(bool bSaveKeybinds) noexcept override;
        void refreshSettings() noexcept override;

        void close() noexcept override;
        void pushWindowCloseCallback(const TFunction<void(void)>& f) noexcept override;

        void Platform_setWindowAlwaysOnTop() noexcept override;
        void Platform_setWindowAlwaysOnBottom() noexcept override;

        void Platform_setWindowShowingOnPager(bool bShowInPager) noexcept override;
        bool Platform_getWindowShowingOnPager() noexcept override;

        void Platform_setWindowShowingOnTaskbar(bool bShowOnTaskbar) noexcept override;
        bool Platform_getWindowShowingOnTaskbar() noexcept override;

        void Platform_setWindowType(UImGui::String type) noexcept override;
        size_t Platform_getWindowID() noexcept override;

        void* Platform_getNativeWindowHandle() noexcept override;
        UImGui::WindowPlatform Platform_getCurrentWindowPlatform() noexcept override;
        void* Platform_getNativeDisplay() noexcept override;

        void setWindowSizeInScreenCoords(UImGui::FVector2 sz) noexcept override;
        UImGui::FVector2 getWindowSizeInScreenCoords() noexcept override;
        void pushWindowResizedInScreenCoordsCallback(const TFunction<void(int, int)>& f) noexcept override;

        void setWindowResizeable(bool bResizeable) noexcept override;
        bool& getWindowResizeableSetting() noexcept override;
        bool getWindowCurrentlyResizeable() noexcept override;
        void pushWindowResizeCallback(const TFunction<void(int, int)>& f) noexcept override;

        void requestWindowAttention() noexcept override;

        void hideWindow() noexcept override;
        void showWindow() noexcept override;
        bool& getWindowHiddenSetting() noexcept override;
        bool getWindowCurrentlyHidden() noexcept override;

        bool getWindowSurfaceTransparent() noexcept override;
        void setWindowSurfaceTransparent(bool bTransparent) noexcept override;
        bool& getWindowSurfaceTransparentSetting() noexcept override;

        void focusWindow() noexcept override;
        bool& getWindowFocusedSetting() noexcept override;
        bool getWindowCurrentlyFocused() noexcept override;
        void pushWindowFocusCallback(const TFunction<void(bool)>& f) noexcept override;

        void iconifyWindow() noexcept override;
        void restoreWindowState() noexcept override;
        void pushWindowIconifyCallback(const TFunction<void(bool)>& f) noexcept override;
        bool getWindowIconified() noexcept override;

        UImGui::FVector2 getWindowContentScale() noexcept override;
        void pushWindowContentScaleCallback(const TFunction<void(UImGui::FVector2)>& f) noexcept override;

        void setSizeLimits(UImGui::FVector2 min, UImGui::FVector2 max) noexcept override;
        void setSizeLimitByAspectRatio(UImGui::FVector2 ratio) noexcept override;
        UImGui::FVector4& getSizeLimits() noexcept override;
        UImGui::FVector2& getAspectRatioSizeLimits() noexcept override;

        UImGui::FVector4 getWindowDecorationFrameDistances() noexcept override;
        bool getCurrentWindowDecoratedState() noexcept override;
        bool& getWindowDecoratedSetting() noexcept override;
        void setWindowDecorated(bool bDecorated) noexcept override;

        void pushWindowRefreshCallback(const TFunction<void(void)>& f) noexcept override;

        void maximiseWindow() noexcept override;
        bool& getWindowMaximisedSetting() noexcept override;
        void pushWindowMaximiseCallback(const TFunction<void(bool)>& f) noexcept override;
        bool getWindowCurrentlyMaximised() noexcept override;

        UImGui::Monitor getWindowMonitor() noexcept override;
        UImGui::Monitor getPrimaryMonitor() noexcept override;
        const UImGui::TVector<UImGui::Monitor>& getMonitors() noexcept override;

        void pushWindowOSDragDropCallback(const TFunction<void(const UImGui::FString&)>& f) noexcept override;

        void* getInternal() noexcept override;
        GenericWindow* get() noexcept override;
    private:
        void handleEvent(SDL_Event& e) noexcept;

        SDL_Window* window = nullptr;

        MonitorSDL sdlMonitor{};
        UImGui::TVector<UImGui::Monitor> monitors{};

        UImGui::FString currentIconLocation{};
        bool bShouldClose = false;
        uint64_t windowFlags = 0;

        bool bCurrentlyMaximised = false;
        bool bCurrentlyMinimised = false;

        void updateEmscriptenRect() noexcept;
    };
}