#ifndef RENDERER_H
#define RENDERER_H

#include <mutex>

class Renderer {
  public:
  Renderer(void(*callback)());
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

  void(*BitmapCallback)();

  void MainLoop();
};

#endif
