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
static const int32_t WIDTH = 640;
static const int32_t HEIGHT = 480;
static GDIWrapper* GDI_WRAPPER = nullptr;
static HINSTANCE mInstance;
static const wchar_t CLASS_NAME[] = L"Sample Window Class";
static const wchar_t WINDOW_TEXT[] = L"Learn To Program Windows";

static int32_t* mpBackBuffer;

// Functions.
LRESULT static CALLBACK MessageLoop(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void CopyPixels();
void DrawFrame();

// Main entry point for the application
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
  mInstance = hInstance;
  
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
    CW_USEDEFAULT, CW_USEDEFAULT, WIDTH, HEIGHT,
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
  GDI_WRAPPER = new GDIWrapper(hwnd, WIDTH, HEIGHT);

  // Launch the renderer.
  Renderer swRenderer(CopyPixels);
  swRenderer.Start();

  // Run the message loop.
  bool bGotMsg;
  MSG  msg;
  msg.message = WM_NULL;
  PeekMessage(&msg, NULL, 0U, 0U, PM_NOREMOVE);

  while (WM_QUIT != msg.message) {
    // Process window events.
    // Use PeekMessage() so we can use idle time to render the scene. 
    bGotMsg = (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE) != 0);

    if (bGotMsg) {
      // Translate and dispatch the message
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }

    // TODO: Add a sleep here to stop spinning on the CPU.
  }

  delete GDI_WRAPPER;

  return 0;
}

LRESULT CALLBACK MessageLoop(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  switch (uMsg) {
    case WM_CLOSE:
      GDI_WRAPPER->StopPaint();
      DestroyWindow(hwnd);
      UnregisterClass(CLASS_NAME, mInstance);
      return 0;

    case WM_DESTROY:
      GDI_WRAPPER->StopPaint();
      PostQuitMessage(0);
      break;
  }

  return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void CopyPixels() {
  int32_t tmp(55);
}

void DrawFrame() {
  int32_t tmp(12);
}
