// MyWinAPIProject.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "MyWinAPIProject.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

// 클릭한거 안 없어지게 전역변수로 둠.(원래는 DrawMarker 함수에 있었음)
static POINT ptMouseDown[32];
static int index;

// SAL 주석: _In_(데이터가 입력된다는 주석), _In_opt_(옵션이 존재함)
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,         // 실행된 프로세스의 시작주소
                     _In_opt_ HINSTANCE hPrevInstance,  // 얘는 옛날의 잔재임
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    // UNREFERENCED_PARAMETER는 주석이라고 생각하면 됨.
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MYWINAPIPROJECT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MYWINAPIPROJECT));

    MSG msg;

    // 기본 메시지 루프입니다:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}

//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MYWINAPIPROJECT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+2);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MYWINAPIPROJECT);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

void Marker(LONG x, LONG y, HWND hwnd)
{
    HDC hdc;

    hdc = GetDC(hwnd);
    MoveToEx(hdc, (int)x - 10, (int)y, (LPPOINT)NULL);
    LineTo(hdc, (int)x + 10, (int)y);
    MoveToEx(hdc, (int)x, (int)y - 10, (LPPOINT)NULL);
    LineTo(hdc, (int)x, (int)y + 10);

    ReleaseDC(hwnd, hdc);
}

void DrawMarker(HWND hWnd, LPARAM lParam)
{
    RECT rc;
    HRGN hrgn;
    POINTS ptTmp;

    if (index >= 32) return;

    GetClientRect(hWnd, &rc);
    hrgn = CreateRectRgn(rc.left, rc.top,
        rc.right, rc.bottom);

    ptTmp = MAKEPOINTS(lParam);
    ptMouseDown[index].x = (LONG)ptTmp.x;
    ptMouseDown[index].y = (LONG)ptTmp.y;

    // Test for a hit in the client rectangle.  

    if (PtInRegion(hrgn, ptMouseDown[index].x, ptMouseDown[index].y))
    {
        // If a hit occurs, record the mouse coords.  

        Marker(ptMouseDown[index].x, ptMouseDown[index].y, hWnd);
        index++;
    }

}

// hWnd는 핸들, HDC는 디바이스
void DrawParabola(HWND hWnd, HDC hdc)
{
    // 포물선 만들 공간
    RECT rect;
    // 그림을 그려주는거
    PAINTSTRUCT ps;
    // 그릴 펜
    HPEN hNewPen, hPrevPen;

    double a = -0.01;
    double b = 0;
    double c = 0;

    // 새로운 펜
    hNewPen = CreatePen(PS_DOT, 2, RGB(255, 255, 0)); //hNewPen 펜 생성 -> 옵션(도트), 굵기2, 색상 R:255 G:0 B:0
    // 이전 펜
    hPrevPen = (HPEN)SelectObject(hdc, hNewPen); //hdc 영역에 새로운 펜을 적용후 이전에 있던 펜의 정보를 hPrevPen에 저장

    // 포물선을 그릴 공간 받아오기
    GetClientRect(hWnd, &rect);

    // Draw the axes -> 데카르트 좌표계
    // x축 y축
    // 좌표이동하고
    MoveToEx(hdc, rect.left, rect.bottom / 2, NULL);
    // 선그리기
    LineTo(hdc, rect.right, rect.bottom / 2);
    // 좌표 이동하고
    MoveToEx(hdc, rect.right / 2, rect.top, NULL);
    // 선그리기
    LineTo(hdc, rect.right / 2, rect.bottom);

    // Draw the parabola -> 2차 함수
    // 포물선 그리기
    MoveToEx(hdc, rect.left, rect.bottom / 2, NULL);
    // x축의 가장 왼쪽에서부터 가장 오른쪽까지
    for (int x = rect.left; x <= rect.right; x++) {
        double y = a * pow(x - rect.right / 2, 2) + b * (x - rect.right / 2) + c + rect.bottom / 2;
        LineTo(hdc, x, y);
    }

    //펜 정보 원상태로 바꾸기
    SelectObject(hdc, hPrevPen); //현재영역에 대한 펜을 원래 있던 펜으로 다시 적용
    DeleteObject(hNewPen); //사용이 끝난 펜 해제
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    // 뭐 그리고 싶을 때 여기서 그림
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...

            RECT rt4Text = { 100, 100, 500, 300 };

            // 텍스트를 가운데 정렬하고 한 줄이 꽉찰 경우 개행한다
            DrawText(hdc, L"Hello World", -1, &rt4Text, DT_CENTER | DT_WORDBREAK);
            DrawParabola(hWnd, hdc);
            EndPaint(hWnd, &ps);

            for (int i = 0; i < index; i++)
            {
                Marker(ptMouseDown[i].x, ptMouseDown[i].y, hWnd);
            }

        }
        break;
    case WM_LBUTTONDOWN:
        // Create the region from the client area.  
        DrawMarker(hWnd, lParam);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
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
