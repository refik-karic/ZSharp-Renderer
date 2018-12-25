#ifndef RENDERER_H
#define RENDERER_H

#include "AssetLoader.h"
#include "Camera.h"
#include "Config.h"
#include "Model.h"
#include "Framebuffer.h"

namespace ZSharp {
class Renderer {
  public:
  Renderer(Config* config);
  ~Renderer();

  void RenderNextFrame();
  Framebuffer* GetFrameBuffer();

  private:
  Camera<float> mCamera;
  Framebuffer mBuffer;
  Model<float> mModel;
  AssetLoader mAssetLoader;

  // For debugging.
  bool mFlip = false;
};
}

#endif
