#include <Windows.h>

LRESULT CALLBACK Win32AppWndProc(HWND wnd, UINT msg, WPARAM wPar, LPARAM lPar)
{
  switch (msg)
  {
  case WM_CLOSE:
    DestroyWindow(wnd);
    return 0;
  case WM_DESTROY:
    PostQuitMessage(0);
    return 0;
  case WM_KEYDOWN:
    switch(wPar)
    {
    case VK_ESCAPE:
      DestroyWindow(wnd);
      break;
    }
    return 0;
  }

  return DefWindowProc(wnd, msg, wPar, lPar);
}

int WINAPI wWinMain(HINSTANCE instance, HINSTANCE, LPWSTR cmdLine, int showCommand)
{
  WNDCLASSEX wndCls = { 0 };
  wndCls.cbSize = sizeof(wndCls);
  wndCls.hbrBackground  = reinterpret_cast<HBRUSH>(COLOR_WINDOW+1);
  wndCls.hCursor        = reinterpret_cast<HCURSOR>(LoadImage(nullptr, IDC_ARROW, IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED));
  wndCls.hIcon          = reinterpret_cast<HICON>(LoadImage(nullptr, IDI_APPLICATION, IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_SHARED));
  wndCls.hIconSm        = reinterpret_cast<HICON>(LoadImage(nullptr, IDI_APPLICATION, IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_SHARED));
  wndCls.hInstance      = instance;
  wndCls.lpszClassName  = L"Win32App_WndClass";
  wndCls.lpfnWndProc    = Win32AppWndProc;
  RegisterClassEx(&wndCls);

  RECT desktopRect = { 0 };
  GetWindowRect(GetDesktopWindow(), &desktopRect);

  const DWORD exStyle = WS_EX_APPWINDOW;
  const DWORD style = WS_OVERLAPPEDWINDOW;
  RECT windowRect = {0, 0, 1280, 720};
  AdjustWindowRectEx(&windowRect, style, FALSE, exStyle);

  const int windowWidth   = windowRect.right - windowRect.left;
  const int windowsHeight = windowRect.bottom - windowRect.top;
  const int windowXpos = (desktopRect.right / 2) - (windowWidth / 2);
  const int windowYpos = (desktopRect.bottom / 2) - (windowsHeight / 2);

  HWND mainWindow = CreateWindowEx(exStyle, wndCls.lpszClassName, L"Win32App", style,
                                   windowXpos, windowYpos, windowWidth, windowsHeight,
                                   nullptr, nullptr, wndCls.hInstance, nullptr);
  ShowWindowAsync(mainWindow, showCommand);

  MSG msg = { 0 };
  while (GetMessage(&msg, nullptr, 0, 0) > 0)
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  UnregisterClass(wndCls.lpszClassName, wndCls.hInstance);
  return 0;
}