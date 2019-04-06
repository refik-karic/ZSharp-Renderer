#pragma once

#include <memory>

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

  Framebuffer& RenderNextFrame();

  void MoveCamera(Direction direction, float amount);

  private:
  Camera<float> mCamera;
  Framebuffer mBuffer;
  Model<float> mModel;

  std::size_t mFrameCount = 0;

  std::shared_ptr<IndexBuffer> mIndexBuffer;
  std::shared_ptr<VertexBuffer<float>> mVertexBuffer;
};
}
