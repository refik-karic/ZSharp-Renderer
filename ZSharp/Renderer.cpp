#include <cstddef>
#include <chrono>

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
  
  Vec3f_t cameraDefaultPos;
  cameraDefaultPos[0] = 0.0f;
  cameraDefaultPos[1] = 0.0f;
  cameraDefaultPos[2] = 35.0f;
  mCamera.MoveCamera(cameraDefaultPos);
}

Framebuffer& Renderer::RenderNextFrame() {
  using namespace std::chrono_literals;

  // Clear the VBO/EBO to ease debugging for now.
  mIndexBuffer->Clear();
  mVertexBuffer->Clear();

  // Copy active geometry into the respective buffers.
  for (Mesh<float>& mesh : mModel.GetMeshData()) {
    // Fill EBO.
    for (std::size_t i = 0; i < mesh.GetTriangleFaceTable().size(); ++i) {
      Triangle<float>& triangle = mesh.GetTriangleFaceTable()[i];
      mIndexBuffer->CopyData(triangle.GetData(), i * Constants::TRI_VERTS, Constants::TRI_VERTS);
    }

    // Update the size of the EBO.
    mIndexBuffer->SetWorkingSize(mesh.GetTriangleFaceTable().size() * Constants::TRI_VERTS);

    // Fill VBO.
    mVertexBuffer->CopyData(mesh.GetVertTable().data(), 0, mesh.GetVertTable().size());

    // Update the size of the VBO.
    mVertexBuffer->SetWorkingSize(mesh.GetVertTable().size());
  }

  // Apply a rotation matrix to the verticies in the model before transforming them to screen space.
  Mat4x4f_t rotationMatrix;
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

  // Track frame times.
  std::chrono::high_resolution_clock::time_point frameStart(std::chrono::high_resolution_clock::now());
  std::chrono::milliseconds frameDelta;

  // Clear the last frame before drawing the next set of primitives.
  mBuffer.Clear(colorBlue);

  // Transform all primitives in the current FOV and get them ready to draw.
  mCamera.PerspectiveProjection(*mVertexBuffer, *mIndexBuffer);

  // Draw the primitives onto the framebuffer.
  DrawTriangles(mBuffer, *mVertexBuffer, *mIndexBuffer, colorRed);

  // Time the frame.
  frameDelta = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - frameStart);

  return mBuffer;
}

void Renderer::MoveCamera(Direction direction, float amount) {
  Vec3f_t currentPositon(mCamera.GetPosition());
  
  switch (direction) {
    case Direction::UP:
      currentPositon[1] -= amount;
      break;
    case Direction::DOWN:
      currentPositon[1] += amount;
      break;
    case Direction::LEFT:
      currentPositon[0] -= amount;
      break;
    case Direction::RIGHT:
      currentPositon[0] += amount;
      break;
  }

  mCamera.MoveCamera(currentPositon);
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
