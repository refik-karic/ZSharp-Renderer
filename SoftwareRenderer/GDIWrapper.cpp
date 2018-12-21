#include <algorithm>
#include <cstring>
#include <cstddef>

#include "GDIWrapper.h"

GDIWrapper::GDIWrapper(HWND hwnd) :
  mHwnd(hwnd)
{
  // Initialize GDI+.
  Gdiplus::GdiplusStartupInput gdiplusStartupInput;
  GdiplusStartup(&mGdiToken, &gdiplusStartupInput, NULL);

  // Create the bitmap and the data object required to lock/unlock the bitmap for writing.
  RECT activeWindowSize;
  GetClientRect(mHwnd, &activeWindowSize);
  mBitmap = new Gdiplus::Bitmap(activeWindowSize.right, activeWindowSize.bottom, PixelFormat32bppARGB);
}

GDIWrapper::~GDIWrapper() {
  delete mBitmap;
  Gdiplus::GdiplusShutdown(mGdiToken);
}

// TODO: Debug this because it is causing a crash.
void GDIWrapper::DrawBitmap(ZSharp::Framebuffer* nextFrame) {
  // Get the current window dimensions.
  RECT activeWindowSize;
  GetClientRect(mHwnd, &activeWindowSize);
  Gdiplus::Rect drawRect(static_cast<int>(activeWindowSize.left),
                         static_cast<int>(activeWindowSize.top),
                         static_cast<int>(activeWindowSize.right),
                         static_cast<int>(activeWindowSize.bottom));

  // Check for cases where the display window size has chnaged.
  if (mBitmap->GetWidth() != drawRect.Width || mBitmap->GetHeight() != drawRect.Height) {
    delete mBitmap;
    mBitmap = new Gdiplus::Bitmap(drawRect.Width, drawRect.Height, PixelFormat32bppARGB);
  }

  // Lock the bitmap to get access to the underlying buffer.
  Gdiplus::BitmapData bitmapData;
  mBitmap->LockBits(&drawRect,
                    Gdiplus::ImageLockMode::ImageLockModeWrite,
                    PixelFormat32bppARGB,
                    &bitmapData);

  // Copy over the next frame.
  std::size_t numFrameBytes = std::min<std::size_t>(bitmapData.Stride * bitmapData.Height, nextFrame->GetStride() * nextFrame->GetHeight());
  std::memcpy(bitmapData.Scan0, nextFrame->GetBuffer(), numFrameBytes);

  // Unlock the bitmap so that it can be drawn to screen.
  mBitmap->UnlockBits(&bitmapData);

  HDC hdc;
  PAINTSTRUCT ps;
  hdc = BeginPaint(mHwnd, &ps);
  Gdiplus::Graphics graphics(hdc);
  graphics.DrawImage(mBitmap, drawRect);
  EndPaint(mHwnd, &ps);

  // TODO: This uses lazy "dirty rectangle" drawing and re-draws the entire frame on each pass.
  // It would be much better to track the changed area and only update that.
  InvalidateRect(mHwnd, &activeWindowSize, false);
}
