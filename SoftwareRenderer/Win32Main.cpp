#if defined(_WIN64)

#include <Win32PlatformApplication.h>

int WINAPI CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {
  Win32PlatformApplication& app = Win32PlatformApplication::GetInstance();
  return app.Run(hInstance);
}

#endif
