#include <cstddef>

#include "GDIWrapper.h"

#include <objbase.h>

// Need to disable this warning in order to compile without warnings in /W4
#pragma warning(push)
#pragma warning(disable:4458)
#include <gdiplus.h>
#pragma warning(pop)

#include <gdiplusinit.h>
#include <gdiplusgraphics.h>
#include <gdiplusheaders.h>
#include <gdipluspixelformats.h>
#include <gdiplustypes.h>

GDIWrapper::GDIWrapper() {
  // Initialize GDI+.
  Gdiplus::GdiplusStartupInput gdiplusStartupInput;
  gdiplusStartupInput.SuppressExternalCodecs = true;
  GdiplusStartup(&mGdiToken, &gdiplusStartupInput, nullptr);
}

GDIWrapper::~GDIWrapper() {
  Gdiplus::GdiplusShutdown(mGdiToken);
}

void GDIWrapper::UpdateWindow(HWND hWnd, ZSharp::Framebuffer& frameData) {
  // Get the current window dimensions.
  RECT activeWindowSize;
  GetClientRect(hWnd, &activeWindowSize);
  Gdiplus::Rect drawRect{
    static_cast<int>(activeWindowSize.left),
    static_cast<int>(activeWindowSize.top),
    static_cast<int>(activeWindowSize.right),
    static_cast<int>(activeWindowSize.bottom)
  };
  
  Gdiplus::Bitmap bitmap(
    drawRect.Width,
    drawRect.Height,
    drawRect.Width * 4,
    PixelFormat32bppARGB,
    reinterpret_cast<BYTE*>(frameData.GetBuffer())
  );

  if(bitmap.RotateFlip(Gdiplus::RotateFlipType::Rotate180FlipNone) != Gdiplus::Status::Ok) {
    return;
  }

  // Use GDI+ to draw the bitmap onto our viewport.
  PAINTSTRUCT ps;
  HDC hdc = BeginPaint(hWnd, &ps);

  if(hdc != nullptr) {
    Gdiplus::Graphics graphics(hdc);
    graphics.DrawImage(&bitmap, drawRect);
    EndPaint(hWnd, &ps);
  }
}
