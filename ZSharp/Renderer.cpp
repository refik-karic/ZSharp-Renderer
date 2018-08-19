#include <chrono>
#include <thread>
#include <cstdint>

#include "Renderer.h"

Renderer::Renderer(void(*callback)()) :
  BitmapCallback(callback),
  mRunState(RUN_STATE::STOPPED),
  mMutex() {

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
  int32_t foo(12);

  // Run as long as the renderer is not told to stop.
  while (mRunState == RUN_STATE::RUNNING) {
    BitmapCallback();
    std::this_thread::sleep_for(2s);
  }
}
