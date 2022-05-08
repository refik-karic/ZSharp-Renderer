#include "ZSharpApplication.h"

#include <InputManager.h>
#include <Renderer.h>
#include <ZConfig.h>

static wchar_t WINDOW_CLASS_NAME[] = L"SoftwareRendererWindowClass";
static wchar_t WINDOW_TITLE[] = L"Software Renderer";
static constexpr UINT FRAMERATE_60_HZ_MS = 1000 / 60;

ZSharpApplication& ZSharpApplication::GetInstance() {
  static ZSharpApplication ZSharpApp;
  return ZSharpApp;
}

LRESULT ZSharpApplication::MessageLoop(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  ZSharpApplication& app = GetInstance();

  switch (uMsg) {
  case WM_CREATE:
    app.OnCreate(hwnd);
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

void ZSharpApplication::Run(HINSTANCE instance) {
  if (mWindowHandle == nullptr) {
    mInstance = instance;

    mWindowHandle = SetupWindow();
    if (mWindowHandle == nullptr) {
      DWORD error = GetLastError();
      HRESULT result = HRESULT_FROM_WIN32(error);
      exit(result);
    }

    ShowWindow(mWindowHandle, SW_SHOW);
    for (MSG msg; GetMessageW(&msg, mWindowHandle, 0, 0) > 0;) {
      TranslateMessage(&msg);
      DispatchMessageW(&msg);
    }

    UnregisterClassW(WINDOW_CLASS_NAME, mInstance);
  }
}

ZSharpApplication::ZSharpApplication() {
  ZeroMemory(&mBitmapInfo, sizeof(BITMAPINFO));
  mBitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFO);
  mBitmapInfo.bmiHeader.biWidth = 0;
  mBitmapInfo.bmiHeader.biHeight = 0;
  mBitmapInfo.bmiHeader.biPlanes = 1;
  mBitmapInfo.bmiHeader.biBitCount = 32;
  mBitmapInfo.bmiHeader.biCompression = BI_RGB;
  mBitmapInfo.bmiHeader.biSizeImage = 0;
  mBitmapInfo.bmiHeader.biXPelsPerMeter = 0;
  mBitmapInfo.bmiHeader.biYPelsPerMeter = 0;
  mBitmapInfo.bmiHeader.biClrUsed = 0;
  mBitmapInfo.bmiHeader.biClrImportant = 0;
}

ZSharpApplication::~ZSharpApplication() {
}

HWND ZSharpApplication::SetupWindow() {
  WNDCLASSEXW wc{
    sizeof(WNDCLASSEXW),
    CS_HREDRAW | CS_VREDRAW,
    &ZSharpApplication::MessageLoop,
    0,
    0,
    mInstance,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    WINDOW_CLASS_NAME,
    nullptr
  };

  if (!RegisterClassExW(&wc)) {
    return nullptr;
  }

  const ZSharp::ZConfig& config = ZSharp::ZConfig::GetInstance();

  DWORD windowStyle = WS_BORDER | WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU | WS_THICKFRAME;
  RECT clientRect{ 0L, 0L, static_cast<long>(config.GetViewportWidth()), static_cast<long>(config.GetViewportHeight()) };
  AdjustWindowRectEx(&clientRect, windowStyle, false, WS_EX_OVERLAPPEDWINDOW);

  return CreateWindowExW(
    WS_EX_OVERLAPPEDWINDOW,
    WINDOW_CLASS_NAME,
    WINDOW_TITLE,
    WS_OVERLAPPEDWINDOW | WS_VISIBLE,
    CW_USEDEFAULT,
    CW_USEDEFAULT,
    clientRect.right - clientRect.left,
    clientRect.bottom - clientRect.top,
    nullptr,
    nullptr,
    mInstance,
    nullptr
  );
}

void ZSharpApplication::OnCreate(HWND initialHandle) {
  mWindowsFrameTimer = SetTimer(initialHandle, 1, FRAMERATE_60_HZ_MS, NULL);

  if (mWindowsFrameTimer == 0) {
    DestroyWindow(initialHandle);
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

    mBitmapInfo.bmiHeader.biWidth = activeWindowSize.right;
    mBitmapInfo.bmiHeader.biHeight = activeWindowSize.bottom;
  }

  UpdateFrame(renderer.RenderNextFrame());
}

void ZSharpApplication::OnLButtonDown(int32_t x, int32_t y) {
  ZSharp::InputManager& inputManager = ZSharp::InputManager::GetInstance();
  inputManager.UpdateMousePosition(x, y);
  inputManager.UpdateMouseState(true);
}

void ZSharpApplication::OnLButtonUp() {
  ZSharp::InputManager& inputManager = ZSharp::InputManager::GetInstance();
  inputManager.ResetMouse();
}

void ZSharpApplication::OnMouseMove(int32_t x, int32_t y) {
  ZSharp::InputManager& inputManager = ZSharp::InputManager::GetInstance();
  inputManager.UpdateMousePosition(x, y);
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
    ZSharp::InputManager& inputManager = ZSharp::InputManager::GetInstance();
    inputManager.Update(key, ZSharp::InputManager::KeyState::Down);
  }
  break;
  }
}

void ZSharpApplication::OnKeyUp(uint8_t key) {
  ZSharp::InputManager& inputManager = ZSharp::InputManager::GetInstance();
  inputManager.Update(key, ZSharp::InputManager::KeyState::Up);
}

void ZSharpApplication::OnClose() {
  DestroyWindow(mWindowHandle);
}

void ZSharpApplication::OnDestroy() {
  KillTimer(mWindowHandle, mWindowsFrameTimer);
  PostQuitMessage(0);
}

void ZSharpApplication::UpdateFrame(const uint8_t* data) {
  PAINTSTRUCT ps;
  HDC hdc = BeginPaint(mWindowHandle, &ps);

  SetDIBitsToDevice(hdc, 
    0, 
    0, 
    mBitmapInfo.bmiHeader.biWidth, 
    mBitmapInfo.bmiHeader.biHeight, 
    0, 
    0,
    0, 
    mBitmapInfo.bmiHeader.biHeight,
    data, 
    &mBitmapInfo, 
    DIB_RGB_COLORS);

  EndPaint(mWindowHandle, &ps);
}
