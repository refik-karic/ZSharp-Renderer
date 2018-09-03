#include "GDIWrapper.h"

GDIWrapper::GDIWrapper(HWND hwnd, int32_t width, int32_t height) :
  mMutex(),
  mHwnd(hwnd),
  mRect(0, 0, width, height),
  mStopPaint(false)
{
  // Initialize GDI+.
  Gdiplus::GdiplusStartupInput gdiplusStartupInput;
  GdiplusStartup(&mGdiToken, &gdiplusStartupInput, NULL);

  // Create the bitmap and the data object required to lock/unlock the bitmap for writing.
  mBitmap = new Gdiplus::Bitmap(width, height, PixelFormat32bppARGB);
  mBitmapData = new Gdiplus::BitmapData();
  mSizeBitmap = width * height * 4; // 4 Bytes per pixel.
}

GDIWrapper::~GDIWrapper() {
  delete mBitmapData;
  delete mBitmap;
  Gdiplus::GdiplusShutdown(mGdiToken);
}

uint32_t* GDIWrapper::GetBitmapBuffer() {
  return (uint32_t*)mBitmapData->Scan0;
}

void GDIWrapper::StopPaint() {
  mMutex.lock();

  mStopPaint = true;

  mMutex.unlock();
}

void GDIWrapper::DrawBitmap(uint8_t* frameData) {
  mMutex.lock();
  
  if (mStopPaint) {
    return;
  }

  // Lock the bitmap to get access to the underlying buffer.
  mBitmap->LockBits(&mRect,
                    Gdiplus::ImageLockMode::ImageLockModeWrite,
                    PixelFormat32bppARGB,
                    mBitmapData);

  // Copy over the next frame.
  std::memcpy(mBitmapData->Scan0, frameData, mSizeBitmap);

  // Unlock the bitmap so that it can be drawn to screen.
  mBitmap->UnlockBits(mBitmapData);

  HDC hdc;
  PAINTSTRUCT ps;
  hdc = BeginPaint(mHwnd, &ps);
  Gdiplus::Graphics graphics(hdc);
  graphics.DrawImage(mBitmap, mRect);
  EndPaint(mHwnd, &ps);

  RECT rect;
  rect.left = 0;
  rect.top = 0;
  rect.right = mRect.Width;
  rect.bottom = mRect.Height;

  InvalidateRect(mHwnd, &rect, false);

  mMutex.unlock();
}
