#include <chrono>
#include <thread>
#include <cstdint>
#include <algorithm>

#include "Constants.h"
#include "Framebuffer.h"
#include "Renderer.h"
#include "ZVector.h"
#include "ZMatrix.h"

namespace ZSharp {
Renderer::Renderer(void(*callback)(uint8_t* data), Config* config) :
  BitmapCallback(callback),
  mRunState(RUN_STATE::STOPPED),
  mMutex(),
  mConfig(config) {

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
  }

  mMutex.unlock();
}

void Renderer::MainLoop() {
  using namespace std::chrono_literals;

  Framebuffer framebuffer(mConfig);

  // Color is stored in ARGB format.
  uint32_t colorBlue = 0xFF0000FF;
  uint32_t colorRed = 0xFFFF0000;

  ZVector<float> testVec(4);
  testVec[0] = 5.0F;

  ZVector<float> test2Vec(4);
  test2Vec[0] = 3.0F;

  ZMatrix<float> tmpMat(4, 4);
  ZMatrix<float>::Identity(tmpMat);

  bool flip = false;

  // Track frame times.
  std::chrono::high_resolution_clock::time_point mainLoopStart(std::chrono::high_resolution_clock::now());
  std::chrono::high_resolution_clock::time_point frameStart;
  std::chrono::milliseconds frameDelta;

  // Run as long as the renderer is not told to stop.
  while (mRunState == RUN_STATE::RUNNING) {
    // Time the start of the current frame.
    frameStart = std::chrono::high_resolution_clock::now();

    uint32_t color = 0;

    if (flip) {
      color = colorBlue;
    } else {
      color = colorRed;
    }

    flip = !flip;

    framebuffer.Clear(color);

    BitmapCallback(framebuffer.GetBuffer());

    // Time the frame.
    frameDelta = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - frameStart);
    std::chrono::duration<float, std::milli> timeDelta(std::max(FRAMERATE_60HZ_MS - static_cast<float>(frameDelta.count()), 0.0f));
    std::this_thread::sleep_for(timeDelta);
  }
}
}
