#include "Framebuffer.h"
#include "ZConfig.h"

namespace ZSharp {
Framebuffer::Framebuffer() {
  const ZConfig& config = ZConfig::GetInstance();
  mWidth = config.GetViewportWidth();
  mHeight = config.GetViewportHeight();
  mStride = config.GetViewportStride();
  mTotalSize = config.GetViewportStride() * config.GetViewportHeight();
  mPixelBuffer = new uint8_t[mTotalSize];
}

Framebuffer::~Framebuffer(){
  if(mPixelBuffer != nullptr){
    delete[] mPixelBuffer;
  }
}

void Framebuffer::SetPixel(std::size_t x, std::size_t y, ZColor color) {
  // Scissor test.
  if (x >= 0 && y >= 0 && x < mWidth && y < mHeight) {
    std::size_t offset = (x * sizeof(std::uint32_t)) + (y * mStride);
    *(reinterpret_cast<std::uint32_t*>(mPixelBuffer + offset)) = color.Color;
  }
}

void Framebuffer::Clear(ZColor color) {
  std::size_t cachedSize = mTotalSize / sizeof(std::uintptr_t);
  std::uintptr_t* pBuf = reinterpret_cast<std::uintptr_t*>(mPixelBuffer);
  std::uintptr_t convColor = static_cast<std::uintptr_t>(color.Color);

  // TODO: Add a conditional #ifdef here to detect 64-bit.
  convColor = convColor << 32;
  convColor |= color.Color;

  // TODO: Vectorize this call since it will be used extremely frequently to clear the frame.
  // x86_64 supports SSE2 at a minimum so the XMM registers could prove really useful here!
  for (std::size_t i = 0; i < cachedSize; i++) {
    pBuf[i] = convColor;
  }
}

std::uint8_t* Framebuffer::GetBuffer() {
  return mPixelBuffer;
}

std::size_t Framebuffer::GetSize() const {
  return mTotalSize;
}

}
