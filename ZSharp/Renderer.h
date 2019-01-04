#ifndef RENDERER_H
#define RENDERER_H

#include "AssetLoader.h"
#include "Camera.h"
#include "Framebuffer.h"
#include "Model.h"
#include "ZColor.h"

namespace ZSharp {
class Renderer {
  public:
  Renderer();

  void RenderNextFrame();
  Framebuffer* GetFrameBuffer();

  private:
  Camera<float> mCamera;
  Framebuffer mBuffer;
  Model<float> mModel;
  AssetLoader mAssetLoader;

  std::size_t mFrameCount = 0;

  template<typename T>
  void DrawPrimitives(Model<T>& model, const ZColor& color);
};
}

#endif
