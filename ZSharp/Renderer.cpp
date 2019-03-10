#include <cstddef>
#include <chrono>
#include <string>

#include "Constants.h"
#include "Renderer.h"
#include "Triangle.h"
#include "UtilMath.h"
#include "ZDrawing.h"
#include "ZColor.h"
#include "ZMatrix.h"
#include "ZVector.h"

static const std::string ASSET_FILE = "C:\\Users\\kr\\Desktop\\SoftwareRendererV3\\ZSharp\\world_db.json";

namespace ZSharp {
Renderer::Renderer() {
  mAssetLoader.LoadModel<float>(ASSET_FILE, mModel);
  
  std::size_t indexBufSize = 0;
  std::size_t vertexBufSize = 0;

  for (Mesh<float>& mesh : mModel.GetMeshData()) {
    // Must account for the possibility of clipping the max amount on each triangle.
    indexBufSize += (mesh.GetTriangleFaceTable().size() * Constants::TRI_VERTS);
    vertexBufSize += mesh.GetVertTable().size();
  }

  mIndexBuffer = std::make_shared<IndexBuffer>(indexBufSize);
  mVertexBuffer = std::make_shared<VertexBuffer<float>>(vertexBufSize, Constants::TRI_VERTS);
  
  Vec3f_t cameraDefaultPos;
  cameraDefaultPos[0] = 0.0f;
  cameraDefaultPos[1] = 0.0f;
  cameraDefaultPos[2] = 15.0f;
  mCamera.MoveCamera(cameraDefaultPos);
}

void Renderer::RenderNextFrame() {
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

  mFrameCount++;
  if (mFrameCount > 360) {
    mFrameCount = 0;
  }

  // Apply rotation transform to all verticies.
  mVertexBuffer->ApplyTransform(rotationMatrix);

  // Color is stored in ARGB format.
  ZColor colorRed;
  colorRed.Color = ZColors::RED;

  ZColor colorBlue;
  colorBlue.Color = ZColors::BLUE;

  // Track frame times.
  std::chrono::high_resolution_clock::time_point mainLoopStart(std::chrono::high_resolution_clock::now());
  std::chrono::high_resolution_clock::time_point frameStart;
  std::chrono::milliseconds frameDelta;

  // Time the start of the current frame.
  frameStart = std::chrono::high_resolution_clock::now();

  // Clear the last frame before drawing the next set of primitives.
  mBuffer.Clear(colorBlue);

  // Transform all primitives in the current FOV and get them ready to draw.
  mCamera.PerspectiveProjection(*mVertexBuffer, *mIndexBuffer);

  // Draw the primitives onto the framebuffer.
  DrawTriangles(mBuffer, *mVertexBuffer, *mIndexBuffer, colorRed);

  // Time the frame.
  frameDelta = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - frameStart);
}

Framebuffer* Renderer::GetFrameBuffer() {
  return &mBuffer;
}

void Renderer::MoveCamera(Direction direction, float amount) {
  ZVector<3, float> currentPositon(mCamera.GetPosition());
  
  switch (direction) {
    case Direction::UP:
      currentPositon[1] = currentPositon[1] + amount;
      break;
    case Direction::DOWN:
      currentPositon[1] = currentPositon[1] - amount;
      break;
    case Direction::LEFT:
      currentPositon[0] = currentPositon[0] + amount;
      break;
    case Direction::RIGHT:
      currentPositon[0] = currentPositon[0] - amount;
      break;
  }

  mCamera.MoveCamera(currentPositon);
}

}
