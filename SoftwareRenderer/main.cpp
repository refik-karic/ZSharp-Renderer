#include <InputManager.h>
#include <Renderer.h>
#include <ZConfig.h>

#include "GDIWrapper.h"
#include "WindowsHeadersWrapper.h"

HWND SetupWindow(HINSTANCE hInstance, const wchar_t* className);
LRESULT CALLBACK MessageLoop(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int nCmdShow) {
  wchar_t className[] = L"Test Class Name";
  HWND hwnd = SetupWindow(hInstance, className);
  if (hwnd == nullptr) {
    return HRESULT_FROM_WIN32(GetLastError());
  }

  ShowWindow(hwnd, nCmdShow);
  UpdateWindow(hwnd);
  for (MSG msg; GetMessageW(&msg, hwnd, 0, 0) > 0;) {
    TranslateMessage(&msg);
    DispatchMessageW(&msg);
  }

  UnregisterClassW(className, hInstance);
  return 0;
}

LRESULT CALLBACK MessageLoop(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  static constexpr UINT FRAMERATE_60_HZ_MS = 1000 / 60;
  static UINT_PTR windowsFrameTimer;

  ZSharp::ZConfig& config = ZSharp::ZConfig::GetInstance();
  static ZSharp::Renderer renderer(config.GetViewportWidth(), config.GetViewportHeight(), config.GetViewportStride());

  switch (uMsg) {
    case WM_CREATE:
      windowsFrameTimer = SetTimer(hwnd, 1, FRAMERATE_60_HZ_MS, NULL);

      if(windowsFrameTimer == 0) {
        DestroyWindow(hwnd);
      }
      break;
    case WM_TIMER:
    {
      RECT activeWindowSize;
      GetClientRect(hwnd, &activeWindowSize);
      InvalidateRect(hwnd, &activeWindowSize, false);
    }
      break;
    case WM_PAINT:
    {
      RECT activeWindowSize;
      if (GetClientRect(hwnd, &activeWindowSize)) {
        if (activeWindowSize.right != config.GetViewportWidth()) {
          config.SetViewportWidth(activeWindowSize.right);
        }

        if (activeWindowSize.bottom != config.GetViewportHeight()) {
          config.SetViewportHeight(activeWindowSize.bottom);
        }
      }

      UpdateWindow(hwnd, renderer.RenderNextFrame());
    }
      return 0;
    case WM_ERASEBKGND:
      return true;
      break;
    case WM_LBUTTONDOWN:
    {
      std::int32_t x = LOWORD(lParam);
      std::int32_t y = HIWORD(lParam);
      ZSharp::InputManager* inputManager = ZSharp::InputManager::GetInstance();
      inputManager->UpdateMousePosition(x, y);
      inputManager->UpdateMouseState(true);
    }
      return 0;
    case WM_LBUTTONUP:
    {
      ZSharp::InputManager* inputManager = ZSharp::InputManager::GetInstance();
      inputManager->ResetMouse();
    }
      return 0;
    case WM_MOUSEMOVE:
    {
      std::int32_t x = LOWORD(lParam);
      std::int32_t y = HIWORD(lParam);
      ZSharp::InputManager* inputManager = ZSharp::InputManager::GetInstance();
      inputManager->UpdateMousePosition(x, y);
    }
      return 0;
    case WM_KEYDOWN:
      switch (wParam) {
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
        case VK_ESCAPE:
          DestroyWindow(hwnd);
          break;
        default:
        {
          ZSharp::InputManager* inputManager = ZSharp::InputManager::GetInstance();
          inputManager->Update(static_cast<std::uint8_t>(wParam), ZSharp::InputManager::KeyState::Down);
        }
          break;
      }
      break;
    case WM_KEYUP:
    {
      ZSharp::InputManager* inputManager = ZSharp::InputManager::GetInstance();
      inputManager->Update(static_cast<std::uint8_t>(wParam), ZSharp::InputManager::KeyState::Up);
    }
      break;
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

HWND SetupWindow(HINSTANCE hInstance, const wchar_t* className) {
  WNDCLASSEXW wc{
    sizeof(WNDCLASSEXW),
    CS_HREDRAW | CS_VREDRAW,
    &MessageLoop,
    0,
    0,
    hInstance,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    className,
    nullptr
  };

  if (!RegisterClassExW(&wc)) {
    return nullptr;
  }
  
  const ZSharp::ZConfig& config = ZSharp::ZConfig::GetInstance();

  DWORD windowStyle = WS_BORDER | WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU | WS_THICKFRAME;
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
