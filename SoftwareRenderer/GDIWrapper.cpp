#include "GDIWrapper.h"

#include <objbase.h>

// Need to disable this warning in order to compile without warnings in /W4
#pragma warning(push)
#pragma warning(disable:4458)
#include <gdiplus.h>
#pragma warning(pop)

#include <gdiplusinit.h>
#include <gdiplusenums.h>
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

void GDIWrapper::UpdateWindow(HWND hWnd, std::uint8_t* frameData) {
  // Get the current window dimensions.
  RECT activeWindowSize;
  GetClientRect(hWnd, &activeWindowSize);

  Gdiplus::Bitmap bitmap(
    activeWindowSize.right,
    activeWindowSize.bottom,
    activeWindowSize.right * 4,
    PixelFormat32bppPARGB,
    reinterpret_cast<BYTE*>(frameData)
  );

  // Use GDI+ to draw the bitmap onto our viewport.
  PAINTSTRUCT ps;
  Gdiplus::Graphics graphics(BeginPaint(hWnd, &ps));
  graphics.DrawImage(&bitmap,
                      0,
                      0,
                      activeWindowSize.left,
                      activeWindowSize.top,
                      activeWindowSize.right,
                      activeWindowSize.bottom,
                      Gdiplus::UnitPixel);
  EndPaint(hWnd, &ps);
}
