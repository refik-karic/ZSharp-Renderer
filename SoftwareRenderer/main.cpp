#include "ZSharpApplication.h"

int WINAPI CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {
  ZSharpApplication& app = ZSharpApplication::GetInstance();
  app.Run(hInstance);
  return 0;
}
