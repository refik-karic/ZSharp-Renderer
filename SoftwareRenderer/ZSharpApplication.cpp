#include "ZSharpApplication.h"

#include <InputManager.h>
#include <Renderer.h>
#include <ZConfig.h>

static constexpr UINT FRAMERATE_60_HZ_MS = 1000 / 60;

ZSharpApplication& ZSharpApplication::GetInstance() {
  static ZSharpApplication ZSharpApp;
  return ZSharpApp;
}

LRESULT ZSharpApplication::MessageLoop(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  ZSharpApplication& app = GetInstance();
  app.SetWindowHandle(hwnd);

  switch (uMsg) {
  case WM_CREATE:
    app.OnCreate();
    break;
  case WM_TIMER:
    app.OnTimer();
    break;
  case WM_PAINT:
    app.OnPaint();
    return 0;
  case WM_ERASEBKGND:
    return true;
  case WM_LBUTTONDOWN:
    app.OnLButtonDown(LOWORD(lParam), HIWORD(lParam));
    return 0;
  case WM_LBUTTONUP:
    app.OnLButtonUp();
    return 0;
  case WM_MOUSEMOVE:
    app.OnMouseMove(LOWORD(lParam), HIWORD(lParam));
    return 0;
  case WM_KEYDOWN:
    app.OnKeyDown(static_cast<uint8_t>(wParam));
    break;
  case WM_KEYUP:
    app.OnKeyUp(static_cast<uint8_t>(wParam));
    break;
  case WM_CLOSE:
    app.OnClose();
    break;
  case WM_DESTROY:
    app.OnDestroy();
    break;
  case WM_QUIT:
    return wParam;
  default:
    return DefWindowProcW(hwnd, uMsg, wParam, lParam);
  }

  return 0;
}

ZSharpApplication::ZSharpApplication() {
}

void ZSharpApplication::SetWindowHandle(HWND handle) {
  mWindowHandle = handle;
}

void ZSharpApplication::OnCreate() {
  mWindowsFrameTimer = SetTimer(mWindowHandle, 1, FRAMERATE_60_HZ_MS, NULL);

  if (mWindowsFrameTimer == 0) {
    DestroyWindow(mWindowHandle);
  }
}

void ZSharpApplication::OnTimer() {
  RECT activeWindowSize;
  GetClientRect(mWindowHandle, &activeWindowSize);
  InvalidateRect(mWindowHandle, &activeWindowSize, false);
}

void ZSharpApplication::OnPaint() {
  ZSharp::ZConfig& config = ZSharp::ZConfig::GetInstance();
  static ZSharp::Renderer renderer(config.GetViewportWidth(), config.GetViewportHeight(), config.GetViewportStride());

  RECT activeWindowSize;
  if (GetClientRect(mWindowHandle, &activeWindowSize)) {
    if (activeWindowSize.right != config.GetViewportWidth()) {
      config.SetViewportWidth(activeWindowSize.right);
    }

    if (activeWindowSize.bottom != config.GetViewportHeight()) {
      config.SetViewportHeight(activeWindowSize.bottom);
    }
  }

  UpdateFrame(renderer.RenderNextFrame());
}

void ZSharpApplication::OnLButtonDown(int32_t x, int32_t y) {
  ZSharp::InputManager* inputManager = ZSharp::InputManager::GetInstance();
  inputManager->UpdateMousePosition(x, y);
  inputManager->UpdateMouseState(true);
}

void ZSharpApplication::OnLButtonUp() {
  ZSharp::InputManager* inputManager = ZSharp::InputManager::GetInstance();
  inputManager->ResetMouse();
}

void ZSharpApplication::OnMouseMove(int32_t x, int32_t y) {
  ZSharp::InputManager* inputManager = ZSharp::InputManager::GetInstance();
  inputManager->UpdateMousePosition(x, y);
}

void ZSharpApplication::OnKeyDown(uint8_t key) {
  switch (key) {
  case VK_SPACE:
    if (mWindowsFrameTimer == 0) {
      mWindowsFrameTimer = SetTimer(mWindowHandle, 1, FRAMERATE_60_HZ_MS, NULL);

      if (mWindowsFrameTimer == 0) {
        DestroyWindow(mWindowHandle);
      }
    }
    else {
      KillTimer(mWindowHandle, mWindowsFrameTimer);
      mWindowsFrameTimer = 0;
    }
    break;
  case VK_ESCAPE:
    DestroyWindow(mWindowHandle);
    break;
  default:
  {
    ZSharp::InputManager* inputManager = ZSharp::InputManager::GetInstance();
    inputManager->Update(key, ZSharp::InputManager::KeyState::Down);
  }
  break;
  }
}

void ZSharpApplication::OnKeyUp(uint8_t key) {
  ZSharp::InputManager* inputManager = ZSharp::InputManager::GetInstance();
  inputManager->Update(key, ZSharp::InputManager::KeyState::Up);
}

void ZSharpApplication::OnClose() {
  DestroyWindow(mWindowHandle);
}

void ZSharpApplication::OnDestroy() {
  KillTimer(mWindowHandle, mWindowsFrameTimer);
  PostQuitMessage(0);
}

void ZSharpApplication::UpdateFrame(uint8_t* frameData) {
  PAINTSTRUCT ps;
  HDC hdc = BeginPaint(mWindowHandle, &ps);
  HDC hdcMem = CreateCompatibleDC(hdc);

  const BITMAP bitmap{
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
  EndPaint(mWindowHandle, &ps);
  DeleteObject(hBitmap);
}
