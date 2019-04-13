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

  void ChangeSpeed(std::int64_t amount);

  private:
  Camera<float> mCamera;
  Framebuffer mBuffer;
  Model<float> mModel;

  std::int64_t mFrameCount = 0;
  std::int64_t mRotationSpeed = 4;

  std::shared_ptr<IndexBuffer> mIndexBuffer;
  std::shared_ptr<VertexBuffer<float>> mVertexBuffer;
};
}
