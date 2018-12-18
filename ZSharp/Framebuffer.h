#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <cstdint>

#include "Config.h"
#include "ZColor.h"

namespace ZSharp {
class Framebuffer {
  public:
  Framebuffer(Config* config);
  ~Framebuffer();

  std::uint8_t* GetBuffer();
  void SetPixel(std::size_t x, std::size_t y, ZColor color);
  void Clear(ZColor color);

  std::size_t GetStride() {
    return mConfig->viewportStride;
  }

  std::size_t GetHeight() {
    return mConfig->viewportHeight;
  }

  private:
  Config* mConfig;
  std::uint8_t* mPixelBuffer;
};
}

#endif
