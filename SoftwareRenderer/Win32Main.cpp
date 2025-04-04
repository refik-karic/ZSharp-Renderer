#ifdef PLATFORM_WINDOWS

#include <Win32PlatformApplication.h>

int WINAPI CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {
  ZSharp::InitializeEnvironment();
  Win32PlatformApplication& app = Win32PlatformApplication::Get();
  return app.Run(hInstance);
}

#endif
