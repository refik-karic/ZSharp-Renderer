#include <cstddef>
#include <chrono>
#include <string>

#include "Constants.h"
#include "Renderer.h"
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
    indexBufSize += (mesh.GetTriangleFaceTable().size() * 3) * 4;
    vertexBufSize += (mesh.GetVertTable().size()) * 2;
  }

  mIndexBuffer.resize(indexBufSize);
  mVertexBuffer.resize(vertexBufSize);
  
  ZVector<3, float> cameraDefaultPos;
  cameraDefaultPos[0] = 4.0f;
  cameraDefaultPos[1] = 0.0f;
  cameraDefaultPos[2] = 25.0f;
  mCamera.MoveCamera(cameraDefaultPos);
}

void Renderer::RenderNextFrame() {
  using namespace std::chrono_literals;

  // Copy over the potential models to be displayed.
  Model<float> copyModel = mModel;

  // TODO: Extract the raw vertex and index buffers from the models and pass them into the camera to perform clipping.

  // Apply a rotation matrix to the verticies in the model before transforming them to screen space.
  ZMatrix<4, 4, float> rotationMatrix;
  ZMatrix<4, 4, float>::SetRotation(rotationMatrix, 
                                    static_cast<float>(DegreesToRadians(static_cast<double>(mFrameCount))), 
                                    ZMatrix<4, 4, float>::Axis::Z);
  mFrameCount++;
  if (mFrameCount > 360) {
    mFrameCount = 0;
  }

  for (Mesh<float>& mesh : copyModel.GetMeshData()) {
    for (Triangle<float>& triangle : mesh.GetTriangleFaceTable()) {
      for (std::size_t vertex = 0; vertex < 3; ++vertex) {
        ZVector<4, float> vertexVector;
        vertexVector[3] = 1.0F;
        vertexVector.LoadRawData(mesh.GetVertTable().data() + (triangle.GetIndex(vertex) * 3), 3);
        vertexVector = ZMatrix<4, 4, float>::ApplyTransform(rotationMatrix, vertexVector);
        vertexVector.StoreRawData(mesh.GetVertTable().data() + (triangle.GetIndex(vertex) * 3), 3);
      }
    }
  }

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
  mCamera.PerspectiveProjection(copyModel);

  // Draw the primitives onto the framebuffer.
  DrawPrimitives(copyModel, colorRed);

  // Time the frame.
  frameDelta = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - frameStart);
}

Framebuffer* Renderer::GetFrameBuffer() {
  return &mBuffer;
}

template<typename T>
void Renderer::DrawPrimitives(const Model<T>& model, ZColor color) {
  // Iterate over each mesh in the model.
  for (const Mesh<T>& mesh : model.GetMeshData()) {
    // Iterate over each mesh's primitives.
    for (const Triangle<T>& triangle : mesh.GetTriangleFaceTable()) {
      // Draw line connecting p1 to p2.
      DrawRunSlice(mBuffer, 
                   static_cast<std::size_t>(mesh.GetVertTable()[triangle.GetIndex(0) * 3]),
                   static_cast<std::size_t>(mesh.GetVertTable()[(triangle.GetIndex(0) * 3) + 1]),
                   static_cast<std::size_t>(mesh.GetVertTable()[triangle.GetIndex(1) * 3]),
                   static_cast<std::size_t>(mesh.GetVertTable()[(triangle.GetIndex(1) * 3) + 1]),
                   color);

      // Draw line connecting p2 to p3.
      DrawRunSlice(mBuffer,
                   static_cast<std::size_t>(mesh.GetVertTable()[triangle.GetIndex(1) * 3]),
                   static_cast<std::size_t>(mesh.GetVertTable()[(triangle.GetIndex(1) * 3) + 1]),
                   static_cast<std::size_t>(mesh.GetVertTable()[triangle.GetIndex(2) * 3]),
                   static_cast<std::size_t>(mesh.GetVertTable()[(triangle.GetIndex(2) * 3) + 1]),
                   color);

      // Draw line connecting p3 to p1.
      DrawRunSlice(mBuffer,
                   static_cast<std::size_t>(mesh.GetVertTable()[triangle.GetIndex(2) * 3]),
                   static_cast<std::size_t>(mesh.GetVertTable()[(triangle.GetIndex(2) * 3) + 1]),
                   static_cast<std::size_t>(mesh.GetVertTable()[triangle.GetIndex(0) * 3]),
                   static_cast<std::size_t>(mesh.GetVertTable()[(triangle.GetIndex(0) * 3) + 1]),
                   color);
    }
  }
}

}
