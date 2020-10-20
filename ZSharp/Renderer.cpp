#include <cstddef>

#include <filesystem>

#include "AssetLoader.h"
#include "Constants.h"
#include "Renderer.h"
#include "Triangle.h"
#include "UtilMath.h"
#include "ZColor.h"
#include "ZDrawing.h"
#include "ZMatrix.h"
#include "ZVector.h"

namespace ZSharp {
Renderer::Renderer() {
  std::filesystem::path assetToLoad = std::filesystem::current_path().parent_path().append("pyramids.json");
  AssetLoader::LoadModel<float>(assetToLoad.string().c_str(), mModel);
  
  std::size_t indexBufSize = 0;
  for (Mesh<float>& mesh : mModel.GetMeshData()) {
    indexBufSize += (mesh.GetTriangleFaceTable().size() * Constants::TRI_VERTS);
  }

  mIndexBuffer = std::make_shared<IndexBuffer>(indexBufSize);
  mVertexBuffer = std::make_shared<VertexBuffer<float>>(indexBufSize * Constants::TRI_VERTS, Constants::TRI_VERTS);

  mCameraPos[0] = 0.0f;
  mCameraPos[1] = 0.0f;
  mCameraPos[2] = 35.0f;
}

Renderer& Renderer::GetInstance() {
  static Renderer singleton;
  return singleton;
}

std::uint8_t* Renderer::RenderNextFrame() {
  mCamera.MoveCamera(mCameraPos);

  mIndexBuffer->Clear();
  mVertexBuffer->Clear();

  mModel.FillBuffers(*mVertexBuffer, *mIndexBuffer);

  Mat4x4f_t rotationMatrix;
  Mat4x4f_t::Identity(rotationMatrix);
  Mat4x4f_t::SetRotation(rotationMatrix,
                          static_cast<float>(DegreesToRadians(static_cast<double>(mFrameCount))),
                          Mat4x4f_t::Axis::Y);

  if (!mPauseTransforms) {
    mFrameCount += mRotationSpeed;
  }

  if (mFrameCount > 360) {
    mFrameCount = 0;
  }

  mVertexBuffer->ApplyTransform(rotationMatrix);
  
  ZColor colorRed{ZColors::RED};
  ZColor colorBlue{ZColors::BLUE};

  mBuffer.Clear(colorBlue);

  mCamera.PerspectiveProjection(*mVertexBuffer, *mIndexBuffer);

  if (mRenderMode) {
    ZDrawing::DrawTrianglesFlat(mBuffer, *mVertexBuffer, *mIndexBuffer, colorRed);
  }
  else {
    ZDrawing::DrawTrianglesWireframe(mBuffer, *mVertexBuffer, *mIndexBuffer, colorRed);
  }

  return mBuffer.GetBuffer();
}

void Renderer::MoveCamera(Direction direction, float amount) {
  switch (direction) {
    case Direction::UP:
      mCameraPos[1] -= amount;
      break;
    case Direction::DOWN:
      mCameraPos[1] += amount;
      break;
    case Direction::LEFT:
      mCameraPos[0] -= amount;
      break;
    case Direction::RIGHT:
      mCameraPos[0] += amount;
      break;
  }
}

void Renderer::RotateCamera(Mat4x4f_t::Axis direction, float angleDegrees) {
  Mat4x4f_t rotationMatrix;
  Mat4x4f_t::Identity(rotationMatrix);
  Mat4x4f_t::SetRotation(rotationMatrix,
    static_cast<float>(DegreesToRadians(static_cast<double>(angleDegrees))),
    direction);

  mCamera.RotateCamera(rotationMatrix);
}

void Renderer::ChangeSpeed(std::int64_t amount) {
  if(mRotationSpeed + amount > 10) {
    mRotationSpeed = 10;
  }
  else if(mRotationSpeed + amount < 1) {
    mRotationSpeed = 1;
  }
  else {
    mRotationSpeed += amount;
  }
}

void Renderer::FlipRenderMode() {
  mRenderMode = !mRenderMode;
}

void Renderer::PauseTransforms() {
  mPauseTransforms = !mPauseTransforms;
}

}
