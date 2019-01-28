// See this for information about Visual Studio C++ XML comments: https://docs.microsoft.com/en-us/cpp/ide/xml-documentation-visual-cpp

// Headers.
#include <Renderer.h>
#include <ZConfig.h>

#include "WindowsHeadersWrapper.h"
#include "GDIWrapper.h"

ZSharp::Renderer* mRenderer = nullptr;

GDIWrapper* mGdiWrapper = nullptr;

const wchar_t mClassName[] = L"Test Class Name";

bool InitializeRenderer();

HWND SetupWindow(HINSTANCE hInstance, const wchar_t* className);

/// <summary>
/// Message loop run on the GUI thread for processing Windows messages to the current Window. 
/// </summary>
LRESULT CALLBACK MessageLoop(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

/// <summary>
/// Main entry point for the application.
/// </summary>
int WinMain(HINSTANCE hInstance, HINSTANCE, PSTR, int nCmdShow) {
  if (!InitializeRenderer()) {
    return -1;
  }
  
  HWND hwnd = SetupWindow(hInstance, mClassName);
  if (hwnd == nullptr) {
    return HRESULT_FROM_WIN32(GetLastError());
  }

  mGdiWrapper = new GDIWrapper(hwnd);

  // Run the message loop.
  MSG msg;
  ShowWindow(hwnd, nCmdShow);
  UpdateWindow(hwnd);

  while (GetMessageW(&msg, hwnd, 0, 0) > 0) {
    // Translate message is required for retrieving decoded user input.
    TranslateMessage(&msg);
    // Dispatch message is required to process the message from the OS.
    DispatchMessageW(&msg);
  }

  UnregisterClassW(mClassName, hInstance);
  delete mGdiWrapper;
  delete mRenderer;

  return 0;
}

LRESULT CALLBACK MessageLoop(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  switch (uMsg) {
    case WM_PAINT:
      mRenderer->RenderNextFrame();
      mGdiWrapper->UpdateWindow(*(mRenderer->GetFrameBuffer()));
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
      }
      break;
    // TODO: Add a case for WM_CHAR and cast wParam to a wchar_t type.
    // https://docs.microsoft.com/en-us/windows/desktop/learnwin32/keyboard-input#character-messages
    case WM_CLOSE:
      DestroyWindow(hwnd);
      break;
    case WM_DESTROY:
      PostQuitMessage(0);
      break;
    case WM_QUIT:
      return wParam;
    default:
      return DefWindowProcW(hwnd, uMsg, wParam, lParam);
  }

  return 0;
}

bool InitializeRenderer() {
  ZSharp::ZConfig& config = ZSharp::ZConfig::GetInstance();
  config.SetBytesPerPixel(4);
  config.SetViewportWidth(640);
  config.SetViewportHeight(480);

  // Prepare renderer and hook GDI+ to its framebuffer.
  mRenderer = new ZSharp::Renderer();

  return true;
}

HWND SetupWindow(HINSTANCE hInstance, const wchar_t* className) {
  WNDCLASSEXW wc;
  wc.cbSize = sizeof(WNDCLASSEXA);
  wc.style = CS_HREDRAW | CS_VREDRAW;
  wc.lpfnWndProc = MessageLoop;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hInstance = hInstance;
  wc.hIcon = 0;
  wc.hCursor = LoadCursorW(nullptr, IDC_ARROW);
  wc.hbrBackground = 0;
  wc.lpszMenuName = 0;
  wc.lpszClassName = className;
  wc.hIconSm = 0;

  if (!RegisterClassExW(&wc)) {
    return nullptr;
  }
  
  ZSharp::ZConfig& config = ZSharp::ZConfig::GetInstance();

  DWORD windowStyle = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
  RECT clientRect;
  clientRect.top = 0;
  clientRect.left = 0;
  clientRect.bottom = static_cast<long>(config.GetViewportHeight());
  clientRect.right = static_cast<long>(config.GetViewportWidth());
  AdjustWindowRectEx(&clientRect, windowStyle, false, 0);

  return CreateWindowExW(
    0, // Optional style.
    className, // Window class.
    L"Test Window", // Window text.
    windowStyle, // Window style.
    // Size and position.
    CW_USEDEFAULT, // X
    CW_USEDEFAULT, // Y
    clientRect.right - clientRect.left, // Width 
    clientRect.bottom - clientRect.top, // Height
    0, // Parent window.
    0, // Menu.
    hInstance, // Instance handle.
    0 // Additional application data.
  );
}
