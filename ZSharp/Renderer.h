#pragma once

#include <memory>

#include "AssetLoader.h"
#include "Camera.h"
#include "Framebuffer.h"
#include "Model.h"
#include "ZColor.h"

namespace ZSharp {
class Renderer {
  public:

  enum class Direction {
    LEFT,
    RIGHT,
    UP,
    DOWN
  };

  Renderer();

  Renderer(const Renderer&) = delete;
  void operator=(const Renderer&) = delete;

  void RenderNextFrame();
  Framebuffer& GetFrameBuffer();

  void MoveCamera(Direction direction, float amount);

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
