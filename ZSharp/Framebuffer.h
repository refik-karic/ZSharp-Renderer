#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <cstdint>

#include "Config.h"

namespace ZSharp {
class Framebuffer {
  public:
  Framebuffer(Config* config);
  ~Framebuffer();

  uint8_t* GetBuffer();
  void SetPixel(uint32_t x, uint32_t y, uint32_t color);
  void Clear(uint32_t color);

  private:
  Config* mConfig;
  uint8_t* mPixelBuffer;
};
}

#endif
