#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

#include "ZColor.h"

namespace ZSharp {
class Framebuffer {
  public:
  Framebuffer();

  void SetPixel(std::size_t x, std::size_t y, ZColor color);
  void Clear(ZColor color);
  const std::uint8_t* GetBuffer() const;
  std::size_t GetSize() const;

  private:
  std::vector<uint8_t> mPixelBuffer;
  std::size_t mWidth;
  std::size_t mHeight;
  std::size_t mBytesPerPixel;
  std::size_t mStride;
  std::size_t mTotalSize;
};
}
