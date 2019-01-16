#ifndef RENDERER_H
#define RENDERER_H

#include <memory>

#include "AssetLoader.h"
#include "Camera.h"
#include "Framebuffer.h"
#include "IndexBuffer.h"
#include "Model.h"
#include "VertexBuffer.h"
#include "ZColor.h"

namespace ZSharp {
class Renderer {
  public:
  Renderer();

  void RenderNextFrame();
  Framebuffer* GetFrameBuffer();

  private:
  Camera<float> mCamera;
  Framebuffer mBuffer;
  Model<float> mModel;
  AssetLoader mAssetLoader;

  std::size_t mFrameCount = 0;

  std::shared_ptr<IndexBuffer> mIndexBuffer;
  std::shared_ptr<VertexBuffer<float>> mVertexBuffer;

  template<typename T>
  void DrawTriangles(const VertexBuffer<T>& vertexBuffer, const IndexBuffer& indexBuffer, ZColor color);
};
}

#endif
