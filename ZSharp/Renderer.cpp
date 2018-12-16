#include <algorithm>
#include <cstdint>
#include <chrono>
#include <thread>

#include "AssetLoader.h"
#include "Constants.h"
#include "Framebuffer.h"
#include "Model.h"
#include "Renderer.h"
#include "Camera.h"
#include "ZColor.h"
#include "ZMatrix.h"
#include "ZVector.h"

static const std::string ASSET_FILE = "C:\\Users\\kr\\Desktop\\SoftwareRendererV3\\ZSharp\\world_db.json";

namespace ZSharp {
Renderer::Renderer(void(*callback)(std::uint8_t* data), Config* config) :
  BitmapCallback(callback),
  mRunState(RUN_STATE::STOPPED),
  mMutex(),
  mConfig(config),
  mRenderThread(nullptr) {

}

Renderer::~Renderer() {
  if (mRenderThread != nullptr) {
    mRunState = RUN_STATE::STOPPED;
    mRenderThread->join();
    delete mRenderThread;
  }
}

void Renderer::Start() {
  mMutex.lock();

  if (BitmapCallback != nullptr && mRunState != RUN_STATE::RUNNING) {
    // Set the main loop in the running state and let it run.
    mRunState = RUN_STATE::RUNNING;
    mRenderThread = new std::thread(&Renderer::MainLoop, this);
  }

  mMutex.unlock();
}

void Renderer::Stop() {
  mMutex.lock();

  if (mRunState == RUN_STATE::RUNNING) {
    // Flag the main loop to stop running on the next frame.
    mRunState = RUN_STATE::STOPPED;

    // Block until the render thread is finished.
    mRenderThread->join();
    delete mRenderThread;
    mRenderThread = nullptr;
  }

  mMutex.unlock();
}

void Renderer::MainLoop() {
  using namespace std::chrono_literals;

  Framebuffer framebuffer(mConfig);

  // Color is stored in ARGB format.
  ZColor colorRed;
  colorRed.Color = ZColors::RED;

  ZColor colorBlue;
  colorBlue.Color = ZColors::BLUE;

  ZVector<3, float> testVec;
  testVec[0] = 5.0F;

  ZVector<3, float> test2Vec;
  test2Vec[0] = 3.0F;

  Camera<float> camera(mConfig);

  AssetLoader assetLoader;
  Model<float> testModel;
  assetLoader.LoadModel<float>(ASSET_FILE, testModel);

  bool flip = false;

  // Track frame times.
  std::chrono::high_resolution_clock::time_point mainLoopStart(std::chrono::high_resolution_clock::now());
  std::chrono::high_resolution_clock::time_point frameStart;
  std::chrono::milliseconds frameDelta;

  // Run as long as the renderer is not told to stop.
  while (mRunState == RUN_STATE::RUNNING) {
    // Time the start of the current frame.
    frameStart = std::chrono::high_resolution_clock::now();

    ZColor color;

    if (flip) {
      color = colorBlue;
    } else {
      color = colorRed;
    }

    flip = !flip;

    framebuffer.Clear(color);

    // Transform all primitives in the current FOV and get them ready to draw.
    camera.PerspectiveProjection(testModel);

    BitmapCallback(framebuffer.GetBuffer());

    // Time the frame.
    frameDelta = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - frameStart);
    std::chrono::duration<float, std::milli> timeDelta(std::max(Constants::FRAMERATE_60HZ_MS - static_cast<float>(frameDelta.count()), 0.0f));
    std::this_thread::sleep_for(timeDelta);
  }
}
}
