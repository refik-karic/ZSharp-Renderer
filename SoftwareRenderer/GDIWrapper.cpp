#include "GDIWrapper.h"

GDIWrapper::GDIWrapper(HWND hwnd, int32_t width, int32_t height) :
  mMutex(),
  mHwnd(hwnd),
  mRect(0, 0, width, height)
{
  // Initialize GDI+.
  Gdiplus::GdiplusStartupInput gdiplusStartupInput;
  GdiplusStartup(&mGdiToken, &gdiplusStartupInput, NULL);

  mBitmap = new Gdiplus::Bitmap(width, height, PixelFormat32bppARGB);
  
  
  Gdiplus::BitmapData* bmpData(new Gdiplus::BitmapData());

  mBitmap->LockBits(&mRect,
                    Gdiplus::ImageLockMode::ImageLockModeWrite,
                    PixelFormat32bppARGB,
                    bmpData);

  uint8_t* pData((uint8_t*)bmpData->Scan0);

  for (uint32_t i = 0; i < bmpData->Height; i++) {
    uint32_t rowOffset = i * bmpData->Stride;

    for (uint32_t j = rowOffset; j < rowOffset + bmpData->Stride; j+=4) {
      pData[j] = 0xFF; // B
      pData[j + 1] = 0x00; // G
      pData[j + 2] = 0x00; // R
      pData[j + 3] = 0xFF; // A
    }
  }

  mBitmap->UnlockBits(bmpData);

  delete bmpData;

  DrawBitmap();
}

GDIWrapper::~GDIWrapper() {
  delete mBitmap;
  Gdiplus::GdiplusShutdown(mGdiToken);
}

void GDIWrapper::StopPaint() {
  mMutex.lock();

  mStopPaint = true;

  mMutex.unlock();
}

void GDIWrapper::DrawBitmap() {
  mMutex.lock();
  
  if (mStopPaint) {
    return;
  }

  HDC hdc;
  PAINTSTRUCT ps;

  hdc = BeginPaint(mHwnd, &ps);

  Gdiplus::Graphics graphics(hdc);
  graphics.DrawImage(mBitmap, mRect);

  EndPaint(mHwnd, &ps);

  mMutex.unlock();
}
