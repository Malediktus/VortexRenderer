add_executable(demo demo.cpp)
target_compile_features(demo PRIVATE cxx_std_17)

target_link_libraries(demo PRIVATE Vortex glfw)

add_custom_command(TARGET demo POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/resources/ ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources)
