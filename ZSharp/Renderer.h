#pragma once

#include <memory>

#include "Camera.h"
#include "Framebuffer.h"
#include "Model.h"
#include "ZColor.h"
#include "ZMatrix.h"

namespace ZSharp {
class Renderer final {
  public:

  enum class Direction {
    LEFT,
    RIGHT,
    UP,
    DOWN
  };

  Renderer(const Renderer&) = delete;
  void operator=(const Renderer&) = delete;

  static Renderer& GetInstance();

  std::uint8_t* RenderNextFrame();

  void MoveCamera(Direction direction, float amount);

  void RotateCamera(Mat4x4f_t::Axis direction, float angleDegrees);

  void ChangeSpeed(std::int64_t amount);

  void FlipRenderMode();

  void PauseTransforms();

  private:

  Renderer();

  Camera<float> mCamera;
  Vec3f_t mCameraPos;

  Framebuffer mBuffer;
  Model<float> mModel;

  std::int64_t mFrameCount = 0;
  std::int64_t mRotationSpeed = 4;
  bool mRenderMode = false;
  bool mPauseTransforms = false;

  std::shared_ptr<IndexBuffer> mIndexBuffer;
  std::shared_ptr<VertexBuffer<float>> mVertexBuffer;
};
}
