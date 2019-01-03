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
Renderer::Renderer(Config* config) :
  mCamera(config),
  mBuffer(config)
{
  mAssetLoader.LoadModel<float>(ASSET_FILE, mModel);
  ZVector<3, float> cameraDefaultPos;
  cameraDefaultPos[0] = 4.0f;
  cameraDefaultPos[1] = 0.0f;
  cameraDefaultPos[2] = 25.0f;
  mCamera.MoveCamera(cameraDefaultPos);
}

Renderer::~Renderer() {

}

void Renderer::RenderNextFrame() {
  using namespace std::chrono_literals;

  // Copy over the potential models to be displayed.
  Model<float> copyModel = mModel;

  // Apply a rotation matrix to the verticies in the model before transforming them to screen space.
  ZMatrix<4, 4, float> rotationMatrix;
  ZMatrix<4, 4, float>::SetRotation(rotationMatrix, 
                                    static_cast<float>(DegreesToRadians(static_cast<double>(mFrameCount))), 
                                    ZMatrix<4, 4, float>::Axis::Z);
  mFrameCount++;
  if (mFrameCount > 360) {
    mFrameCount = 0;
  }

  // Iterate over each mesh in the model.
  for (std::size_t meshIdx = 0; meshIdx < copyModel.MeshCount(); meshIdx++) {
    // Iterate over each mesh's primitives.
    for (std::size_t triIdx = 0; triIdx < copyModel[meshIdx].GetTriangleFaceTable().size(); triIdx++) {
      // Get the triangle indicies for the current primitive in the mesh.
      Triangle<float>& curTriangle = copyModel[meshIdx].GetTriangleFaceTable()[triIdx];

      // Get the data from the model.
      ZVector<4, float> p1;
      p1[0] = copyModel[meshIdx].GetVertTable()[curTriangle.GetIndex(0) * 3];
      p1[1] = copyModel[meshIdx].GetVertTable()[(curTriangle.GetIndex(0) * 3) + 1];
      p1[2] = copyModel[meshIdx].GetVertTable()[(curTriangle.GetIndex(0) * 3) + 2];
      p1[3] = 1.0F;

      ZVector<4, float> p2;
      p2[0] = copyModel[meshIdx].GetVertTable()[curTriangle.GetIndex(1) * 3];
      p2[1] = copyModel[meshIdx].GetVertTable()[(curTriangle.GetIndex(1) * 3) + 1];
      p2[2] = copyModel[meshIdx].GetVertTable()[(curTriangle.GetIndex(1) * 3) + 2];
      p2[3] = 1.0F;

      ZVector<4, float> p3;
      p3[0] = copyModel[meshIdx].GetVertTable()[curTriangle.GetIndex(2) * 3];
      p3[1] = copyModel[meshIdx].GetVertTable()[(curTriangle.GetIndex(2) * 3) + 1];
      p3[2] = copyModel[meshIdx].GetVertTable()[(curTriangle.GetIndex(2) * 3) + 2];
      p3[3] = 1.0F;

      // Apply the rotation matrix to the vectors.
      p1 = ZMatrix<4, 4, float>::ApplyTransform(rotationMatrix, p1);
      p2 = ZMatrix<4, 4, float>::ApplyTransform(rotationMatrix, p2);
      p3 = ZMatrix<4, 4, float>::ApplyTransform(rotationMatrix, p3);

      // Write the data back to the model verticies.
      copyModel[meshIdx].GetVertTable()[curTriangle.GetIndex(0) * 3] = p1[0];
      copyModel[meshIdx].GetVertTable()[(curTriangle.GetIndex(0) * 3) + 1] = p1[1];
      copyModel[meshIdx].GetVertTable()[(curTriangle.GetIndex(0) * 3) + 2] = p1[2];

      copyModel[meshIdx].GetVertTable()[curTriangle.GetIndex(1) * 3] = p2[0];
      copyModel[meshIdx].GetVertTable()[(curTriangle.GetIndex(1) * 3) + 1] = p2[1];
      copyModel[meshIdx].GetVertTable()[(curTriangle.GetIndex(1) * 3) + 2] = p2[2];

      copyModel[meshIdx].GetVertTable()[curTriangle.GetIndex(2) * 3] = p3[0];
      copyModel[meshIdx].GetVertTable()[(curTriangle.GetIndex(2) * 3) + 1] = p3[1];
      copyModel[meshIdx].GetVertTable()[(curTriangle.GetIndex(2) * 3) + 2] = p3[2];
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
void Renderer::DrawPrimitives(Model<T>& model, const ZColor& color) {
  // Iterate over each mesh in the model.
  for (std::size_t meshIdx = 0; meshIdx < model.MeshCount(); meshIdx++) {
    // Iterate over each mesh's primitives.
    for (std::size_t triIdx = 0; triIdx < model[meshIdx].GetTriangleFaceTable().size(); triIdx++) {
      // Get the triangle indicies for the current primitive in the mesh.
      Triangle<T>& curTriangle = model[meshIdx].GetTriangleFaceTable()[triIdx];

      // Draw line connecting p1 to p2.
      DrawRunSlice(mBuffer, 
                   static_cast<std::size_t>(model[meshIdx].GetVertTable()[curTriangle.GetIndex(0) * 3]), 
                   static_cast<std::size_t>(model[meshIdx].GetVertTable()[(curTriangle.GetIndex(0) * 3) + 1]),
                   static_cast<std::size_t>(model[meshIdx].GetVertTable()[curTriangle.GetIndex(1) * 3]),
                   static_cast<std::size_t>(model[meshIdx].GetVertTable()[(curTriangle.GetIndex(1) * 3) + 1]),
                   color);

      // Draw line connecting p2 to p3.
      DrawRunSlice(mBuffer,
                   static_cast<std::size_t>(model[meshIdx].GetVertTable()[curTriangle.GetIndex(1) * 3]),
                   static_cast<std::size_t>(model[meshIdx].GetVertTable()[(curTriangle.GetIndex(1) * 3) + 1]),
                   static_cast<std::size_t>(model[meshIdx].GetVertTable()[curTriangle.GetIndex(2) * 3]),
                   static_cast<std::size_t>(model[meshIdx].GetVertTable()[(curTriangle.GetIndex(2) * 3) + 1]),
                   color);

      // Draw line connecting p3 to p1.
      DrawRunSlice(mBuffer,
                   static_cast<std::size_t>(model[meshIdx].GetVertTable()[curTriangle.GetIndex(2) * 3]),
                   static_cast<std::size_t>(model[meshIdx].GetVertTable()[(curTriangle.GetIndex(2) * 3) + 1]),
                   static_cast<std::size_t>(model[meshIdx].GetVertTable()[curTriangle.GetIndex(0) * 3]),
                   static_cast<std::size_t>(model[meshIdx].GetVertTable()[(curTriangle.GetIndex(0) * 3) + 1]),
                   color);
    }
  }
}

}
