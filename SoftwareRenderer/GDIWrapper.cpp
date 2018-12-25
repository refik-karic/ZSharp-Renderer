#include <cstddef>

#include "GDIWrapper.h"

GDIWrapper::GDIWrapper(HWND hwnd, ZSharp::Framebuffer* frameData) :
  mHwnd(hwnd)
{
  // Initialize GDI+.
  Gdiplus::GdiplusStartupInput gdiplusStartupInput;
  gdiplusStartupInput.GdiplusVersion = 1;
  gdiplusStartupInput.DebugEventCallback = nullptr;
  gdiplusStartupInput.SuppressBackgroundThread = false;
  gdiplusStartupInput.SuppressExternalCodecs = false; // Ignored for GDI+ v1.0
  GdiplusStartup(&mGdiToken, &gdiplusStartupInput, nullptr);

  mBitmap = new Gdiplus::Bitmap(static_cast<INT>(frameData->GetWidth()),
                                static_cast<INT>(frameData->GetHeight()),
                                static_cast<INT>(frameData->GetStride()),
                                PixelFormat32bppARGB,
                                frameData->GetBuffer());
}

GDIWrapper::~GDIWrapper() {
  delete mBitmap;
  Gdiplus::GdiplusShutdown(mGdiToken);
}

void GDIWrapper::UpdateWindow() {
  // Get the current window dimensions.
  RECT activeWindowSize;
  GetClientRect(mHwnd, &activeWindowSize);
  Gdiplus::Rect drawRect(static_cast<int>(activeWindowSize.left),
                         static_cast<int>(activeWindowSize.top),
                         static_cast<int>(activeWindowSize.right),
                         static_cast<int>(activeWindowSize.bottom));

  // Use GDI+ to draw the bitmap onto our viewport.
  HDC hdc;
  PAINTSTRUCT ps;
  hdc = BeginPaint(mHwnd, &ps);
  Gdiplus::Graphics graphics(hdc);
  // Note that GDI+ will handle upscaling/downscaling if necessary.
  graphics.DrawImage(mBitmap, drawRect);
  EndPaint(mHwnd, &ps);

  // TODO: This uses lazy "dirty rectangle" drawing and re-draws the entire frame on each pass.
  // It would be much better to track the changed area and only update that.
  InvalidateRect(mHwnd, &activeWindowSize, false);
}
