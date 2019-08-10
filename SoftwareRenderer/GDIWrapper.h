#pragma once

#include <cstdint>

#include "WindowsHeadersWrapper.h"

class GDIWrapper final {
  public:
  GDIWrapper();
  ~GDIWrapper();

  GDIWrapper(const GDIWrapper&) = delete;
  void operator=(const GDIWrapper&) = delete;

  void UpdateWindow(HWND hWnd, std::uint8_t* frameData);

  private:
};
