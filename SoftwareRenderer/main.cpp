// See this for information about Visual Studio C++ XML comments: https://docs.microsoft.com/en-us/cpp/ide/xml-documentation-visual-cpp

#ifndef UNICODE
#define UNICODE
#endif

// Exclude rarely-used stuff from Windows headers.
#define WIN32_LEAN_AND_MEAN

// Headers.
#include <windows.h>
#include <Renderer.h>
#include <Framebuffer.h>
#include "GDIWrapper.h"

// Global variables.
/// <summary>
/// Global config passed between objects as needed to acess common settings.
/// </summary>
static ZSharp::Config mConfig;

static ZSharp::Renderer* mRenderer = nullptr;

static GDIWrapper* mGdiWrapper = nullptr;

/// <summary>
/// Global handle to the current Window instance. 
/// </summary>
static HINSTANCE mInstance;

static const wchar_t CLASS_NAME[] = L"Sample Window Class";

static const wchar_t WINDOW_TEXT[] = L"Learn To Program Windows";

// Functions.
/// <summary>
/// Message loop run on the GUI thread for processing Windows messages to the current Window. 
/// </summary>
LRESULT static CALLBACK MessageLoop(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

/// <summary>
/// Main entry point for the application.
/// </summary>
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
  mInstance = hInstance;

  std::size_t bytesPerPixel = 4;
  mConfig.bytesPerPixel = bytesPerPixel;
  mConfig.viewportWidth = 640;
  mConfig.viewportHeight = 480;
  mConfig.viewportStride = mConfig.viewportWidth * bytesPerPixel;
  
  WNDCLASS wc = {};
  wc.lpfnWndProc = MessageLoop;
  wc.hInstance = mInstance;
  wc.lpszClassName = CLASS_NAME;

  if (!RegisterClass(&wc)) {
    DWORD error = GetLastError();

    if (error != ERROR_CLASS_ALREADY_EXISTS) {
      return HRESULT_FROM_WIN32(error);
    }
  }

  HWND hwnd = CreateWindowEx(
    0, // Optional style.
    CLASS_NAME, // Window class.
    WINDOW_TEXT, // Window text.
    WS_OVERLAPPEDWINDOW, // Window style.
    // Size and position.
    CW_USEDEFAULT, CW_USEDEFAULT, static_cast<int>(mConfig.viewportWidth), static_cast<int>(mConfig.viewportHeight),
    NULL, // Parent window.
    NULL, // Menu.
    mInstance, // Instance handle.
    NULL // Additional application data.
  );

  if (hwnd == NULL) {
    return HRESULT_FROM_WIN32(GetLastError());
  }

  ShowWindow(hwnd, nCmdShow);

  // Setup bitmap display on the window.
  mGdiWrapper = new GDIWrapper(hwnd);

  // Launch the renderer.
  mRenderer = new ZSharp::Renderer(&mConfig);
  mRenderer->Start();

  // Run the message loop.
  MSG msg;
  msg.message = WM_NULL;

  while (GetMessage(&msg, hwnd, 0, 0)) {
    // Translate message is required for retrieving decoded user input.
    TranslateMessage(&msg);
    // Dispatch message is required to process the message from the OS.
    DispatchMessage(&msg);
  }

  UnregisterClass(CLASS_NAME, mInstance);
  delete mGdiWrapper;

  return 0;
}

LRESULT CALLBACK MessageLoop(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  switch (uMsg) {
    case WM_PAINT:
      if (mGdiWrapper != nullptr && mRenderer != nullptr) {
        ZSharp::Framebuffer* buf = mRenderer->GetNextFrame();

        if (buf->GetBuffer() != nullptr) {
          mGdiWrapper->DrawBitmap(buf);
        }
      }
      return 0;
    case WM_CLOSE:
      DestroyWindow(hwnd);
      UnregisterClass(CLASS_NAME, mInstance);
      return 0;
    case WM_DESTROY:
      PostQuitMessage(0);
      break;
  }

  return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
