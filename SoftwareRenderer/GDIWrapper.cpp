#include "GDIWrapper.h"

#include <cstddef>

void UpdateWindow(HWND hWnd, std::uint8_t* frameData) {
  PAINTSTRUCT ps;
  HDC hdc = BeginPaint(hWnd, &ps);
  HDC hdcMem = CreateCompatibleDC(hdc);

  const BITMAP bitmap {
    0,
    ps.rcPaint.right,
    ps.rcPaint.bottom,
    ps.rcPaint.right * 4,
    1,
    32,
    frameData
  };

  HBITMAP hBitmap = CreateBitmapIndirect(&bitmap);
  HGDIOBJ lastObject = SelectObject(hdcMem, hBitmap);
  BitBlt(hdc, 0, 0, ps.rcPaint.right, ps.rcPaint.bottom, hdcMem, 0, 0, SRCCOPY);
  SelectObject(hdcMem, lastObject);

  DeleteDC(hdcMem);
  EndPaint(hWnd, &ps);
  DeleteObject(hBitmap);
}
