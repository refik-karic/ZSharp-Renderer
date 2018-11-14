#ifndef CONFIG_H
#define CONFIG_H

#include <cstdint>
#include <cstddef>

namespace ZSharp{
struct Config {
  std::size_t viewportWidth;
  std::size_t viewportHeight;
  std::size_t bytesPerPixel;
  std::size_t viewportStride;
};
}
#endif
