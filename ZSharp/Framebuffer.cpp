#include "Framebuffer.h"

#include <cstring>

namespace ZSharp {
Framebuffer::Framebuffer(Config* config) :
  mConfig(config) {
  mPixelBuffer = new uint8_t[config->viewportStride * config->viewportHeight];
}

Framebuffer::~Framebuffer() {
  delete[] mPixelBuffer;
}

uint8_t* Framebuffer::GetBuffer() {
  return mPixelBuffer;
}

void Framebuffer::SetPixel(uint32_t x, uint32_t y, uint32_t color) {
  // Scissor test.
  if (x >= 0 && y >= 0 && x < mConfig->viewportWidth && y < mConfig->viewportHeight) {
    uint32_t offset = (x * mConfig->bytesPerPixel) + (y * mConfig->viewportStride);
    mPixelBuffer[offset] = color;
  }
}

void Framebuffer::Clear(uint32_t color) {
  uint32_t cachedSize = mConfig->viewportWidth * mConfig->viewportHeight;
  uint32_t* pBuf = (uint32_t*)mPixelBuffer;

  // TODO: Vectorize this call since it will be used extremely frequently to clear the frame.
  // x86_64 supports SSE2 at a minimum so the XMM registers could prove really useful here!
  // TODO: This loop should use std::size_t because it will match the compilation target native size (32-bit = 32-bit ptr, 64-bit = 64-bit ptr).
  for (uint32_t i = 0; i < cachedSize; i++) {
    pBuf[i] = color;
  }
}
}
