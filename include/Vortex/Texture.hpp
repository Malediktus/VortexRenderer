#pragma once

#include "Core.hpp"
#include <memory>
#include <string>
#include <cassert>

namespace Vortex {
/**
 * @brief This is an abstract class and should not be instantiated.
 *
 * @version 0.1
 * @author Nico Grundei (malediktusrgb@gmail.com)
 * @date 2023-03-31
 * @copyright Copyright (c) 2023
 */
class Texture {
public:
    virtual ~Texture() = default;

    virtual uint32_t GetWidth() const = 0;
    virtual uint32_t GetHeight() const = 0;

    virtual void Bind(uint32_t slot = 0) const = 0;

    virtual void* GetNative() const = 0;
};

/**
 * @brief This class stores a 2D texture.
 *
 * @version 0.1
 * @author Nico Grundei (malediktusrgb@gmail.com)
 * @date 2023-03-31
 * @copyright Copyright (c) 2023
 */
class Texture2D : public Texture {
public:
    virtual ~Texture2D() = default;

    virtual uint32_t GetWidth() const override {
        assert(false);
    }

    virtual uint32_t GetHeight() const override {
        assert(false);
    }

    virtual void Bind(uint32_t) const override {
        assert(false);
    }

    virtual void* GetNative() const override {
        assert(false);
    }
};

/**
 * @brief This function is the only way that should be used for creating a Texture2D from a file.
 *
 * @param path
 * @return std::shared_ptr<Texture2D>
 * @version 0.1
 * @author Nico Grundei (malediktusrgb@gmail.com)
 * @date 2023-03-31
 * @copyright Copyright (c) 2023
 */
VT_API std::shared_ptr<Texture2D> Texture2DCreate(const std::string& path);
VT_API std::shared_ptr<Texture2D> Texture2DCreate(const int width, const int height);
/**
 * @brief This function is the only way that should be used for creating a Texture2D from pixel data.
 *
 * @param width
 * @param height
 * @param data
 * @return std::shared_ptr<Texture2D>
 * @version 0.1
 * @author Nico Grundei (malediktusrgb@gmail.com)
 * @date 2023-03-31
 * @copyright Copyright (c) 2023
 */
VT_API std::shared_ptr<Texture2D> Texture2DCreate(const int width, const int height, const void* data);
} // namespace Vortex
