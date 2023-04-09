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

target_include_directories(VortexDemo PUBLIC imgui)
target_link_libraries(VortexDemo PRIVATE Vortex::Vortex glfw)

set(ASSET_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/assets")
set(ASSET_DEST_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets")

add_custom_command(
    TARGET VortexDemo POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink ${ASSET_SOURCE_DIR} ${ASSET_DEST_DIR}
    DEPENDS ${ASSET_DEST_DIR}
    COMMENT "symbolic link resources folder from ${ASSET_SOURCE_DIR} => ${ASSET_DEST_DIR}"
)
