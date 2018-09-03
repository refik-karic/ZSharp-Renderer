#ifndef GDIWRAPPER_H
#define GDIWRAPPER_H

// Exclude rarely-used stuff from Windows headers.
#define WIN32_LEAN_AND_MEAN

#include <cstdint>

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

#include <mutex>

class GDIWrapper {
  public:
  GDIWrapper(HWND hwnd, int32_t width, int32_t height);
  ~GDIWrapper();

  uint32_t* GetBitmapBuffer();
  void StopPaint();
  void DrawBitmap(uint8_t* frameData);

  private:
  std::mutex mMutex;
  bool mStopPaint;
  HWND mHwnd;
  ULONG_PTR mGdiToken;
  Gdiplus::Rect mRect;
  Gdiplus::Bitmap* mBitmap;
  Gdiplus::BitmapData* mBitmapData;
  uint32_t mSizeBitmap;
};

#endif
