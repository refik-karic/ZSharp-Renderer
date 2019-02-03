#ifndef GDIWRAPPER_H
#define GDIWRAPPER_H

#include "WindowsHeadersWrapper.h"

#include <Framebuffer.h>

class GDIWrapper {
  public:
  GDIWrapper(HWND hwnd);
  ~GDIWrapper();

  GDIWrapper(const GDIWrapper&) = delete;
  void operator=(const GDIWrapper&) = delete;

  void UpdateWindow(const ZSharp::Framebuffer& frameData);

  private:
  HWND mHwnd;
  ULONG_PTR mGdiToken;
};

#endif
