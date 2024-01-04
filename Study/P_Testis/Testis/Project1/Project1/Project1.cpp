﻿// Project1.cpp : 애플리케이션에 대한 진입점을 정의합니다.
#include "framework.h"
#include "Project1.h"

// 일정 범위안의 난수 생성을 위한 매크로문
#define random(n) (rand()%n)

// 함수 원형 선언
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void brick(int flag);
void leftmove();
void rightmove();
int downmove();
void rotate();
int whataround(int, int);
void isfull();
void printstage();
void makenewblock();
void drawscreen();
void printscore();
void drawnextbrick();

// 각 벽돌의 패턴 배열. 한 벽돌당 8바이트씩이면 회전 모양의 개수에 따라  
// 벽돌당 데이터의 개수가 다르다. 각 값은 중심점에서의 상대적인 거리이다   
int pat[7][32] = {
  {0,0,1,0,2,0,-1,0,0,0,0,1,0,-1,0,-2,},
  {0,0,1,0,0,1,1,1,},
  {0,0,-1,0,0,-1,1,-1,0,0,0,1,-1,0,-1,-1,},
  {0,0,-1,-1,0,-1,1,0,0,0,-1,0,-1,1,0,-1,},
  {0,0,0,-1,1,0,2,0,0,0,-1,0,0,-1,0,-2,0,0,-1,0,-2,0,0,1,0,0,1,0,0,1,0,2},
  {0,0,-1,0,-2,0,0,-1,0,0,-1,0,0,1,0,2,0,0,0,1,1,0,2,0,0,0,1,0,0,-1,0,-2},
  {0,0,-1,0,1,0,0,1,0,0,0,-1,0,1,1,0,0,0,-1,0,1,0,0,-1,0,0,-1,0,0,-1,0,1}
};
int brickpatnum[7] = { 2,1,2,2,4,4,4 };  // 각 벽돌의 패턴 개수 
int tet[14][23];         // 게임판 배열 
int i, j, k;               // 다목적 임시 변수, 주로 루프 제어에 사용 
int score;               // 점수  
int speed;               // 속도
wchar_t imsistr[128];       // 문자열 출력을 위한 임시 버퍼 
int nowbrick, nowrot;     // 현재 선택된 벽돌과 회전 모양 
int nextbrick;           // 다음 출력될 벽돌             
int nowx, nowy;           // 현재의 위치 화면 좌표값이다. 
BOOL GameStart;			 // 현재 게임 진행 중?

HDC hDC, hMemDC, hMemDC2;
HBITMAP hbrick, hbrick2;
HINSTANCE g_hInst;
HWND hWnd;

//######################################################
// 메인 루틴, 윈도우를 생성하며 메시지 루프를 실행시킨다. 
//######################################################
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance
	, LPSTR lpszCmdParam, int nCmdShow)
{
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(g_hInst, MAKEINTRESOURCE(IDI_SMALL));
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.lpszClassName = L"Shtet3";
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDC_PROJECT1);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd = CreateWindow(L"Shtet3", L"Sh tetris 3",
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		50, 50, 470, 460, NULL, NULL, g_hInst, NULL);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

//######################################################
// 메시지 처리 루틴 
//######################################################
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	static FARPROC lpfnAboutDlgProc;
	PAINTSTRUCT ps;
	switch (iMessage) {
		// 윈도우가 처음 만들어질 때 보내지는 메시지이다.
		// 이 메시지 처리 루틴에서 주로 프로그램 초기화를 수행하며 실행을
		// 위한 준비를 한다. 여기서는 메모리 DC를 만들고 두개의 비트맵을
		// 미리 읽어둔다.
	case WM_CREATE:
		hDC = GetDC(hWnd);
		hMemDC = CreateCompatibleDC(hDC);
		hMemDC2 = CreateCompatibleDC(hDC);
		ReleaseDC(hWnd, hDC);
		hbrick = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BRICK));      // 벽돌 모양 
		SelectObject(hMemDC, hbrick);
		hbrick2 = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BLANK));     // 벽돌을 지우기 위한 공백 
		SelectObject(hMemDC2, hbrick2);
		GameStart = FALSE;
		return 0;
		// 키보드 이벤트 처리. 상하좌우 키에 따라 벽돌을 움직이는 함수를
		// 호출한다. 
	case WM_KEYDOWN:
		switch (wParam) {
		case VK_LEFT:
			leftmove();
			break;
		case VK_RIGHT:
			rightmove();
			break;
		case VK_UP:
			rotate();
			break;
		case VK_DOWN:
			while (downmove());
			makenewblock();
			break;
		}
		return 0;
		// 타이머 이벤트가 발생할 때마다 벽돌을 한칸씩 아래로 내린다.
		// 타이머 이벤트는 일정하게 발생하므로 게임의 속도를 항상 일정
		// 하게 유지시킬 수 있다.  
	case WM_TIMER:
		if (downmove() == 0) makenewblock();
		return 0;
		// 화면을 다시 그려야 할 경우 전체 화면을 그리고 현재 이동중인
		// 벽돌은 별도로 다시 그린다. 
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);         // 작업 영역을 유효화시킨다.
		drawscreen();               // 화면을 다시 그리는 함수
		if (GameStart)
			brick(1);       // 이동중인 벽돌 그림
		return 0;
		// 메뉴 항목이 선택될 때 발생하는 메시지이다.
		// Start 항목이 선택되면 배열과 변수를 초기화하고 타이머를 생성
		// 하여 게임을 시작한다. 
	case WM_COMMAND:
		switch (wParam) {
		case ID_START:
			for (i = 0; i < 23; i++)for (j = 0; j < 14; j++)     // 배열을 초기화한다.
				if (i == 21 || j == 0 || j == 13)
					tet[j][i] = 2;
				else
					tet[j][i] = 0;
			speed = 500;
			score = 0;
			SetTimer(hWnd, 1, speed, NULL);
			nextbrick = random(7);
			makenewblock();
			EnableMenuItem(GetMenu(hWnd), ID_START, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
			GameStart = TRUE;
			InvalidateRect(hWnd, NULL, TRUE);
			UpdateWindow(hWnd);
			break;
		}
		return 0;
		// 윈도우가 파괴될 때 보내지는 메시지이다. 생성했던 DC를 모두 해제하고
		// 타이머를 정지시키며 GDI오브젝트를 삭제한다.
	case WM_DESTROY:
		DeleteDC(hMemDC);
		DeleteDC(hMemDC2);
		KillTimer(hWnd, 1);
		DeleteObject(hbrick);
		DeleteObject(hbrick2);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}

//######################################################
// 여기서부터 일반 함수들
// 사용하는 논리는 shtet1과 동일하다. 
//######################################################

// 새로운 블럭을 만든다. 블럭을 한단계 앞에 만들어 nextbrick
// 변수에 저장해 두고 다음 나올 벽돌 모양을 화면 우하단에
// 출력해 준다. 
void makenewblock()
{
	nowbrick = nextbrick;
	nextbrick = random(7);
	nowx = 8;
	nowy = 3;
	nowrot = 0;    // 변수 초기화
	brick(1);
	drawnextbrick();
	// 벽돌이 생성된 위치에 이미 벽돌이 있으면 게임을 끝낸다.
	// 게임을 끝낼 때는 반드시 타이머를 없애 주어야 한다.
	if (whataround(nowx, nowy) != 0) {
		KillTimer(hWnd, 1);
		GameStart = FALSE;
		if (MessageBox(hWnd, L"게임이 끝났네!\r\n재미있는데 또 할래?"
			, L"테트리스", MB_YESNO | MB_ICONHAND) == IDYES)
			SendMessage(hWnd, WM_COMMAND, ID_START, NULL);
		else
			EnableMenuItem(GetMenu(hWnd), ID_START, MF_BYCOMMAND | MF_ENABLED);
	}
}

// WM_PAINT 메시지 처리 루틴에서 호출하는 함수
// 화면을 다시 그린다. 
void drawscreen()
{
	HDC c;
	hDC = GetDC(hWnd);
	Rectangle(hDC, 8, 8, 252, 412);
	Rectangle(hDC, 260, 8, 460, 150);
	TextOut(hDC, 320, 20, L"Shtet 3", 7);

	printscore();

	// 배열값을 일일이 읽어서 벽돌들을 출력해 준다.
	Rectangle(hDC, 260, 300, 460, 412);
	for (i = 1; i <= 20; i++)
		for (j = 1; j <= 12; j++) {
			if (tet[j][i] == 1)
				c = hMemDC;
			else
				c = hMemDC2;
			BitBlt(hDC, (j - 1) * 20 + 10, (i - 1) * 20 + 10,
				20, 20, c, 0, 0, SRCCOPY);
		}
	ReleaseDC(hWnd, hDC);
	drawnextbrick();
}

void drawnextbrick()
{
	hDC = GetDC(hWnd);
	RECT rect = { 261,301,459,399 };
	FillRect(hDC, &rect, (HBRUSH)GetStockObject(WHITE_BRUSH));
	for (i = 0; i < 4; i++) {             // 패턴 배열을 읽어 화면에 그림 
		BitBlt(hDC, (17 + pat[nextbrick][i * 2] - 1) * 20,
			(18 + pat[nextbrick][i * 2 + 1] - 1) * 20,
			20, 20, hMemDC, 0, 0, SRCCOPY);
	}
	ReleaseDC(hWnd, hDC);
}

// 점수를 출력한다.
void printscore()
{
	wsprintf(imsistr, L"점수 : %d", score);
	TextOut(hDC, 260, 200, imsistr, wcslen(imsistr));
}

// 벽돌을 출력하거나 삭제한다.
void brick(int flag)
{
	HDC c;
	hDC = GetDC(hWnd);
	// flag값에 따라 벽돌 내지는 공백을 출력한다.
	if (flag == 0)
		c = hMemDC2;
	else
		c = hMemDC;
	for (i = 0; i < 4; i++) {             // 패턴 배열을 읽어 화면에 그림
		BitBlt(hDC, (nowx + pat[nowbrick][nowrot * 8 + i * 2] - 1) * 20 + 10,
			(nowy + pat[nowbrick][nowrot * 8 + i * 2 + 1] - 1) * 20 + 10, 20, 20, c, 0, 0, SRCCOPY);
	}
	ReleaseDC(hWnd, hDC);
}

// 벽돌을 한 칸 아래로 떨어뜨림. 빈 공간에서 떨어지면 1을 리턴,  
// 바닥이나 쌓인 벽돌에 부딪치면 벽돌 제거 후 0을 리턴하여      
// 새로운 벽돌을 만들도록 한다.                                   
int downmove()
{
	if (whataround(nowx, nowy + 1) != 0) {
		isfull();
		return 0;
	}
	brick(0);     // 원래 자리 지우고 한 칸 밑에 다시 그림
	nowy++;
	brick(1);
	return 1;
}

// 좌측 이동. 좌측이 벽이면 이동하지 않는다. 
void leftmove()
{
	if (whataround(nowx - 1, nowy) != 0)
		return;
	brick(0);
	nowx -= 1;
	brick(1);
}

// 우측 이동. 우측이 벽이면 이동하지 않는다. 
void rightmove()
{
	if (whataround(nowx + 1, nowy) != 0)
		return;
	brick(0);
	nowx += 1;
	brick(1);
}

// 회전. 회전할 수 없는 공간이면 회전하지 않는다 
void rotate()
{
	int imsi, imsi2;
	imsi = nowrot;
	nowrot++;
	if (nowrot == brickpatnum[nowbrick])
		nowrot = 0;
	if (whataround(nowx, nowy) != 0) {
		nowrot = imsi;
		return;
	}
	imsi2 = nowrot;
	nowrot = imsi;
	brick(0);
	nowrot = imsi2;
	brick(1);
}

// 주변에 무엇이 있는지 판별해준다. 인수(x,y)는 배열상의 좌표이다. 
// 빈 공간이면 0, 쌓인 벽돌에 부딪치면 1, 벽에 부딪치면 2를 리턴한다. 
int whataround(int x, int y)
{
	k = 0;
	for (i = 0; i < 4; i++) {
		j = tet[x + pat[nowbrick][nowrot * 8 + i * 2]]
			[y + pat[nowbrick][nowrot * 8 + i * 2 + 1]];
		if (j > k) k = j;
	}
	return k;
}

// 벽돌이 바닥에 닿았을 때의 처리. 전체 배열을 검사해서 한 라인이  
// 다 차면 삭제해준다.                                             
void isfull()
{
	int check, check2;
	score++;
	printscore();
	for (i = 0; i < 4; i++)       // 배열에 현재 떨어진 벽돌을 기록 
		tet[nowx + pat[nowbrick][nowrot * 8 + i * 2]]
		[nowy + pat[nowbrick][nowrot * 8 + i * 2 + 1]] = 1;
	check2 = 0;
	for (i = 20; i >= 0; i--) {     // 한 라인이 다 찼는지 검사 
		check = 0;
		for (j = 1; j < 13; j++)
			if (tet[j][i] == 0) check = 1;
		if (check == 0) {             // 다 찬 라인이 발견되면 삭제한다.  
			check2 = 1;                // 위의 라인을 아래로 복사해준다.   
			for (k = i - 1; k > 0; k--)      // 그 라인을 다시 검사하기 위해 i는 
				for (j = 1; j < 13; j++)       // 그 자리에 묶어 두어야 한다.      
					tet[j][k + 1] = tet[j][k];
			i++;
		}
	}

	if (check2 == 0) return;     // 한 라인도 지우지 못했으면 리턴  
	score += 10;
	KillTimer(hWnd, 1);
	speed -= 10;
	if (speed < 100) speed = 100;
	SetTimer(hWnd, 1, speed, NULL);
	drawscreen();
}