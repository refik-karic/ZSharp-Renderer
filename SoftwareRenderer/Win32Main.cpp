#ifdef PLATFORM_WINDOWS

#include <Win32PlatformApplication.h>

int WINAPI CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {
  GlobalApplication = new Win32PlatformApplication();
  int ret = GlobalApplication->Run(hInstance);
  delete GlobalApplication;
  return ret;
}

#endif
