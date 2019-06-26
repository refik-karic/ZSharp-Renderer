﻿#include <Renderer.h>
#include <ZConfig.h>
#include <ZMatrix.h>

#include "WindowsHeadersWrapper.h"
#include "GDIWrapper.h"

static ZSharp::Renderer* mRenderer = nullptr;
static GDIWrapper* mGdiWrapper = nullptr;

void InitializeRenderer();
HWND SetupWindow(HINSTANCE hInstance, const wchar_t* className);
LRESULT CALLBACK MessageLoop(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int nCmdShow) {
  InitializeRenderer();

  wchar_t className[] = L"Test Class Name";
  HWND hwnd = SetupWindow(hInstance, className);
  if (hwnd == nullptr) {
    return HRESULT_FROM_WIN32(GetLastError());
  }

  mGdiWrapper = new GDIWrapper();

  // Run the message loop.
  ShowWindow(hwnd, nCmdShow);
  UpdateWindow(hwnd);
  MSG msg;
  while (GetMessageW(&msg, hwnd, 0, 0) > 0) {
    // Translate message is required for retrieving decoded user input.
    TranslateMessage(&msg);
    // Dispatch message is required to process the message from the OS.
    DispatchMessageW(&msg);
  }

  UnregisterClassW(className, hInstance);
  return 0;
}

LRESULT CALLBACK MessageLoop(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  static constexpr UINT FRAMERATE_60_HZ_MS = 1000 / 60;
  static UINT_PTR windowsFrameTimer;

  switch (uMsg) {
    case WM_CREATE:
      windowsFrameTimer = SetTimer(hwnd, 1, FRAMERATE_60_HZ_MS, NULL);

      if(windowsFrameTimer == 0){
        DestroyWindow(hwnd);
      }
      break;
    case WM_TIMER:      
      RECT activeWindowSize;
      GetClientRect(hwnd, &activeWindowSize);
      InvalidateRect(hwnd, &activeWindowSize, false);
      break;
    case WM_PAINT:
      mGdiWrapper->UpdateWindow(hwnd, mRenderer->RenderNextFrame());
      break;
    case WM_KEYDOWN:
      // Just treat wParam as an ASCII character press for simplicity at the moment.
      switch (wParam) {
        case 'W':
          mRenderer->MoveCamera(ZSharp::Renderer::Direction::UP, 1.0F);
          break;
        case 'S':
          mRenderer->MoveCamera(ZSharp::Renderer::Direction::DOWN, 1.0F);
          break;
        case 'A':
          mRenderer->MoveCamera(ZSharp::Renderer::Direction::RIGHT, 1.0F);
          break;
        case 'D':
          mRenderer->MoveCamera(ZSharp::Renderer::Direction::LEFT, 1.0F);
          break;
        case 'Q':
          mRenderer->RotateCamera(ZSharp::ZMatrix<4, 4, float>::Axis::Y, 1.0F);
          break;
        case 'E':
          mRenderer->RotateCamera(ZSharp::ZMatrix<4, 4, float>::Axis::Y, -1.0F);
          break;
        case VK_SPACE:
          if(windowsFrameTimer == 0) {
            windowsFrameTimer = SetTimer(hwnd, 1, FRAMERATE_60_HZ_MS, NULL);

            if(windowsFrameTimer == 0){
              DestroyWindow(hwnd);
            }
          }
          else {
            KillTimer(hwnd, windowsFrameTimer);
            windowsFrameTimer = 0;
          }
          break;
        case VK_UP:
          mRenderer->ChangeSpeed(1);
          break;
        case VK_DOWN:
          mRenderer->ChangeSpeed(-1);
          break;
        case VK_ESCAPE:
          DestroyWindow(hwnd);
          break;
      }
      break;
    // TODO: Add a case for WM_CHAR and cast wParam to a wchar_t type.
    // https://docs.microsoft.com/en-us/windows/desktop/learnwin32/keyboard-input#character-messages
    case WM_CLOSE:
      DestroyWindow(hwnd);
      break;
    case WM_DESTROY:
      KillTimer(hwnd, windowsFrameTimer);
      PostQuitMessage(0);
      break;
    case WM_QUIT:
      return wParam;
    default:
      return DefWindowProcW(hwnd, uMsg, wParam, lParam);
  }

  return 0;
}

void InitializeRenderer() {
  ZSharp::ZConfig& config = ZSharp::ZConfig::GetInstance();
  config.SetViewportWidth(640);
  config.SetViewportHeight(360);
  config.SetBytesPerPixel(4);

  mRenderer = new ZSharp::Renderer();
}

HWND SetupWindow(HINSTANCE hInstance, const wchar_t* className) {
  WNDCLASSEXW wc;
  wc.cbSize = sizeof(WNDCLASSEXW);
  wc.style = 0;
  wc.lpfnWndProc = &MessageLoop;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hInstance = hInstance;
  wc.hIcon = nullptr;
  wc.hCursor = nullptr;
  wc.hbrBackground = nullptr;
  wc.lpszMenuName = nullptr;
  wc.lpszClassName = className;
  wc.hIconSm = nullptr;

  if (!RegisterClassExW(&wc)) {
    return nullptr;
  }
  
  const ZSharp::ZConfig& config = ZSharp::ZConfig::GetInstance();

  DWORD windowStyle = WS_BORDER | WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
  RECT clientRect{0L, 0L, static_cast<long>(config.GetViewportWidth()), static_cast<long>(config.GetViewportHeight())};
  AdjustWindowRectEx(&clientRect, windowStyle, false, WS_EX_OVERLAPPEDWINDOW);

  return CreateWindowExW(
    WS_EX_OVERLAPPEDWINDOW,
    className,
    L"Test Window",
    WS_OVERLAPPEDWINDOW | WS_VISIBLE,
    CW_USEDEFAULT,
    CW_USEDEFAULT,
    clientRect.right - clientRect.left,
    clientRect.bottom - clientRect.top,
    nullptr,
    nullptr,
    hInstance,
    nullptr
  );
}
