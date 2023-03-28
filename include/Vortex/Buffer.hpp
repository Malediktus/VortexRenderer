#pragma once

#include "Core.hpp"

#include <cassert>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace Vortex {
enum class ShaderDataType { None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool };

namespace Utils {
static uint32_t ShaderDataTypeSize(ShaderDataType type) {
    switch (type) {
    case ShaderDataType::Float:
        return 4;
    case ShaderDataType::Float2:
        return 4 * 2;
    case ShaderDataType::Float3:
        return 4 * 3;
    case ShaderDataType::Float4:
        return 4 * 4;
    case ShaderDataType::Mat3:
        return 4 * 3 * 3;
    case ShaderDataType::Mat4:
        return 4 * 4 * 4;
    case ShaderDataType::Int:
        return 4;
    case ShaderDataType::Int2:
        return 4 * 2;
    case ShaderDataType::Int3:
        return 4 * 3;
    case ShaderDataType::Int4:
        return 4 * 4;
    case ShaderDataType::Bool:
        return 1;
    default:
        assert(false);
    }
    return 0;
}
} // namespace Utils

struct BufferElement {
    std::string Name;
    ShaderDataType Type;
    uint32_t Size;
    uint64_t Offset;
    bool Normalized;

    BufferElement() {
    }

    BufferElement(ShaderDataType type, const std::string& name, bool normalized = false)
        : Name(name), Type(type), Size(Utils::ShaderDataTypeSize(type)), Offset(0), Normalized(normalized) {
    }

    uint32_t GetComponentCount() const {
        switch (Type) {
        case ShaderDataType::Float:
            return 1;
        case ShaderDataType::Float2:
            return 2;
        case ShaderDataType::Float3:
            return 3;
        case ShaderDataType::Float4:
            return 4;
        case ShaderDataType::Mat3:
            return 3 * 3;
        case ShaderDataType::Mat4:
            return 4 * 4;
        case ShaderDataType::Int:
            return 1;
        case ShaderDataType::Int2:
            return 2;
        case ShaderDataType::Int3:
            return 3;
        case ShaderDataType::Int4:
            return 4;
        case ShaderDataType::Bool:
            return 1;
        default:
            assert(false);
        }

        return 0;
    }
};

class BufferLayout {
public:
    VT_API BufferLayout() {
    }

    VT_API BufferLayout(const std::initializer_list<BufferElement>& elements) : m_Elements(elements) {
        CalculateOffsetsAndStride();
    }

    VT_API inline uint32_t GetStride() const {
        return m_Stride;
    }
    VT_API inline const std::vector<BufferElement>& GetElements() const {
        return m_Elements;
    }

    VT_API std::vector<BufferElement>::iterator begin() {
        return m_Elements.begin();
    }
    VT_API std::vector<BufferElement>::iterator end() {
        return m_Elements.end();
    }
    VT_API std::vector<BufferElement>::const_iterator begin() const {
        return m_Elements.begin();
    }
    VT_API std::vector<BufferElement>::const_iterator end() const {
        return m_Elements.end();
    }

private:
    void CalculateOffsetsAndStride() {
        uint64_t offset = 0;
        m_Stride = 0;
        for (auto& element : m_Elements) {
            element.Offset = offset;
            offset += element.Size;
            m_Stride += element.Size;
        }
    }

    std::vector<BufferElement> m_Elements;
    uint32_t m_Stride = 0;
};

class VertexBuffer {
public:
    virtual VT_API ~VertexBuffer() {
    }

    virtual VT_API void Bind() const = 0;
    virtual VT_API void Unbind() const = 0;

    virtual VT_API const BufferLayout& GetLayout() const = 0;
    virtual VT_API void SetLayout(const BufferLayout& layout) = 0;
};

class IndexBuffer {
public:
    virtual ~IndexBuffer() {
    }

    virtual VT_API void Bind() const = 0;
    virtual VT_API void Unbind() const = 0;

    virtual uint32_t GetCount() const = 0;
};

VT_API std::shared_ptr<VertexBuffer> VertexBufferCreate(float* vertices, uint32_t size);
VT_API std::shared_ptr<IndexBuffer> IndexBufferCreate(uint32_t* indices, uint32_t size);
} // namespace Vortex
