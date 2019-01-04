#include <cstddef>
#include <cstring>

#include "GDIWrapper.h"

#include <objidl.h>
#include <gdiplus.h>
#include <gdiplusinit.h>
#include <gdiplusimaging.h>
#include <gdiplusgraphics.h>
#include <gdiplusheaders.h>
#include <gdipluspixelformats.h>
#include <gdiplustypes.h>

Gdiplus::Bitmap* mBitmap;

GDIWrapper::GDIWrapper(HWND hwnd) :
  mHwnd(hwnd)
{
  // Initialize GDI+.
  Gdiplus::GdiplusStartupInput gdiplusStartupInput;
  gdiplusStartupInput.GdiplusVersion = 1;
  gdiplusStartupInput.DebugEventCallback = nullptr;
  gdiplusStartupInput.SuppressBackgroundThread = false;
  gdiplusStartupInput.SuppressExternalCodecs = false; // Ignored for GDI+ v1.0
  GdiplusStartup(&mGdiToken, &gdiplusStartupInput, nullptr);

  RECT activeWindowSize;
  GetClientRect(mHwnd, &activeWindowSize);

  mBitmap = new Gdiplus::Bitmap(activeWindowSize.right,
                                activeWindowSize.bottom,
                                PixelFormat32bppARGB);
}

GDIWrapper::~GDIWrapper() {
  delete mBitmap;
  Gdiplus::GdiplusShutdown(mGdiToken);
}

void GDIWrapper::UpdateWindow(const ZSharp::Framebuffer& frameData) {
  // Get the current window dimensions.
  RECT activeWindowSize;
  GetClientRect(mHwnd, &activeWindowSize);
  Gdiplus::Rect drawRect(static_cast<int>(activeWindowSize.left),
                         static_cast<int>(activeWindowSize.top),
                         static_cast<int>(activeWindowSize.right),
                         static_cast<int>(activeWindowSize.bottom));
  
  Gdiplus::BitmapData bitmapData;
  Gdiplus::Status lockResult = mBitmap->LockBits(&drawRect, 
                                                 Gdiplus::ImageLockMode::ImageLockModeWrite, 
                                                 PixelFormat32bppARGB, 
                                                 &bitmapData);

  if (lockResult == Gdiplus::Status::Ok) {
    // TODO: For some reason GDI+ will not let me just pass in my own bitmapData struct with a pointer set directly to my framebuffer.
    // Instead I have to waste a lot of time copying data over to the locked region. Need to investigate this further at some point.
    std::memcpy(bitmapData.Scan0, frameData.GetBuffer(), frameData.GetSize());
    mBitmap->UnlockBits(&bitmapData);

    // Use GDI+ to draw the bitmap onto our viewport.
    HDC hdc;
    PAINTSTRUCT ps;
    hdc = BeginPaint(mHwnd, &ps);
    Gdiplus::Graphics graphics(hdc);
    // Note that GDI+ will handle upscaling/downscaling if necessary.
    graphics.DrawImage(mBitmap, drawRect);
    EndPaint(mHwnd, &ps);
  }

  // TODO: This uses lazy "dirty rectangle" drawing and re-draws the entire frame on each pass.
  // It would be much better to track the changed area and only update that.
  InvalidateRect(mHwnd, &activeWindowSize, false);
}
