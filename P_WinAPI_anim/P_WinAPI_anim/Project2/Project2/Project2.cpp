// Project2.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "Project2.h"
#define MAX_LOADSTRING 100

#pragma comment(lib, "msimg32.lib")

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.


SYSTEMTIME st;
static TCHAR sTime[128] = _T("");
static RECT rt = { 100,100,400,120 };

static int x = 300, y = 0;
static int frame = 0;

void Init() 
{
}
void Release() 
{
}

void Draw(HDC hdc) 
{    
    BITMAP bit; int bx, by;

    HBITMAP MyBitmap1 = 0;
    HBITMAP MyBitmap2 = 0;
    HBITMAP MyBitmap3 = 0;
    HBITMAP MyBitmap4 = 0;
    MyBitmap1 = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1));    //로딩
    MyBitmap2 = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP2));    //로딩
    MyBitmap3 = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP3));    //로딩
    MyBitmap4 = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP4));    //로딩

    // 비트맵 정보를 알아낸다
    //GetObject(MyBitmap1, sizeof(BITMAP), &bit);
    //bx = bit.bmWidth;
    //by = bit.bmHeight;

    HDC MemDC = 0;
    HDC MemDC1 = 0;
    HDC MemDC2 = 0;

    HBITMAP OldBitmap = 0;

    // 더블 버퍼링 -----------------------------------------------------------

    MemDC = CreateCompatibleDC(hdc); 			                            //메모리DC 생성  //배경
    HBITMAP hBitmap = CreateCompatibleBitmap(hdc, 800, 600);
    SelectObject(MemDC, hBitmap);                                           //비트맵 선택    

        FillRect(MemDC, &rt, (HBRUSH)GetStockObject(GRAY_BRUSH));               //배경색  //GetClientRect(hWnd, &rt);

        MemDC1 = CreateCompatibleDC(hdc);
        OldBitmap = (HBITMAP)SelectObject(MemDC1, MyBitmap1); 	                //비트맵 선택    //객체2
        BitBlt(MemDC, 0, 0, 256, 256, MemDC1, 0, 0, SRCCOPY); 		            //배경에 복사 및 출력

        MemDC2 = CreateCompatibleDC(hdc);
        SelectObject(MemDC2, MyBitmap2); 	                                    //비트맵 선택    //객체2
        StretchBlt(MemDC, 200, 0, 128, 128, MemDC2, 0, 0, 256, 256, SRCCOPY);   //배경에 복사 및 출력
        
    BitBlt(hdc, 0, 0, 800, 600, MemDC, 0, 0, SRCCOPY);                      //배경을 화면으로 

    // 에니메이션 -----------------------------------------------------------
    if (frame < 2)
    {
        SelectObject(MemDC, MyBitmap3); 	    //비트맵 선택
        TransparentBlt(hdc, x, y, 256, 256, MemDC, 0, 0, 256, 256, RGB(255, 255, 255));
    }
    else
    {
        SelectObject(MemDC, MyBitmap4); 	    //비트맵 선택
        TransparentBlt(hdc, x, y, 256, 256, MemDC, 0, 0, 256, 256, RGB(255, 255, 255));
    }

    //SelectObject(MemDC, MyBitmap4); 	    //비트맵 선택
    //static BLENDFUNCTION bf;
    //bf.BlendOp = bf.BlendFlags = bf.AlphaFormat = 0;
    //bf.SourceConstantAlpha = 128;
    //AlphaBlend(hdc, 300, 0, 256, 256, MemDC, 0, 0, 256, 256, bf);

    SelectObject(MemDC, OldBitmap);

    DeleteDC(MemDC);
    DeleteDC(MemDC1);
    DeleteDC(MemDC2);

    DeleteObject(MyBitmap1);
    DeleteObject(MyBitmap2);
    DeleteObject(MyBitmap3);
    DeleteObject(MyBitmap4);
}


// TimerProc 
void CALLBACK TimerProc(HWND hWnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
    //GetKeyState --------------------------------------------------------
    //if (GetKeyState(VK_UP) < 0) y--;
    //if (GetKeyState(VK_DOWN) < 0) y++;
    //if (GetKeyState(VK_LEFT) < 0) x--;
    //if (GetKeyState(VK_RIGHT) < 0) x++;

    //GetAsyncKeyState ---------------------------------------------------
    //0x0000        이전에 누른 적이 없고 호출 시점에도 눌려있지 않은 상태
    //0x0001        이전에 누른 적이 있고 호출 시점에는 눌려있지 않은 상태
    //0x8000        이전에 누른 적이 없고 호출 시점에는 눌려있는 상태    
    //0x8001        이전에 누른 적이 있고 호출 시점에도 눌려있는 상태
    //
    //  한번 눌릴키가 쭉 눌려있는 상태인지 알고 싶다면 0x8000 을 사용.
    //  키가 변했는지 알고 싶다면, & 연산을 사용 0x0001에 대해 검사.

    if (GetAsyncKeyState(VK_LEFT) & 0x8000)     { x-= 3; }
    if (GetAsyncKeyState(VK_RIGHT) & 0x8000)    { x+= 3; }
    if (GetAsyncKeyState(VK_UP) & 0x8000)       { y-= 3; }
    if (GetAsyncKeyState(VK_DOWN) & 0x8000)     { y+= 3; }

    InvalidateRect(hWnd, NULL, TRUE);

    frame++; frame %= 4;
}


// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_PROJECT2, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    Init();

    // 기본 메시지 루프입니다:
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PROJECT2));
    MSG msg;    
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    Release();

    return (int) msg.wParam;
}

//  함수: MyRegisterClass()
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PROJECT2));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_PROJECT2);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
    return RegisterClassExW(&wcex);
}

//   함수: InitInstance(HINSTANCE, int)
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.
   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);
   if (!hWnd) { return FALSE; }
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);
   return TRUE;
}

//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        SetTimer(hWnd, 1, 1000, NULL);
        SetTimer(hWnd, 2, 50, (TIMERPROC)TimerProc);
        SendMessage(hWnd, WM_TIMER, 1, 0);   	// 타이머 아이디 1

        GetClientRect(hWnd, &rt);
        break;
    case WM_TIMER:
        {
            switch (wParam) {
            case 1:
                {
                    //GetLocalTime(&st);
                    //_stprintf_s(sTime, _T("지금 시간은 %d:%d:%d입니다"), st.wHour, st.wMinute, st.wSecond);                    
                    //InvalidateRect(hWnd, &rt, TRUE);	//InvalidateRect
                }
                break;
            case 2:
                //MessageBeep(0);
                break;
            }
        }
        break;
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
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...

            Draw(hdc);

            TextOut(hdc, 10, 10, sTime, _tcslen(sTime));

            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        KillTimer(hWnd, 1);
        KillTimer(hWnd, 2);
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
