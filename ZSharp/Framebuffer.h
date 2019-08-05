#pragma once

#include <cstddef>
#include <cstdint>

#include "ZColor.h"

namespace ZSharp {
class Framebuffer final {
  public:
  Framebuffer();
  ~Framebuffer();

  Framebuffer(const Framebuffer&) = delete;
  void operator=(const Framebuffer&) = delete;

  void SetPixel(std::size_t x, std::size_t y, ZColor color);
  void SetRow(std::size_t y, std::size_t x1, std::size_t x2, ZColor color);
  void Clear(ZColor color);
  std::uint8_t* GetBuffer();
  std::size_t GetSize() const;

  private:
  uint8_t* mPixelBuffer = nullptr;
  uint8_t* mScratchBuffer = nullptr;
  std::size_t mWidth;
  std::size_t mHeight;
  std::size_t mStride;
  std::size_t mTotalSize;
};
}
