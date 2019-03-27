#pragma once

#include "WindowsHeadersWrapper.h"

#include <Framebuffer.h>

class GDIWrapper {
  public:
  GDIWrapper();
  ~GDIWrapper();

  GDIWrapper(const GDIWrapper&) = delete;
  void operator=(const GDIWrapper&) = delete;

  void UpdateWindow(HWND hWnd, ZSharp::Framebuffer& frameData);

  private:
  ULONG_PTR mGdiToken;
};
