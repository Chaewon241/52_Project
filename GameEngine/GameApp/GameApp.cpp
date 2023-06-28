#include "pch.h"
#include "GameApp.h"
#include "D2DRenderer.h"
#include "TimeManager.h"

GameApp* GameApp::m_hInstance = nullptr;
HWND GameApp::m_hwnd;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

GameApp::GameApp(HINSTANCE hInstance)
    :m_hInst(hInstance), m_szWindowClass{ L"DefaultWindowClass" }, m_szTitle{ L"DefaultWindowTitle" }
{
    m_wcex.cbSize = sizeof(WNDCLASSEX);
    m_wcex.style = CS_HREDRAW | CS_VREDRAW;
    m_wcex.lpfnWndProc = WndProc;
    m_wcex.cbClsExtra = 0;
    m_wcex.cbWndExtra = 0;
    m_wcex.hInstance = m_hInst;
    m_wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    m_wcex.lpszMenuName = NULL;
    m_wcex.lpszClassName = m_szWindowClass;
}

GameApp::~GameApp()
{
    DestroyWindow(m_hwnd);
    UnregisterClass(L"RotateObject", m_hInst);
}

bool GameApp::Initialize()
{
    // m_wcex 가 준비되었다고 가정
    RegisterClassExW(&m_wcex);


    m_hwnd = CreateWindowW(m_szWindowClass, m_szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, m_hInst, nullptr);

    if (!m_hwnd)
    {
        return FALSE;
    }

    ShowWindow(m_hwnd, SW_SHOW);
    UpdateWindow(m_hwnd);

    HRESULT hr = m_D2DRenderer.Initialize();

    TimeManager::m_Instance = new TimeManager;
    TimeManager::m_Instance->Initialize();

    return true;
}

void GameApp::Loop()
{
    MSG msg;

    while (true)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (WM_QUIT == msg.message)
                break;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            Update();
            Render();
        }
    }
}

void GameApp::Update()
{
    TimeManager::m_Instance->Update();
}

void GameApp::Render()
{

}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}