#pragma once

#include "WindowsHeadersWrapper.h"

#include <cstdint>

class ZSharpApplication {
  public:
  
  static ZSharpApplication& GetInstance();

  static LRESULT CALLBACK MessageLoop(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

  void Run(HINSTANCE instance);

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

  void OnLButtonDown(int32_t x, int32_t y);

  void OnLButtonUp();

  void OnMouseMove(int32_t x, int32_t y);

  void OnKeyDown(uint8_t key);

  void OnKeyUp(uint8_t key);

  void OnClose();

  void OnDestroy();

  // Everything else not related to message loop.

  void UpdateFrame(const uint8_t* data);
};
