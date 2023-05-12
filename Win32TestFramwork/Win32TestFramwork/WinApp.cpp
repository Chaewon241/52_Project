#include <cstdlib>
#include "WinApp.h"
#include "GameManager.h"

namespace global
{
	WinApp winApp;

	const WinApp& GetWinApp()
	{
		return winApp;
	}
}

// const보다 강력함
constexpr int SCREEN_START_LEFT = 10;

constexpr int SCREEN_START_TOP = 10;

constexpr int SCREEN_WIDTH = 1024;

constexpr int SCREEN_HEIGHT = 768;

// 인스턴스 : 응용 프로그램의 아이디, 같은 종류의 프로그램은 같은 인스턴스를 가짐.
// 프로세스가 달라도 같은 종류의 인스턴스를 가짐
// 핸들 : 운영체제에서 할당한 자원의 아이디(윈도우, 펜, 브러쉬 등).
// 동종 프로그램일시라도 각 요소의 핸들 아이디는 다르다. 
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	// 안 씀
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	global::winApp.Initialize(hInstance);

	global::winApp.Run();

	global::winApp.Finalize();

	return EXIT_SUCCESS;
}


void PlaceInCenterOfScreen(HWND window, DWORD style, DWORD exStyle)
{
	// GetSystemMetrics -> 컴퓨터 사용 설정 함수
	// SM_CXSCREEN -> 현재 화면 해상도의 x축 크기
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	// 얜 y축 크기
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	// SM_CXFULLSCREEN, SM_CYFULLSCREEN : 윈도우가 최대화 되었을 때 x축 y축 크기
	// SM_CXFULLSCREEN은 해상도가 2560 * 1600이면 2560을 반환하는데
	// 사용자가 테스크 바를 아래쪽이 아니라 왼쪽이나 오른쪽에 두고 있다면 2560보다 작은 값이 나옴
	// SM_CYFULLSCREEN은 해상도가 2560 * 1600이면 1537을 반환함(테스크바의 높이만큼 제외하고 알려줌)

	RECT clientRect;
	GetClientRect(window, &clientRect);

	// 모니터 스크린 가져옴
	int clientWidth = clientRect.right - clientRect.left;
	int clientHeight = clientRect.bottom - clientRect.top;

	// 윈도우 위치 가운데로 오게 하기
	SetWindowPos(window, NULL,
		screenWidth / 2 - clientWidth / 2,
		screenHeight / 2 - clientHeight / 2,
		clientWidth, clientHeight, 0
	);
}

LRESULT CALLBACK WinApp::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
	{
		PlaceInCenterOfScreen(hWnd, WS_OVERLAPPEDWINDOW, WS_EX_APPWINDOW | WS_EX_WINDOWEDGE);
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


void WinApp::Initialize(HINSTANCE hInstance)
{
	m_hInstance = hInstance;

	const TCHAR* appName = TEXT("Test Game Framework");

	//Step 1: Registering the Window Class

	WNDCLASS wndClass;

	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = WndProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = hInstance;
	wndClass.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = appName;

	// 윈도우 클래스의 속성을 설정한 WNDCLASS 형식 변수의 주소를 전달하면결과를 반환
	RegisterClass(&wndClass);

	// Step 2: Creating the Window

	// 현재 클라이언트 영역
	RECT rect{ SCREEN_START_LEFT, SCREEN_START_TOP,
	SCREEN_START_LEFT + SCREEN_WIDTH, SCREEN_START_TOP + SCREEN_HEIGHT };

	// 현재 윈도우의 크기가 클라이언트 영역이 되도록 크기 조정
	// (현재 윈도우 크기, 윈도우 모양 스타일, 메뉴 여부)
	::AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;

	// 창 크기 고정 -> WS_OVERLAPPED | WS_SYSMENU
	m_hWnd = CreateWindow(appName, appName, WS_OVERLAPPED | WS_SYSMENU,
		SCREEN_START_LEFT, SCREEN_START_TOP, width, height, NULL, NULL, hInstance, NULL);

	ShowWindow(m_hWnd, SW_SHOWNORMAL);
	UpdateWindow(m_hWnd);

	// Step 3: Game Initialize Here
	game::GameManager::GetInstance()->Initialize();
}

void WinApp::Run()
{
	// Step 4: Game Loop Here
	game::GameManager::GetInstance()->Run();
}

void WinApp::Finalize()
{
	// Step 5: Game Finalize Here
	game::GameManager::GetInstance()->Finalize();
}