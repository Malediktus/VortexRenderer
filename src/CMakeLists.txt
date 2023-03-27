set(HEADER_LIST "${VortexRenderer_SOURCE_DIR}/include/Vortex/Vortex.hpp" "${VortexRenderer_SOURCE_DIR}/include/Vortex/Core.hpp")

add_library(Vortex Vortex.cpp ${HEADER_LIST})
target_include_directories(Vortex PUBLIC ../include)
target_compile_definitions(Vortex PRIVATE VT_EXPORT_API)
target_compile_features(Vortex PUBLIC cxx_std_17)

source_group(
    TREE "${PROJECT_SOURCE_DIR}/include"
    PREFIX "Header Files"
    FILES ${HEADER_LIST})
