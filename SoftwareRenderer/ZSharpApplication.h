#pragma once

#include "WindowsHeadersWrapper.h"

#include <ZBaseTypes.h>

class ZSharpApplication {
  public:
  
  static ZSharpApplication& GetInstance();

  static LRESULT CALLBACK MessageLoop(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

  int Run(HINSTANCE instance);

  private:
  HINSTANCE mInstance = nullptr;
  HWND mWindowHandle = nullptr;
  UINT_PTR mWindowsFrameTimer = 0;

  BITMAPINFO mBitmapInfo;

  ZSharpApplication();

  ~ZSharpApplication();

  HWND SetupWindow();

  // Win32 message loop handlers.

  void OnCreate(HWND initialHandle);

  void OnTimer();

  void OnPaint();

  void OnLButtonDown(ZSharp::int32 x, ZSharp::int32 y);

  void OnLButtonUp();

  void OnMouseMove(ZSharp::int32 x, ZSharp::int32 y);

  void OnKeyDown(ZSharp::uint8 key);

  void OnKeyUp(ZSharp::uint8 key);

  void OnClose();

  void OnDestroy();

  // Everything else not related to message loop.

  void UpdateFrame(const ZSharp::uint8* data);
};
