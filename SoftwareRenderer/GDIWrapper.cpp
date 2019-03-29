#include <cstddef>
#include <cstring>

#include "GDIWrapper.h"

// Need to disable this warning in order to compile without warnings in /W4
#pragma warning(push)
#pragma warning(disable:4458)

#include <objidl.h>
#include <gdiplus.h>
#include <gdiplusinit.h>
#include <gdiplusimaging.h>
#include <gdiplusgraphics.h>
#include <gdiplusheaders.h>
#include <gdipluspixelformats.h>
#include <gdiplustypes.h>

#pragma warning(pop)

GDIWrapper::GDIWrapper()
{
  // Initialize GDI+.
  Gdiplus::GdiplusStartupInput gdiplusStartupInput;
  gdiplusStartupInput.GdiplusVersion = 1;
  gdiplusStartupInput.DebugEventCallback = nullptr;
  gdiplusStartupInput.SuppressBackgroundThread = false;
  gdiplusStartupInput.SuppressExternalCodecs = false; // Ignored for GDI+ v1.0
  GdiplusStartup(&mGdiToken, &gdiplusStartupInput, nullptr);
}

GDIWrapper::~GDIWrapper() {
  Gdiplus::GdiplusShutdown(mGdiToken);
}

void GDIWrapper::UpdateWindow(HWND hWnd, ZSharp::Framebuffer& frameData) {
  // Get the current window dimensions.
  RECT activeWindowSize;
  GetClientRect(hWnd, &activeWindowSize);
  Gdiplus::Rect drawRect(static_cast<int>(activeWindowSize.left),
                         static_cast<int>(activeWindowSize.top),
                         static_cast<int>(activeWindowSize.right),
                         static_cast<int>(activeWindowSize.bottom));
  
  //Gdiplus::BitmapData bitmapData;
  Gdiplus::Bitmap bitmap(drawRect.Width, 
    drawRect.Height,
    drawRect.Width * 4,
    PixelFormat32bppARGB, 
    static_cast<BYTE*>(frameData.GetBuffer()));

  // Use GDI+ to draw the bitmap onto our viewport.
  HDC hdc;
  PAINTSTRUCT ps;
  hdc = BeginPaint(hWnd, &ps);

  if(hdc != nullptr) {
    Gdiplus::Graphics graphics(hdc);
    // TODO: Figure out a way to rotate the bitmap 180 degrees quickly since it is being drawn upside down right now.
    graphics.DrawImage(&bitmap, drawRect);
  }

  EndPaint(hWnd, &ps);
}
