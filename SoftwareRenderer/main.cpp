#ifndef UNICODE
#define UNICODE
#endif

// Exclude rarely-used stuff from Windows headers.
#define WIN32_LEAN_AND_MEAN

// Headers.
#include <windows.h>

#include <Renderer.h>

// Global variables.
static const wchar_t CLASS_NAME[] = L"Sample Window Class";
static const wchar_t WINDOW_TEXT[] = L"Learn To Program Windows";

// Functions.
LRESULT static CALLBACK MessageLoop(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void DrawBitmap();

// Main entry point for the application
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
  WNDCLASS wc = {};
  wc.lpfnWndProc = MessageLoop;
  wc.hInstance = hInstance;
  wc.lpszClassName = CLASS_NAME;
  RegisterClass(&wc);

  HWND hwnd = CreateWindowEx(
    0, // Optional style.
    CLASS_NAME, // Window class.
    WINDOW_TEXT, // Window text.
    WS_OVERLAPPEDWINDOW, // Window style.
    // Size and position.
    CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
    NULL, // Parent window.
    NULL, // Menu.
    hInstance, // Instance handle.
    NULL // Additional application data.
  );

  ShowWindow(hwnd, nCmdShow);

  // Launch the renderer.
  Renderer swRenderer(DrawBitmap);
  swRenderer.Start();

  // Run the message loop.
  MSG msg = {};
  while (GetMessage(&msg, NULL, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  return 0;
}

LRESULT CALLBACK MessageLoop(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  switch (uMsg) {
    case WM_DESTROY:
      PostQuitMessage(0);
      return 0;

    case WM_PAINT:
      PAINTSTRUCT ps;
      HDC hdc = BeginPaint(hwnd, &ps);
      FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
      EndPaint(hwnd, &ps);

      return 0;
  }

  return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void DrawBitmap() {
  int32_t tmp = 55;
}
