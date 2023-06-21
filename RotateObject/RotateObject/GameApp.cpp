#include "pch.h"
#include "GameApp.h"
#include <cassert>

HINSTANCE GameApp::global_instance = NULL;
HWND GameApp::global_handle = NULL;

// 메세지를 받아서 처리해주는게 WndProc
// 콜백함수: 뒤에서 호출됨
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) 
{
    switch (message) 
    {
    case WM_CLOSE:
    case WM_LBUTTONDOWN:
        break;
    case WM_LBUTTONUP:
        break;
    case WM_MOUSEMOVE:
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, message, wParam, lParam);
    }
    return 0;
}

GameApp::GameApp(HINSTANCE hInstance, const UINT& width, const UINT& height)
{
    WNDCLASSEX wnd_class;
    wnd_class.cbClsExtra = 0;
    wnd_class.cbWndExtra = 0;
    wnd_class.hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
    wnd_class.hIcon = LoadIcon(nullptr, IDI_ERROR);
    wnd_class.hIconSm = LoadIcon(nullptr, IDI_ERROR);
    wnd_class.hInstance = hInstance;
    wnd_class.lpfnWndProc = WndProc;
    wnd_class.lpszClassName = L"돌려볼거시애오.";
    wnd_class.lpszMenuName = nullptr;
    wnd_class.style = CS_HREDRAW | CS_VREDRAW;
    wnd_class.cbSize = sizeof(WNDCLASSEX);

    auto check = RegisterClassEx(&wnd_class);
    assert(check != 0);

    global_handle = CreateWindowExW
    (
        WS_EX_APPWINDOW,
        L"RotateObject",
        L"RotateObject",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        static_cast<int>(width),
        static_cast<int>(height),
        nullptr,
        nullptr,
        hInstance,
        nullptr
    );

    assert(global_handle != nullptr);
}

GameApp::~GameApp()
{
    DestroyWindow(global_handle);
    UnregisterClass(L"RotateObject", global_instance);
}

void GameApp::Show()
{
    SetForegroundWindow(global_handle);
    SetFocus(global_handle);
    ShowCursor(TRUE);
    ShowWindow(global_handle, SW_NORMAL);
    UpdateWindow(global_handle);
}

bool GameApp::Update()
{
    MSG msg;
    ZeroMemory(&msg, sizeof(msg));

    if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.message != WM_QUIT;
}

const UINT GameApp::GetWidth()
{
    RECT rect;
    GetClientRect(global_handle, &rect);

    return static_cast<UINT>(rect.right - rect.left);
}

const UINT GameApp::GetHeight()
{
    RECT rect;
    GetClientRect(global_handle, &rect);

    return static_cast<UINT>(rect.bottom - rect.top);
}