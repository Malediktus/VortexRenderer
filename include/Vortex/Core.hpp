#pragma once

#ifdef VT_EXPORT_API
    // Exports
    #ifdef _MSC_VER
        #define VT_API __declspec(dllexport)
    #else
        #define VT_API __attribute__((visibility("default")))
    #endif
#else
    // Imports
    #ifdef _MSC_VER
        #define VT_API __declspec(dllimport)
    #else
        #define VT_API
    #endif
#endif

namespace Vortex {
enum class RenderingAPI { OpenGL };

extern RenderingAPI CurrentRenderingAPI;
} // namespace Vortex
