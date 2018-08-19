#include <chrono>
#include <thread>
#include <cstdint>

#include "Framebuffer.h"
#include "Renderer.h"

Renderer::Renderer(void(*callback)(uint8_t* data), Config* config) :
  BitmapCallback(callback),
  mRunState(RUN_STATE::STOPPED),
  mMutex(),
  mConfig(config)
{

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
  Framebuffer framebuffer(mConfig);

  using namespace std::chrono_literals;
  int32_t foo(12);
  // Color is stored in ARGB format.
  uint32_t colorBlue = 0xFF0000FF;
  uint32_t colorRed = 0xFFFF0000;

  bool flip = false;

  // Run as long as the renderer is not told to stop.
  while (mRunState == RUN_STATE::RUNNING) {
    uint32_t color = 0;
    
    if (flip) {
      color = colorBlue;
    }
    else {
      color = colorRed;
    }

    flip = !flip;

    framebuffer.Clear(color);

    BitmapCallback(framebuffer.GetBuffer());
    std::this_thread::sleep_for(50ms);
  }
}
