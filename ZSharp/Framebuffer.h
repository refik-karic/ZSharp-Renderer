#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <cstddef>
#include <cstdint>

#include "Config.h"
#include "ZColor.h"

namespace ZSharp {
class Framebuffer {
  public:
  Framebuffer(Config* config);
  ~Framebuffer();

  void SetPixel(std::size_t x, std::size_t y, ZColor color);
  void Clear(ZColor color);

  std::uint8_t* GetBuffer() {
    return mPixelBuffer;
  }

  std::size_t GetStride() const {
    return mConfig->viewportStride;
  }

  std::size_t GetWidth() const {
    return mConfig->viewportWidth;
  }

  std::size_t GetHeight() const {
    return mConfig->viewportHeight;
  }

  private:
  Config* mConfig;
  std::uint8_t* mPixelBuffer;
};
}

#endif
