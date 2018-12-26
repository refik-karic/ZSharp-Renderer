// See this for information about Visual Studio C++ XML comments: https://docs.microsoft.com/en-us/cpp/ide/xml-documentation-visual-cpp

// Headers.
#include "WindowsHeadersWrapper.h"
#include <Renderer.h>
#include "GDIWrapper.h"

// Global variables.
/// <summary>
/// Global config passed between objects as needed to acess common settings.
/// </summary>
ZSharp::Config mConfig;
std::size_t mBytesPerPixel = 4;

ZSharp::Renderer* mRenderer = nullptr;

GDIWrapper* mGdiWrapper = nullptr;

const char mClassName[] = "Test Class Name";

HWND SetupWindow(HINSTANCE hInstance, const char* className);

/// <summary>
/// Message loop run on the GUI thread for processing Windows messages to the current Window. 
/// </summary>
LRESULT CALLBACK MessageLoop(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

/// <summary>
/// Main entry point for the application.
/// </summary>
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
  mConfig.bytesPerPixel = mBytesPerPixel;
  mConfig.viewportWidth = 640;
  mConfig.viewportHeight = 480;
  mConfig.viewportStride = mConfig.viewportWidth * mBytesPerPixel;
  
  HWND hwnd = SetupWindow(hInstance, mClassName);
  if (hwnd == nullptr) {
    return HRESULT_FROM_WIN32(GetLastError());
  }

  // Prepare renderer and hook GDI+ to its framebuffer.
  mRenderer = new ZSharp::Renderer(&mConfig);
  mGdiWrapper = new GDIWrapper(hwnd);

  // Run the message loop.
  MSG msg;
  ShowWindow(hwnd, nCmdShow);
  UpdateWindow(hwnd);

  while (GetMessageA(&msg, hwnd, 0, 0) > 0) {
    // Translate message is required for retrieving decoded user input.
    TranslateMessage(&msg);
    // Dispatch message is required to process the message from the OS.
    DispatchMessageA(&msg);
  }

  UnregisterClassA(mClassName, hInstance);
  delete mGdiWrapper;
  delete mRenderer;

  return 0;
}

LRESULT CALLBACK MessageLoop(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  switch (uMsg) {
    case WM_PAINT:
      if (mGdiWrapper != nullptr && mRenderer != nullptr) {
        mRenderer->RenderNextFrame();
        mGdiWrapper->UpdateWindow(*(mRenderer->GetFrameBuffer()));
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

HWND SetupWindow(HINSTANCE hInstance, const char* className) {
  WNDCLASSEXA wc;
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

  if (!RegisterClassExA(&wc)) {
    return nullptr;
  }
  
  DWORD windowStyle = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
  RECT clientRect;
  clientRect.top = 0;
  clientRect.left = 0;
  clientRect.bottom = static_cast<long>(mConfig.viewportHeight);
  clientRect.right = static_cast<long>(mConfig.viewportWidth);
  AdjustWindowRectEx(&clientRect, windowStyle, false, 0);

  return CreateWindowExA(
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
}
