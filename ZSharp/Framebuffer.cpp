#include "Common.h"
#include "Framebuffer.h"
#include "ZConfig.h"

#include "IntelIntrinsics.h"
#include <malloc.h>

#define AVX512_SUPPORTED 0

namespace ZSharp {
Framebuffer::Framebuffer() {
  const ZConfig& config = ZConfig::GetInstance();
  mWidth = config.GetViewportWidth();
  mHeight = config.GetViewportHeight();
  mStride = config.GetViewportStride();
  mTotalSize = config.GetViewportStride() * config.GetViewportHeight();
  mPixelBuffer = static_cast<std::uint8_t*>(_aligned_malloc(mTotalSize, 64));
  mScratchBuffer = static_cast<std::uint8_t*>(_aligned_malloc(64, 64));
}

Framebuffer::~Framebuffer(){
  if(mPixelBuffer != nullptr){
    _aligned_free(mPixelBuffer);
  }

  if (mScratchBuffer != nullptr) {
    _aligned_free(mScratchBuffer);
  }
}

void Framebuffer::SetPixel(std::size_t x, std::size_t y, ZColor color) {
  // Scissor test.
  if (x >= 0 && y >= 0 && x < mWidth && y < mHeight) {
    std::size_t offset = (x * sizeof(std::uint32_t)) + (y * mStride);
    *(reinterpret_cast<std::uint32_t*>(mPixelBuffer + offset)) = color.Color;
  }
}

void Framebuffer::SetRow(std::size_t y, std::size_t x1, std::size_t x2, ZColor color) {
  if (y >= 0 && y < mHeight && x1 >= 0 && x1 < mWidth && x2 >= 0 && x2 < mWidth && x1 < x2) {
    std::size_t offset = (x1 * sizeof(std::uint32_t)) + (y * mStride);
    
    ZSharp::MemsetAny(reinterpret_cast<std::uint32_t*>(mPixelBuffer + offset), 
                      color.Color, 
                      x2 - x1);

    /*for (std::size_t i = x1; i < x2; ++i) {
      *(reinterpret_cast<std::uint32_t*>(mPixelBuffer + offset)) = color.Color;
      offset += 4;
    }*/
  }
}

void Framebuffer::Clear(ZColor color) {
#if AVX512_SUPPORTED
  for (std::size_t i = 0; i < 16; ++i) {
    *(reinterpret_cast<std::uint32_t*>(mScratchBuffer) + i) = color.Color;
  }

  avx512memsetaligned(mPixelBuffer, mScratchBuffer, mTotalSize);
#else
  std::size_t cachedSize = mTotalSize / sizeof(std::uintptr_t);
  std::uintptr_t* pBuf = reinterpret_cast<std::uintptr_t*>(mPixelBuffer);
  std::uintptr_t convColor = static_cast<std::uintptr_t>(color.Color) << 32;

  convColor |= color.Color;

  for (std::size_t i = 0; i < cachedSize; i++) {
    pBuf[i] = convColor;
  }
#endif
}

std::uint8_t* Framebuffer::GetBuffer() {
  return mPixelBuffer;
}

std::size_t Framebuffer::GetSize() const {
  return mTotalSize;
}

}
