#include "GDIWrapper.h"

#include <cstddef>

#include <wingdi.h>
#include <WinUser.h>

GDIWrapper::GDIWrapper() {

}

GDIWrapper::~GDIWrapper() {

}

void GDIWrapper::UpdateWindow(HWND hWnd, std::uint8_t* frameData) {
  RECT activeWindowSize;
  GetClientRect(hWnd, &activeWindowSize);

  PAINTSTRUCT ps;
  HDC hdc = BeginPaint(hWnd, &ps);
  HDC hdcMem = CreateCompatibleDC(hdc);

  BITMAP bitmap{
    0,
    activeWindowSize.right,
    activeWindowSize.bottom,
    activeWindowSize.right * 4,
    1,
    32,
    frameData
  };

  HBITMAP hBitmap = CreateBitmapIndirect(&bitmap);
  //HBITMAP hBitmap = CreateBitmap(activeWindowSize.right, activeWindowSize.bottom, 1, 32, frameData);

  HGDIOBJ lastObject = SelectObject(hdcMem, hBitmap);
  BitBlt(hdc, 0, 0, activeWindowSize.right, activeWindowSize.bottom, hdcMem, 0, 0, SRCCOPY);
  SelectObject(hdcMem, lastObject);

  DeleteDC(hdcMem);
  EndPaint(hWnd, &ps);
  DeleteObject(hBitmap);
}
