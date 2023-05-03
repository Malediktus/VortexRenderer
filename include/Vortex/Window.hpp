#pragma once

namespace Vortex {
class Window {
public:
    virtual ~Window() = default;

    virtual void* GetSurface() = 0;
    virtual void SetupOpenglContext() = 0;
};
} // namespace Vortex
