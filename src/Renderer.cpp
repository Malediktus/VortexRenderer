#include <Vortex/Renderer.hpp>

using namespace Vortex;

void Renderer::BeginScene() {
}

void Renderer::EndScene() {
}

void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray) {
    vertexArray->Bind();
    RenderCommand::DrawIndexed(vertexArray);
}
