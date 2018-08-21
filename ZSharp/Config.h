#ifndef CONFIG_H
#define CONFIG_H

#include <cstdint>

namespace ZSharp{
struct Config {
  uint32_t viewportWidth;
  uint32_t viewportHeight;
  uint32_t bytesPerPixel;
  uint32_t viewportStride;
};
}
#endif
