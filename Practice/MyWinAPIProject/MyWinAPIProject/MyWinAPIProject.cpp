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
// 윈도우 프로시저가 메시지 처리를 끝냈다고 운영체제에게 알려주는 값
//0을 반환한다는 것은 운영체제는 이 메세지에 관여하지 않고 프로그래머가 직접 처리한다는 의미를 갖습니다. -1을 반환하게 되면 운영체제가 진행하는 작업을 취소한다는 의미를 갖습니다.
// CALLBACK 함수는 사용자가 호출하는 함수가 아닌, 특정 트리거(이벤트)에 의해 운영체제가 자동으로 실행하는 함수
// 핸들이란 운영체제 내부에 있는 어떤 리소스의 주소를 정수(32bit 혹은 64bit)로 치환한 값, 즉 HANDLE이란 자료형은 오브젝트의 주소를 나타내는 자료형
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
// 윈도우 클래스를 커널에 등록하는 과정을 거칠 차례
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
    // lpszClass : 윈도우를 생성하는 클래스를 관리하는 변수
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
      CW_USEDEFAULT, 0, 1600, 900, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

void PaintGrid(LONG x, LONG y, HWND hwnd)
{
    RECT rc;
    GetClientRect(hwnd, &rc);
    HDC hdc;

    hdc = GetDC(hwnd);
    float pix_x = (float)x / rc.right;
    float pix_y = (float)y / rc.bottom;

    // x좌표 어딨는지
    float tmp1 = 0;
    for (int i = 0; i < 7; i++)
    {
        if (tmp1 <= pix_x && pix_x < tmp1 + 0.125)
            break;
        tmp1 += 0.125;
    }

    // y좌표 어딨는지
    float tmp2 = 0;
    for (int i = 0; i < 3; i++)
    {
        if (tmp2 <= pix_y && pix_y < tmp2 + 0.25)
            break;
        tmp2 += 0.25;
    }

    HBRUSH myBrush = (HBRUSH)CreateSolidBrush(RGB(255, 0, 0));
    HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, myBrush);
    Rectangle(hdc, tmp1 * 1600, tmp2 * 900, (tmp1 + 0.125) * 1600, (tmp2 + 0.25) * 900);
    
    SelectObject(hdc, oldBrush);
    DeleteObject(myBrush);

    ReleaseDC(hwnd, hdc);
}

void SelectGrid(HWND hWnd, LPARAM lParam)
{
    RECT rc;
    HRGN hrgn;
    POINTS ptTmp;

    GetClientRect(hWnd, &rc);
    hrgn = CreateRectRgn(rc.left, rc.top, rc.right, rc.bottom);

    ptTmp = MAKEPOINTS(lParam);
    ptMouseDown[index].x = (LONG)ptTmp.x;
    ptMouseDown[index].y = (LONG)ptTmp.y;

    if (PtInRegion(hrgn, ptMouseDown[index].x, ptMouseDown[index].y))
    {
        PaintGrid(ptMouseDown[index].x, ptMouseDown[index].y, hWnd);
    }

}

// hWnd는 핸들, HDC는 디바이스
void DrawGrid(HWND hWnd, HDC hdc)
{
    // 그리드 만들 공간
    RECT rect;
    // 그림을 그려주는거
    PAINTSTRUCT ps;
    // 그릴 펜
    HPEN hNewPen, hPrevPen;

    // 새로운 펜
    hNewPen = CreatePen(PS_DOT, 2, RGB(0, 0, 0)); //hNewPen 펜 생성 -> 옵션(도트), 굵기2, 색상 R:255 G:0 B:0
    // 이전 펜
    hPrevPen = (HPEN)SelectObject(hdc, hNewPen); //hdc 영역에 새로운 펜을 적용후 이전에 있던 펜의 정보를 hPrevPen에 저장

    // 그리드 그릴 공간 받아오기
    GetClientRect(hWnd, &rect);

    // 가로
    float div1 = 1;
    while (div1 > 0)
    {
        div1 -= 0.25;
        MoveToEx(hdc, rect.left, rect.bottom * div1, NULL);
        LineTo(hdc, rect.right, rect.bottom * div1);
    }
    
    // 세로
    float div2 = 1;
    while (div2 > 0)
    {
        div2 -= 0.125;
        MoveToEx(hdc, rect.right * div2, rect.top, NULL);
        LineTo(hdc, rect.right * div2, rect.bottom);
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

            DrawGrid(hWnd, hdc);
            EndPaint(hWnd, &ps);

            /*for (int i = 0; i < index; i++)
            {
                Marker(ptMouseDown[i].x, ptMouseDown[i].y, hWnd);
            }*/

        }
        break;
    case WM_LBUTTONDOWN:
        // Create the region from the client area.  
        SelectGrid(hWnd, lParam);
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
