#include "GDIWrapper.h"

GDIWrapper::GDIWrapper(HWND hwnd, std::size_t width, std::size_t height) :
  mMutex(),
  mHwnd(hwnd),
  mRect(0, 0, static_cast<int>(width), static_cast<int>(height)),
  mStopPaint(false)
{
  // Initialize GDI+.
  Gdiplus::GdiplusStartupInput gdiplusStartupInput;
  GdiplusStartup(&mGdiToken, &gdiplusStartupInput, NULL);

  // Create the bitmap and the data object required to lock/unlock the bitmap for writing.
  mBitmap = new Gdiplus::Bitmap(static_cast<INT>(width), static_cast<INT>(height), PixelFormat32bppARGB);
  mBitmapData = new Gdiplus::BitmapData();
  mSizeBitmap = width * height * 4; // 4 Bytes per pixel.
}

GDIWrapper::~GDIWrapper() {
  delete mBitmapData;
  delete mBitmap;
  Gdiplus::GdiplusShutdown(mGdiToken);
}

std::uintptr_t* GDIWrapper::GetBitmapBuffer() {
  return reinterpret_cast<std::uintptr_t*>(mBitmapData->Scan0);
}

void GDIWrapper::StopPaint() {
  mMutex.lock();

  mStopPaint = true;

  mMutex.unlock();
}

void GDIWrapper::DrawBitmap(std::uint8_t* frameData) {
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
  std::size_t cachedSize = mSizeBitmap / sizeof(std::uintptr_t);
  std::uintptr_t* bmpDataPtr = reinterpret_cast<std::uintptr_t*>(mBitmapData->Scan0);
  std::uintptr_t* frameDataPtr = reinterpret_cast<std::uintptr_t*>(frameData);

  for (std::size_t i = 0; i < cachedSize; i++) {
    bmpDataPtr[i] = frameDataPtr[i];
  }

  // Unlock the bitmap so that it can be drawn to screen.
  mBitmap->UnlockBits(mBitmapData);

  HDC hdc;
  PAINTSTRUCT ps;
  hdc = BeginPaint(mHwnd, &ps);
  Gdiplus::Graphics graphics(hdc);
  graphics.DrawImage(mBitmap, mRect);
  EndPaint(mHwnd, &ps);

  // TODO: This uses lazy "dirty rectangle" drawing and re-draws the entire frame on each pass.
  // It would be much better to track the changed area and only update that.

  RECT rect;
  rect.left = 0;
  rect.top = 0;
  rect.right = mRect.Width;
  rect.bottom = mRect.Height;

  InvalidateRect(mHwnd, &rect, false);

  mMutex.unlock();
}
