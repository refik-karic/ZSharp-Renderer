#ifndef RENDERER_H
#define RENDERER_H

#include <mutex>

#include "Framebuffer.h"
#include "Config.h"

namespace ZSharp {
class Renderer {
  public:
  Renderer(Config* config);
  ~Renderer();

  void Start();
  void Stop();
  Framebuffer* GetNextFrame();

  private:
  enum class RUN_STATE {
    RUNNING,
    STOPPED
  } mRunState;

  std::mutex mMutex;
  std::thread* mRenderThread;
  Config* mConfig;
  Framebuffer* mBuffer;

  void MainLoop();
};
}

#endif
