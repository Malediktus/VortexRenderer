set(ASSETS,
    "${CMAKE_CURRENT_SOURCE_DIR}/assets/shaders/Base.glsl"
    "${CMAKE_CURRENT_SOURCE_DIR}/assets/textures/Vulkano.png")

add_executable(demo demo.cpp ${ASSETS})
target_compile_features(demo PRIVATE cxx_std_17)

target_link_libraries(demo PRIVATE Vortex glfw)

set(ASSET_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/assets")
set(ASSET_DEST_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets")

source_group(
    TREE "${CMAKE_CURRENT_SOURCE_DIR/}assets"
    PREFIX "Assets"
    FILES ${ASSETS})

add_custom_command(
    TARGET demo POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink ${ASSET_SOURCE_DIR} ${ASSET_DEST_DIR}
    DEPENDS ${ASSET_DEST_DIR}
    COMMENT "symbolic link resources folder from ${ASSET_SOURCE_DIR} => ${ASSET_DEST_DIR}"
)
