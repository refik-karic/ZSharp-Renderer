// Headers.
#include <Renderer.h>
#include <ZConfig.h>

#include <chrono>

#include "WindowsHeadersWrapper.h"
#include "GDIWrapper.h"

static ZSharp::Renderer* mRenderer = nullptr;
static GDIWrapper* mGdiWrapper = nullptr;

//static std::chrono::high_resolution_clock::time_point LAST_FRAME;

bool InitializeRenderer();
HWND SetupWindow(HINSTANCE hInstance, const wchar_t* className);
LRESULT CALLBACK MessageLoop(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int nCmdShow) {
  if (!InitializeRenderer()) {
    return -1;
  }

  static constexpr wchar_t className[] = L"Test Class Name";
  HWND hwnd = SetupWindow(hInstance, className);
  if (hwnd == nullptr) {
    return HRESULT_FROM_WIN32(GetLastError());
  }

  mGdiWrapper = new GDIWrapper();

  //LAST_FRAME = std::chrono::high_resolution_clock::now();

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

  UnregisterClassW(className, hInstance);
  delete mGdiWrapper;
  delete mRenderer;

  return 0;
}

LRESULT CALLBACK MessageLoop(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  static constexpr long long FRAMERATE_60_HZ_MS = 1000 / 60;
  static UINT_PTR windowsFrameTimer;

  switch (uMsg) {
    case WM_CREATE:
      windowsFrameTimer = SetTimer(hwnd, 1, static_cast<UINT>(FRAMERATE_60_HZ_MS), NULL);

      if(windowsFrameTimer == 0){
        DestroyWindow(hwnd);
      }

      break;
    case WM_TIMER:
      // TODO: Figure out if the stuff for calculating the frame delta would be useful later.
      /*std::chrono::milliseconds frameDelta;
      frameDelta = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - LAST_FRAME);

      if(frameDelta.count() > FRAMERATE_60_HZ_MS){
        RECT activeWindowSize;
        GetClientRect(hwnd, &activeWindowSize);
        // TODO: This uses lazy "dirty rectangle" drawing and re-draws the entire frame on each pass.
        // It would be much better to track the changed area and only update that.
        InvalidateRect(hwnd, &activeWindowSize, false);
      }

      LAST_FRAME = std::chrono::high_resolution_clock::now();*/
      
      RECT activeWindowSize;
      GetClientRect(hwnd, &activeWindowSize);
      InvalidateRect(hwnd, &activeWindowSize, false);

      break;
    case WM_PAINT:
      mRenderer->RenderNextFrame();
      mGdiWrapper->UpdateWindow(hwnd, mRenderer->GetFrameBuffer());
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

bool InitializeRenderer() {
  ZSharp::ZConfig& config = ZSharp::ZConfig::GetInstance();
  config.SetViewportWidth(640);
  config.SetViewportHeight(480);
  config.SetBytesPerPixel(4);

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
