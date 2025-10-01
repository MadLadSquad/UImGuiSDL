# UImGuiSDL
Custom window backend for the [UntitledImGuiFramework](https://github.com/MadLadSquad/UntitledImGuiFramework) based on SDL.

Supported subsystems:

1. All functions in the Window interface
1. All functions in the Input interface
1. Custom Monitors(with extended SDL functions)
1. All renderers:
   - OpenGL
   - WebGPU
   - Vulkan
   - [UImGuiRendererExamples](https://github.com/MadLadSquad/UImGuiRendererExamples):
     - bgfx
     - dx12
     - metal

## Building and testing
Clone this repository in a framework project's `Source/` directory using the following command:
```
user $ git clone https://github.com/MadLadSquad/UImGuiSDL.git --recursive
```
Then, include `UImGuiSDL/UImGuiSDL.hpp` in your project and create an instance of the `UImGuiSDL` class. In your instance's constructor, provide the memory address of the
instance to the `InitInfo` struct:
```cpp
UImGuiDemo::Instance::Instance() noexcept
{
    initInfo =
    {
        .titlebarComponents = { reinterpret_cast<UImGui::TitlebarComponent*>(&title) },
        .windowComponents = { reinterpret_cast<UImGui::WindowComponent*>(&demoWindow) },
        UIMGUI_INIT_INFO_DEFAULT_DIRS,
        .customWindow = &inst
    };
}
```
Compile and run your project.
