#include <cstddef>

#include "AssetLoader.h"
#include "Constants.h"
#include "Renderer.h"
#include "Triangle.h"
#include "UtilMath.h"
#include "ZDrawing.h"
#include "ZColor.h"
#include "ZMatrix.h"
#include "ZVector.h"

static const constexpr char* ASSET_FILE = "C:\\Users\\kr\\Desktop\\SoftwareRendererV3\\ZSharp\\world_db.json";

namespace ZSharp {
Renderer::Renderer() {
  AssetLoader::LoadModel<float>(ASSET_FILE, mModel);
  
  std::size_t indexBufSize = 0;
  std::size_t vertexBufSize = 0;

  for (Mesh<float>& mesh : mModel.GetMeshData()) {
    indexBufSize += (mesh.GetTriangleFaceTable().size() * Constants::TRI_VERTS);
    vertexBufSize += mesh.GetVertTable().size();
  }

  mIndexBuffer = std::make_shared<IndexBuffer>(indexBufSize);
  mVertexBuffer = std::make_shared<VertexBuffer<float>>(vertexBufSize, Constants::TRI_VERTS);
  
  mCameraPos[0] = 0.0f;
  mCameraPos[1] = 0.0f;
  mCameraPos[2] = 35.0f;
}

Framebuffer& Renderer::RenderNextFrame() {
  // Update the camera position.
  mCamera.MoveCamera(mCameraPos);

  // Clear the VBO/EBO to ease debugging for now.
  mIndexBuffer->Clear();
  mVertexBuffer->Clear();

  // Copy active geometry into the respective buffers.
  mModel.FillBuffers(*mVertexBuffer, *mIndexBuffer);

  // Apply a rotation matrix to the verticies in the model before transforming them to screen space.
  Mat4x4f_t rotationMatrix;
  Mat4x4f_t::Identity(rotationMatrix);
  Mat4x4f_t::SetRotation(rotationMatrix,
                                    static_cast<float>(DegreesToRadians(static_cast<double>(mFrameCount))), 
                                    Mat4x4f_t::Axis::Y);

  mFrameCount+=mRotationSpeed;
  if (mFrameCount > 360) {
    mFrameCount = 0;
  }

  // Apply rotation transform to all verticies.
  mVertexBuffer->ApplyTransform(rotationMatrix);

  // Color is stored in ARGB format.
  ZColor colorRed{ZColors::RED};
  ZColor colorBlue{ZColors::BLUE};

  // Clear the last frame before drawing the next set of primitives.
  mBuffer.Clear(colorBlue);

  // Transform all primitives in the current FOV and get them ready to draw.
  mCamera.PerspectiveProjection(*mVertexBuffer, *mIndexBuffer);

  // Draw the primitives onto the framebuffer.
  DrawTriangles(mBuffer, *mVertexBuffer, *mIndexBuffer, colorRed);

  return mBuffer;
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

}
