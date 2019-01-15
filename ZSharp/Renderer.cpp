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
    indexBufSize += (mesh.GetTriangleFaceTable().size() * 3);
    vertexBufSize += mesh.GetVertTable().size();
  }

  mIndexBuffer = std::make_shared<IndexBuffer>(indexBufSize);
  mVertexBuffer = std::make_shared<VertexBuffer<float>>(vertexBufSize, 3);
  
  Vec3f_t cameraDefaultPos;
  cameraDefaultPos[0] = 4.0f;
  cameraDefaultPos[1] = 0.0f;
  cameraDefaultPos[2] = 25.0f;
  mCamera.MoveCamera(cameraDefaultPos);
}

void Renderer::RenderNextFrame() {
  using namespace std::chrono_literals;

  // TODO: Figure out why the triangles are showing up in a different orientation this time.
  // This probably has something to do with the new way I'm indexing into the VBO/EBO.

  // Clear the VBO/EBO to ease debugging for now.
  mIndexBuffer->Clear();
  mVertexBuffer->Clear();

  // Copy active geometry into the respective buffers.
  for (Mesh<float>& mesh : mModel.GetMeshData()) {
    // Fill EBO.
    for (std::size_t i = 0; i < mesh.GetTriangleFaceTable().size(); ++i) {
      Triangle<float>& triangle = mesh.GetTriangleFaceTable()[i];
      mIndexBuffer->CopyData(triangle.GetData(), i * 3, 3);
    }

    // Update the size of the EBO.
    mIndexBuffer->SetWorkingSize(mesh.GetTriangleFaceTable().size() * 3);

    // Fill VBO.
    mVertexBuffer->CopyData(mesh.GetVertTable().data(), 0, mesh.GetVertTable().size());

    // Update the size of the VBO.
    mVertexBuffer->SetWorkingSize(mesh.GetVertTable().size());
  }

  // Apply a rotation matrix to the verticies in the model before transforming them to screen space.
  Mat4x4f_t rotationMatrix;
  Mat4x4f_t::SetRotation(rotationMatrix,
                                    static_cast<float>(DegreesToRadians(static_cast<double>(mFrameCount))), 
                                    Mat4x4f_t::Axis::Z);
  mFrameCount++;
  if (mFrameCount > 360) {
    mFrameCount = 0;
  }

  // Apply the rotation transform to each vertex in the VBO.
  for (std::size_t i = 0; i < mVertexBuffer->GetWorkingSize(); i += mVertexBuffer->GetStride()) {
    Vec4f_t vertexVector;
    vertexVector[3] = 1.0F;
    vertexVector.LoadRawData(mVertexBuffer->GetData() + i, 3);
    vertexVector = Mat4x4f_t::ApplyTransform(rotationMatrix, vertexVector);
    vertexVector.StoreRawData(mVertexBuffer->GetData() + i, 3);
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
  mCamera.PerspectiveProjection(*mVertexBuffer, *mIndexBuffer);

  // Draw the primitives onto the framebuffer.
  DrawPrimitives(*mVertexBuffer, *mIndexBuffer, colorRed);

  // Time the frame.
  frameDelta = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - frameStart);
}

Framebuffer* Renderer::GetFrameBuffer() {
  return &mBuffer;
}

template<typename T>
void Renderer::DrawPrimitives(const VertexBuffer<T>& vertexBuffer, const IndexBuffer& indexBuffer, ZColor color) {
  // Iterate over each triangle using the EBO.
  // This means the VBO can have gaps in its data as a result of clipping but those will be skipped
  //    if the indexing here is done purely using the EBO!
  //    (i.e.) indicies can be easily swapped elsewhere without having to rearrange the VBO on each clip operation.
  for (std::size_t i = 0; i < indexBuffer.GetWorkingSize(); i += 3) {
    // Draw line connecting p1 to p2.
    DrawRunSlice(mBuffer,
                 static_cast<std::size_t>(*(vertexBuffer.GetData() + indexBuffer[i])),
                 static_cast<std::size_t>(*(vertexBuffer.GetData() + indexBuffer[i] + 1)),
                 static_cast<std::size_t>(*(vertexBuffer.GetData() + indexBuffer[i + 1])),
                 static_cast<std::size_t>(*(vertexBuffer.GetData() + indexBuffer[i + 1] + 1)),
                 color);

    // Draw line connecting p2 to p3.
    DrawRunSlice(mBuffer,
                 static_cast<std::size_t>(*(vertexBuffer.GetData() + indexBuffer[i + 1])),
                 static_cast<std::size_t>(*(vertexBuffer.GetData() + indexBuffer[i + 1] + 1)),
                 static_cast<std::size_t>(*(vertexBuffer.GetData() + indexBuffer[i + 2])),
                 static_cast<std::size_t>(*(vertexBuffer.GetData() + indexBuffer[i + 2] + 1)),
                 color);

    // Draw line connecting p3 to p1.
    DrawRunSlice(mBuffer,
                 static_cast<std::size_t>(*(vertexBuffer.GetData() + indexBuffer[i + 2])),
                 static_cast<std::size_t>(*(vertexBuffer.GetData() + indexBuffer[i + 2] + 1)),
                 static_cast<std::size_t>(*(vertexBuffer.GetData() + indexBuffer[i])),
                 static_cast<std::size_t>(*(vertexBuffer.GetData() + indexBuffer[i] + 1)),
                 color);
  }
}

}
