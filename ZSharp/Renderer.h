#ifndef RENDERER_H
#define RENDERER_H

#include <memory>

#include "AssetLoader.h"
#include "Camera.h"
#include "Framebuffer.h"
#include "Model.h"
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
};
}

#endif
