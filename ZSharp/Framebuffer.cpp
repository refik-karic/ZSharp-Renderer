#include "Framebuffer.h"

namespace ZSharp {
Framebuffer::Framebuffer(Config* config) :
  mConfig(config) {
  mPixelBuffer = new std::uint8_t[config->viewportStride * config->viewportHeight];
}

Framebuffer::~Framebuffer() {
  delete[] mPixelBuffer;
}

void Framebuffer::SetPixel(std::size_t x, std::size_t y, ZColor color) {
  // Scissor test.
  if (x >= 0 && y >= 0 && x < mConfig->viewportWidth && y < mConfig->viewportHeight) {
    std::size_t offset = (x * mConfig->bytesPerPixel) + (y * mConfig->viewportStride);
    *(reinterpret_cast<std::uint32_t*>(mPixelBuffer + offset)) = color.Color;
  }
}

void Framebuffer::Clear(ZColor color) {
  std::size_t cachedSize = (mConfig->viewportStride * mConfig->viewportHeight) / sizeof(std::uintptr_t);
  std::uintptr_t* pBuf = reinterpret_cast<std::uintptr_t*>(mPixelBuffer);
  std::uintptr_t convColor = static_cast<std::uintptr_t>(color.Color);

  // HACK: For 64-bit systems we need to shift the 4 BPP color into the additional register space.
  if (sizeof(std::uintptr_t) > 4) {
    convColor = convColor << 32;
    convColor |= color.Color;
  }

  // TODO: Vectorize this call since it will be used extremely frequently to clear the frame.
  // x86_64 supports SSE2 at a minimum so the XMM registers could prove really useful here!
  // TODO: This loop should use std::size_t because it will match the compilation target native size (32-bit = 32-bit ptr, 64-bit = 64-bit ptr).
  for (std::size_t i = 0; i < cachedSize; i++) {
    pBuf[i] = convColor;
  }
}
}
