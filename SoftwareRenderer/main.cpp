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
static ZSharp::Config mConfig;
static GDIWrapper* mGdiWrapper = nullptr;
static HINSTANCE mInstance;
static const wchar_t CLASS_NAME[] = L"Sample Window Class";
static const wchar_t WINDOW_TEXT[] = L"Learn To Program Windows";

// Functions.
LRESULT static CALLBACK MessageLoop(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void RendererDrawCallback(uint8_t* frameData);

// Main entry point for the application
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
  mInstance = hInstance;

  mConfig.bytesPerPixel = 4;
  mConfig.viewportWidth = 640;
  mConfig.viewportHeight = 480;
  mConfig.viewportStride = mConfig.viewportWidth * 4;
  
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
    CW_USEDEFAULT, CW_USEDEFAULT, mConfig.viewportWidth, mConfig.viewportHeight,
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
  mGdiWrapper = new GDIWrapper(hwnd, mConfig.viewportWidth, mConfig.viewportHeight);

  // Launch the renderer.
  ZSharp::Renderer swRenderer(RendererDrawCallback, &mConfig);
  swRenderer.Start();

  // Run the message loop.
  bool bGotMsg;
  MSG  msg;
  msg.message = WM_NULL;
  PeekMessage(&msg, NULL, 0U, 0U, PM_NOREMOVE);

  // For sleeping.
  using namespace std::chrono_literals;

  while (WM_QUIT != msg.message) {
    // Process window events.
    // Use PeekMessage() so we can use idle time to render the scene. 
    bGotMsg = (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE) != 0);

    if (bGotMsg) {
      // Translate and dispatch the message
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }

    // Sleep to avoid busy spinning.
    std::this_thread::sleep_for(10ms);
  }

  delete mGdiWrapper;

  return 0;
}

LRESULT CALLBACK MessageLoop(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  switch (uMsg) {
    case WM_PAINT:

      return 0;
    case WM_CLOSE:
      mGdiWrapper->StopPaint();
      DestroyWindow(hwnd);
      UnregisterClass(CLASS_NAME, mInstance);
      return 0;

    case WM_DESTROY:
      mGdiWrapper->StopPaint();
      PostQuitMessage(0);
      break;
  }

  return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void RendererDrawCallback(uint8_t* frameData) {
  mGdiWrapper->DrawBitmap(frameData);
}
