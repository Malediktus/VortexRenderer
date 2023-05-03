set(ASSET_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/assets/shaders/Base.glsl"
    "${CMAKE_CURRENT_SOURCE_DIR}/assets/textures/Vulkano.png")

add_executable(VortexDemo demo.cpp)
target_compile_features(VortexDemo PRIVATE cxx_std_17)

if(MSVC)
    target_compile_options(VortexDemo PRIVATE /W4 /WX)
else()
    target_compile_options(VortexDemo PRIVATE -Wall -Wextra -Wpedantic)
endif()

target_link_libraries(VortexDemo PRIVATE Vortex::Vortex glfw Vulkan::Vulkan)
