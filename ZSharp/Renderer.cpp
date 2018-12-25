#include <cstddef>
#include <chrono>
#include <string>

#include "Constants.h"
#include "Renderer.h"
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
}

Renderer::~Renderer() {

}

void Renderer::RenderNextFrame() {
  using namespace std::chrono_literals;

  // Color is stored in ARGB format.
  ZColor colorRed;
  colorRed.Color = ZColors::RED;

  ZColor colorBlue;
  colorBlue.Color = ZColors::BLUE;

  ZVector<3, float> testVec;
  testVec[0] = 5.0F;

  ZVector<3, float> test2Vec;
  test2Vec[0] = 3.0F;

  // Track frame times.
  std::chrono::high_resolution_clock::time_point mainLoopStart(std::chrono::high_resolution_clock::now());
  std::chrono::high_resolution_clock::time_point frameStart;
  std::chrono::milliseconds frameDelta;

  // Time the start of the current frame.
  frameStart = std::chrono::high_resolution_clock::now();

  ZColor color;

  if (mFlip) {
    color = colorBlue;
  } else {
    color = colorRed;
  }

  mFlip = !mFlip;

  mBuffer.Clear(color);

  // Transform all primitives in the current FOV and get them ready to draw.
  mCamera.PerspectiveProjection(mModel);

  // Time the frame.
  frameDelta = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - frameStart);
}

Framebuffer* Renderer::GetFrameBuffer() {
  return &mBuffer;
}

}
