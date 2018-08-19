#include "Framebuffer.h"

#include <cstring>

Framebuffer::Framebuffer(Config* config) :
  mConfig(config)
{
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

  for (uint32_t i = 0; i < cachedSize; i++) {
    pBuf[i] = color;
  }
}
