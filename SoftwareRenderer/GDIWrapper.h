#ifndef GDIWRAPPER_H
#define GDIWRAPPER_H

// Exclude rarely-used stuff from Windows headers.
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winuser.h>
#include <objidl.h>
#pragma comment (lib,"Gdiplus.lib")
#include <gdiplus.h>
#include <gdiplusinit.h>
#include <gdiplusimaging.h>
#include <gdiplusgraphics.h>
#include <gdiplusheaders.h>
#include <Gdipluspixelformats.h>
#include <gdiplustypes.h>

#include <Framebuffer.h>

class GDIWrapper {
  public:
  GDIWrapper(HWND hwnd, ZSharp::Framebuffer* frameData);
  ~GDIWrapper();

  void UpdateWindow();

  private:
  HWND mHwnd;
  ULONG_PTR mGdiToken;
  Gdiplus::Bitmap* mBitmap;
};

#endif
