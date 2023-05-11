set(ASSET_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/assets/shaders/Base.glsl"
    "${CMAKE_CURRENT_SOURCE_DIR}/assets/textures/Vulkano.png")

add_executable(VortexDemo
    demo.cpp

    extern/imgui/imconfig.h
    extern/imgui/imgui_demo.cpp
    extern/imgui/imgui_draw.cpp
    extern/imgui/imgui_internal.h
    extern/imgui/imgui_tables.cpp
    extern/imgui/imgui_widgets.cpp
    extern/imgui/imgui.cpp
    extern/imgui/imgui.h
    extern/imgui/imstb_rectpack.h
    extern/imgui/imstb_textedit.h
    extern/imgui/imstb_truetype.h
    extern/imgui/backends/imgui_impl_glfw.h
    extern/imgui/backends/imgui_impl_glfw.cpp
    extern/imgui/backends/imgui_impl_opengl3.h
    extern/imgui/backends/imgui_impl_opengl3.cpp
    extern/imgui/backends/imgui_impl_opengl3_loader.h)

target_compile_features(VortexDemo PRIVATE cxx_std_17)

if(MSVC)
    target_compile_options(VortexDemo PRIVATE /W4 /WX)
else()
    target_compile_options(VortexDemo PRIVATE -Wall -Wextra -Wpedantic)
endif()

target_include_directories(VortexDemo PRIVATE extern/imgui)
target_link_libraries(VortexDemo PRIVATE Vortex::Vortex glfw Vulkan::Vulkan)
