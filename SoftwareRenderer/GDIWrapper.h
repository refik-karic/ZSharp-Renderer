#pragma once

#include <cstdint>

#include "WindowsHeadersWrapper.h"

class GDIWrapper final {
  public:
  GDIWrapper() = delete;
  ~GDIWrapper() = delete;
  GDIWrapper(const GDIWrapper&) = delete;
  void operator=(const GDIWrapper&) = delete;

  static void UpdateWindow(HWND hWnd, std::uint8_t* frameData);

  private:
};
