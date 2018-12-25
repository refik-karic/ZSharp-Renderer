// See this for information about Visual Studio C++ XML comments: https://docs.microsoft.com/en-us/cpp/ide/xml-documentation-visual-cpp

#ifndef UNICODE
#define UNICODE
#endif

// Exclude rarely-used stuff from Windows headers.
#define WIN32_LEAN_AND_MEAN

// Headers.
#include <windows.h>
#include <Renderer.h>
#include "GDIWrapper.h"

// Global variables.
/// <summary>
/// Global config passed between objects as needed to acess common settings.
/// </summary>
ZSharp::Config mConfig;

ZSharp::Renderer* mRenderer = nullptr;

GDIWrapper* mGdiWrapper = nullptr;

// Functions.
/// <summary>
/// Message loop run on the GUI thread for processing Windows messages to the current Window. 
/// </summary>
LRESULT CALLBACK MessageLoop(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

/// <summary>
/// Main entry point for the application.
/// </summary>
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
  std::size_t bytesPerPixel = 4;
  mConfig.bytesPerPixel = bytesPerPixel;
  mConfig.viewportWidth = 640;
  mConfig.viewportHeight = 480;
  mConfig.viewportStride = mConfig.viewportWidth * bytesPerPixel;
  
  const char className[] = "Test Class Name";
  WNDCLASSEXA wc;
  wc.cbSize = sizeof(WNDCLASSEXA);
  wc.style = CS_HREDRAW | CS_VREDRAW;
  wc.lpfnWndProc = MessageLoop;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hInstance = hInstance;
  wc.hIcon = LoadIconW(NULL, IDI_APPLICATION);
  wc.hCursor = LoadCursorW(NULL, IDC_ARROW);
  wc.hbrBackground = 0;
  wc.lpszMenuName = 0;
  wc.lpszClassName = className;
  wc.hIconSm = 0;

  if (!RegisterClassExA(&wc)) {
    DWORD error = GetLastError();

    if (error != ERROR_CLASS_ALREADY_EXISTS) {
      return HRESULT_FROM_WIN32(error);
    }
  }

  DWORD windowStyle = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
  RECT clientRect;
  clientRect.top = 0;
  clientRect.left = 0;
  clientRect.bottom = static_cast<long>(mConfig.viewportHeight);
  clientRect.right = static_cast<long>(mConfig.viewportWidth);
  AdjustWindowRectEx(&clientRect, windowStyle, false, 0);

  HWND hwnd = CreateWindowExA(
    0, // Optional style.
    className, // Window class.
    "Test Window", // Window text.
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

  if (hwnd == NULL) {
    return HRESULT_FROM_WIN32(GetLastError());
  }

  // Prepare renderer and hook GDI+ to its framebuffer.
  mRenderer = new ZSharp::Renderer(&mConfig);
  mRenderer->RenderNextFrame();
  mGdiWrapper = new GDIWrapper(hwnd, mRenderer->GetFrameBuffer());

  // Run the message loop.
  MSG msg;
  ShowWindow(hwnd, nCmdShow);
  UpdateWindow(hwnd);

  // TODO: Something wrong with the way I'm handling the message loop and it's causing Windows to crash.
  // Investigate further.
  while (GetMessageA(&msg, hwnd, 0, 0)) {
    // Translate message is required for retrieving decoded user input.
    TranslateMessage(&msg);
    // Dispatch message is required to process the message from the OS.
    DispatchMessageA(&msg);
  }

  UnregisterClassA(className, hInstance);
  delete mGdiWrapper;
  delete mRenderer;

  return 0;
}

LRESULT CALLBACK MessageLoop(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  switch (uMsg) {
    // Update the frame whenever the it changes size or it needs to redraw.
    case WM_ERASEBKGND:
      // MSDN says we need to return non-zero when changing the background.
      return 1;
    case WM_PAINT:
      if (mGdiWrapper != nullptr && mRenderer != nullptr) {
        mGdiWrapper->UpdateWindow();
      }
      break;
    case WM_CLOSE:
      DestroyWindow(hwnd);
      break;
    case WM_DESTROY:
      PostQuitMessage(0);
      break;
    case WM_QUIT:
      return wParam;
    default:
      return DefWindowProcA(hwnd, uMsg, wParam, lParam);
  }

  return 0;
}
