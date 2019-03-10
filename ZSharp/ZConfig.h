#pragma once

#include <cstdint>
#include <cstddef>

namespace ZSharp{

class ZConfig {
  public:

  ZConfig(const ZConfig&) = delete;
  void operator=(const ZConfig&) = delete;

  static ZConfig& GetInstance();

  std::size_t GetViewportWidth() const {
    return mViewportWidth;
  }

  std::size_t GetViewportHeight() const {
    return mViewportHeight;
  }

  std::size_t GetViewportStride() const {
    return mViewportStride;
  }

  std::size_t GetBytesPerPixel() const {
    return mBytesPerPixel;
  }

  void SetViewportWidth(std::size_t width) {
    mViewportWidth = width;
    mViewportStride = mBytesPerPixel * width;
  }

  void SetViewportHeight(std::size_t height) {
    mViewportHeight = height;
  }

  void SetBytesPerPixel(std::size_t bytesPerPixel) {
    mBytesPerPixel = bytesPerPixel;
    mViewportStride = mViewportWidth * bytesPerPixel;
  }

  private:

  ZConfig() {};

  std::size_t mViewportWidth = 0;
  std::size_t mViewportHeight = 0;
  std::size_t mBytesPerPixel = 0;
  std::size_t mViewportStride = 0;
};
}
