# UImGuiSDL

[![CI](https://github.com/MadLadSquad/UImGuiSDL/actions/workflows/ci.yml/badge.svg)](https://github.com/MadLadSquad/UImGuiSDL/actions/workflows/ci.yml)
[![MIT license](https://img.shields.io/badge/License-MIT-blue.svg)](https://lbesson.mit-license.org/)
[![trello](https://img.shields.io/badge/Trello-UDE-blue])](https://trello.com/b/HmfuRY2K/untitleddesktop)
[![Discord](https://img.shields.io/discord/717037253292982315.svg?label=&logo=discord&logoColor=ffffff&color=7389D8&labelColor=6A7EC2)](https://discord.gg/4wgH8ZE)

Custom window backend for the [UntitledImGuiFramework](https://github.com/MadLadSquad/UntitledImGuiFramework) based on 
[SDL](https://libsdl.org/).

Supported subsystems:

1. All functions in the Window interface
1. All functions in the Input interface
1. Custom Monitors(with extended SDL functions)
1. All renderers:
   - OpenGL + in Emscripten
   - WebGPU(Emscripten-only)
   - Vulkan
   - [UImGuiRendererExamples](https://github.com/MadLadSquad/UImGuiRendererExamples):
     - [bgfx](https://github.com/bkaradzic/bgfx) + in Emscripten
     - DirectX 12
     - Metal

## Building and testing
Clone the SDL repository in a framework project's root directory using the following command:
```
user $ git clone https://github.com/MadLadSquad/UImGuiSDL.git --recursive
```
Next, make sure you're compiling your project with the `imgui_impl_sdl3.{h,cpp}` backends. If cloning dear imgui to
explicitly copy them, make sure to check out into the `docking` branch before copying!

Then, edit your `Config/cmake/<Your project>.cmake` file to look like this:
```cmake
function(custom_setup_step)
    set(SDL_AUDIO_DEFAULT OFF)
    set(SDL_GPU_DEFAULT OFF)
    set(SDL_RENDER_DEFAULT OFF)
    set(SDL_CAMERA_DEFAULT OFF)
    set(SDL_JOYSTICK_DEFAULT OFF)
    set(SDL_HAPTIC_DEFAULT OFF)
    set(SDL_HIDAPI_DEFAULT OFF)
    set(SDL_POWER_DEFAULT OFF)
    set(SDL_SENSOR_DEFAULT OFF)
    set(SDL_DIALOG_DEFAULT OFF)
    set(SDL_TRAY_DEFAULT OFF)

    set(SDL_AUDIO OFF)
    set(SDL_GPU OFF)
    set(SDL_RENDER OFF)
    set(SDL_CAMERA OFF)
    set(SDL_JOYSTICK OFF)
    set(SDL_HAPTIC OFF)
    set(SDL_HIDAPI OFF)
    set(SDL_POWER OFF)
    set(SDL_SENSOR OFF)
    set(SDL_DIALOG OFF)
    set(SDL_TRAY OFF)

    set(SDL_TEST_LIBRARY OFF)
    set(SDL_TESTS OFF)
    set(SDL_EXAMPLES OFF)

    add_subdirectory(SDL/)
endfunction()

function(custom_compile_step)
    if (EMSCRIPTEN)
        multicast(target_link_options PRIVATE "-sUSE_SDL=3")
        target_link_libraries(UImGuiDemo SDL3::SDL3-static)
    elseif (WIN32)
        target_link_libraries(UntitledImGuiFramework SDL3::SDL3-static)
    else ()
        target_link_libraries(UImGuiDemoLib SDL3::SDL3-static)
    endif ()
    include_directories(SDL/include/)
endfunction()

set(ENABLE_PRE_SCRIPT ON)
set(ENABLE_POST_SCRIPT ON)
```

> [!NOTE]
> Disabling all the subsystems in the example above is not necessary, but it will improve compilation times. We added
> that because we don't use these features and there's no point in compiling them in, but feel free to enable them
> if needed.

Then, include `UImGuiSDL/UImGuiSDL.hpp` in your project and create an instance of the `UImGuiSDL` class. 
In your instance's constructor, provide the memory address of the instance to the `InitInfo` struct:
```cpp
UImGuiDemo::Instance::Instance() noexcept
{
    initInfo =
    {
        .titlebarComponents = { reinterpret_cast<UImGui::TitlebarComponent*>(&title) },
        .windowComponents = { reinterpret_cast<UImGui::WindowComponent*>(&demoWindow) },
        .customWindow = &sdlWindow,
        UIMGUI_INIT_INFO_DEFAULT_DIRS,
    };
}
```
Compile and run your project.
