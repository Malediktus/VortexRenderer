#include <Vortex/Platform/OpenGL/OpenGLRendererAPI.hpp>
#include <glad/glad.h>

using namespace Vortex::OpenGL;

namespace Vortex::Utils {
static GLenum StencilActionToGLenum(RendererAPI::StencilTestAction action) {
    switch (action) {
    case RendererAPI::StencilTestAction::KEEP:
        return GL_KEEP;
    case RendererAPI::StencilTestAction::ZERO:
        return GL_ZERO;
    case RendererAPI::StencilTestAction::REPLACE:
        return GL_REPLACE;
    case RendererAPI::StencilTestAction::INCR:
        return GL_INCR;
    case RendererAPI::StencilTestAction::INCR_WRAP:
        return GL_INCR_WRAP;
    case RendererAPI::StencilTestAction::DECR:
        return GL_DECR;
    case RendererAPI::StencilTestAction::DECR_WRAP:
        return GL_DECR_WRAP;
    case RendererAPI::StencilTestAction::INVERT:
        return GL_INVERT;
    }
}

static GLenum BlendingFuncToGLenum(RendererAPI::BlendingFunc action) {
    switch (action) {
    case RendererAPI::BlendingFunc::ZERO:
        return GL_ZERO;
    case RendererAPI::BlendingFunc::ONE:
        return GL_ONE;
    case RendererAPI::BlendingFunc::SRC_COLOR:
        return GL_SRC_COLOR;
    case RendererAPI::BlendingFunc::ONE_MINUS_SRC_COLOR:
        return GL_ONE_MINUS_SRC_COLOR;
    case RendererAPI::BlendingFunc::DST_COLOR:
        return GL_DST_COLOR;
    case RendererAPI::BlendingFunc::ONE_MINUS_DST_COLOR:
        return GL_ONE_MINUS_DST_COLOR;
    case RendererAPI::BlendingFunc::SRC_ALPHA:
        return GL_SRC_ALPHA;
    case RendererAPI::BlendingFunc::ONE_MINUS_SRC_ALPHA:
        return GL_ONE_MINUS_SRC_ALPHA;
    case RendererAPI::BlendingFunc::DST_ALPHA:
        return GL_DST_ALPHA;
    case RendererAPI::BlendingFunc::ONE_MINUS_DST_ALPHA:
        return GL_ONE_MINUS_DST_ALPHA;
    case RendererAPI::BlendingFunc::CONSTANT_COLOR:
        return GL_CONSTANT_COLOR;
    case RendererAPI::BlendingFunc::ONE_MINUS_CONSTANT_COLOR:
        return GL_ONE_MINUS_CONSTANT_COLOR;
    case RendererAPI::BlendingFunc::CONSTANT_ALPHA:
        return GL_CONSTANT_ALPHA;
    case RendererAPI::BlendingFunc::ONE_MINUS_CONSTANT_ALPHA:
        return GL_ONE_MINUS_CONSTANT_ALPHA;
    }
}
} // namespace Vortex::Utils

void OpenGLRendererAPI::SetClearColor(const glm::vec4& color) {
    glClearColor(color.r, color.g, color.b, color.a);
}

void OpenGLRendererAPI::Clear(const ClearBuffer clearBuffer) {
    switch (clearBuffer) {
    case ClearBuffer::COLOR:
        glClear(GL_COLOR_BUFFER_BIT);
        break;
    case ClearBuffer::DEPTH:
        glClear(GL_DEPTH_BUFFER_BIT);
        break;
    case ClearBuffer::STENCIL:
        glClear(GL_STENCIL_BUFFER_BIT);
        break;
    }
}

void OpenGLRendererAPI::SetViewport(const int width, const int height) {
    glViewport(0, 0, width, height);
}

void OpenGLRendererAPI::ConfigureDepthTesting(const bool enable, const bool depthMask, const DepthTestFunc func) {
    if (enable)
        glEnable(GL_DEPTH_TEST);
    else
        glDisable(GL_DEPTH_TEST);

    if (depthMask)
        glDepthMask(GL_TRUE);
    else
        glDepthMask(GL_FALSE);

    switch (func) {
    case DepthTestFunc::ALWAYS:
        glDepthFunc(GL_ALWAYS);
        break;
    case DepthTestFunc::NEVER:
        glDepthFunc(GL_NEVER);
        break;
    case DepthTestFunc::LESS:
        glDepthFunc(GL_LESS);
        break;
    case DepthTestFunc::EQUAL:
        glDepthFunc(GL_EQUAL);
        break;
    case DepthTestFunc::LEQUAL:
        glDepthFunc(GL_LEQUAL);
        break;
    case DepthTestFunc::GREATER:
        glDepthFunc(GL_GREATER);
        break;
    case DepthTestFunc::NOTEQUAL:
        glDepthFunc(GL_NOTEQUAL);
        break;
    case DepthTestFunc::GEQUAL:
        glDepthFunc(GL_GEQUAL);
        break;
    }
}

void OpenGLRendererAPI::ConfigureStencilTesting(const bool enable, const int writeMask, const int readMask, const StencilTestFunc func, const int ref,
                                                const StencilTestAction stencilFailAction, const StencilTestAction stencilPassDepthFailAction,
                                                const StencilTestAction stencilPassDepthPassAction) {
    if (enable)
        glEnable(GL_STENCIL_TEST);
    else
        glDisable(GL_STENCIL_TEST);

    glStencilMask(writeMask);

    switch (func) {
    case StencilTestFunc::ALWAYS:
        glStencilFunc(GL_ALWAYS, ref, readMask);
        break;
    case StencilTestFunc::NEVER:
        glStencilFunc(GL_NEVER, ref, readMask);
        break;
    case StencilTestFunc::LESS:
        glStencilFunc(GL_LESS, ref, readMask);
        break;
    case StencilTestFunc::EQUAL:
        glStencilFunc(GL_EQUAL, ref, readMask);
        break;
    case StencilTestFunc::LEQUAL:
        glStencilFunc(GL_LEQUAL, ref, readMask);
        break;
    case StencilTestFunc::GREATER:
        glStencilFunc(GL_GREATER, ref, readMask);
        break;
    case StencilTestFunc::NOTEQUAL:
        glStencilFunc(GL_NOTEQUAL, ref, readMask);
        break;
    case StencilTestFunc::GEQUAL:
        glStencilFunc(GL_GEQUAL, ref, readMask);
        break;
    }

    glStencilOp(Utils::StencilActionToGLenum(stencilFailAction), Utils::StencilActionToGLenum(stencilPassDepthFailAction),
                Utils::StencilActionToGLenum(stencilPassDepthPassAction));
}

void OpenGLRendererAPI::ConfigureBlending(const bool enable, const BlendingFunc blendingFunc1, const BlendingFunc blendingFunc2, const BlendingFunc blendingFuncR,
                                          const BlendingFunc blendingFuncG, const BlendingFunc blendingFuncB, const BlendingFunc blendingFuncA) {
    if (enable)
        glEnable(GL_BLEND);
    else
        glDisable(GL_BLEND);

    glBlendFunc(Utils::BlendingFuncToGLenum(blendingFunc1), Utils::BlendingFuncToGLenum(blendingFunc2));
    glBlendFuncSeparate(Utils::BlendingFuncToGLenum(blendingFuncR), Utils::BlendingFuncToGLenum(blendingFuncG), Utils::BlendingFuncToGLenum(blendingFuncB),
                        Utils::BlendingFuncToGLenum(blendingFuncA));
}

void OpenGLRendererAPI::ConfigureCulling(const bool enable, const CullingType type) {
    if (enable)
        glEnable(GL_CULL_FACE);
    else
        glDisable(GL_CULL_FACE);
    switch (type) {
    case CullingType::BACK:
        glCullFace(GL_BACK);
    case CullingType::FRONT:
        glCullFace(GL_FRONT);
    case CullingType::FRONT_AND_BACK:
        glCullFace(GL_FRONT_AND_BACK);
        break;
    }
}

void OpenGLRendererAPI::DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t indexCount) {
    uint32_t count = indexCount == 0 ? vertexArray->GetIndexBuffer()->GetCount() : indexCount;
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
}
