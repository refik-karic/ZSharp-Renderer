#include "WindowsHeadersWrapper.h"
#include "ZSharpApplication.h"

#include <ZConfig.h>

static wchar_t WINDOW_CLASS_NAME[] = L"SoftwareRendererWindowClass";
static wchar_t WINDOW_TITLE[] = L"Software Renderer";

HWND SetupWindow(HINSTANCE hInstance, const wchar_t* className) {
  WNDCLASSEXW wc{
    sizeof(WNDCLASSEXW),
    CS_HREDRAW | CS_VREDRAW,
    &ZSharpApplication::MessageLoop,
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
  RECT clientRect{ 0L, 0L, static_cast<long>(config.GetViewportWidth()), static_cast<long>(config.GetViewportHeight()) };
  AdjustWindowRectEx(&clientRect, windowStyle, false, WS_EX_OVERLAPPEDWINDOW);

  return CreateWindowExW(
    WS_EX_OVERLAPPEDWINDOW,
    className,
    WINDOW_TITLE,
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

int WINAPI CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int nCmdShow) {
  HWND hwnd = SetupWindow(hInstance, WINDOW_CLASS_NAME);
  if (hwnd == nullptr) {
    return HRESULT_FROM_WIN32(GetLastError());
  }

  ShowWindow(hwnd, nCmdShow);
  for (MSG msg; GetMessageW(&msg, hwnd, 0, 0) > 0;) {
    TranslateMessage(&msg);
    DispatchMessageW(&msg);
  }

  UnregisterClassW(WINDOW_CLASS_NAME, hInstance);
  return 0;
}
