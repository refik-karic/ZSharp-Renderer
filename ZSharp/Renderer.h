#ifndef RENDERER_H
#define RENDERER_H

#include <mutex>

#include "Config.h"

namespace ZSharp {
class Renderer {
  public:
  Renderer(void(*callback)(uint8_t* data), Config* config);
  ~Renderer();

  void Start();
  void Stop();

  private:
  enum class RUN_STATE {
    RUNNING,
    STOPPED
  } mRunState;

  std::mutex mMutex;
  std::thread* mRenderThread;
  Config* mConfig;

  void(*BitmapCallback)(uint8_t* data);

  void MainLoop();
};
}

#endif
