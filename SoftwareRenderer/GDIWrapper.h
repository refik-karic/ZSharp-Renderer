#ifndef GDIWRAPPER_H
#define GDIWRAPPER_H

#include <cstdint>

#include <windows.h>
#include <objidl.h>
#pragma comment (lib,"Gdiplus.lib")
#include <gdiplus.h>
#include <gdiplusinit.h>
#include <gdiplusimaging.h>
#include <gdiplusgraphics.h>
#include <gdiplusheaders.h>
#include <Gdipluspixelformats.h>
#include <gdiplustypes.h>

#include <mutex>

class GDIWrapper {
  public:
  GDIWrapper(HWND hwnd, int32_t width, int32_t height);
  ~GDIWrapper();

  void StopPaint();
  void DrawBitmap();

  private:
  std::mutex mMutex;
  bool mStopPaint = false;
  HWND mHwnd;
  ULONG_PTR mGdiToken;
  Gdiplus::Rect mRect;
  Gdiplus::Bitmap* mBitmap;
};

#endif
