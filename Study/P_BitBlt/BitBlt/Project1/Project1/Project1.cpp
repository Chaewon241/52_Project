// Project1.cpp : 애플리케이션에 대한 진입점을 정의합니다.

#include "framework.h"
#include "Project1.h"

#pragma comment(lib, "msimg32.lib")		//


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;

	HDC hMemDC;
	HBITMAP hImage, hOldBitmap;
	int bx, by;
	BITMAP bit;

	switch (message) {
	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);
		hMemDC = CreateCompatibleDC(hdc); // 메모리 DC를 만든다

		// 비트맵 속성으로 파일을 불러오고 HBITMAP으로 형변환해서 hImage에 저장한다
		// fuLoad 속성: 리소스 대신 파일명으로, 호환 비트맵이 아닌 DIB 섹션 비트맵으로 불러온다
		hImage = (HBITMAP)LoadImage(NULL, TEXT("image.bmp"),
			IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

		// hImage가 선택되기 전의 핸들을 저장해 둔다
		hOldBitmap = (HBITMAP)SelectObject(hMemDC, hImage);

		// 비트맵 정보를 알아낸다
		GetObject(hImage, sizeof(BITMAP), &bit);
		bx = bit.bmWidth;
		by = bit.bmHeight;
/*
		unsigned int* p = new unsigned int[bx * 100];
		for (size_t i = 0; i < bx * 100; i++)
		{
			*(p+i) = 0x00FF0000;  //0xAARRGGBB
		}		
		// create bitmap
		{
			BITMAPINFO BmpInfo;
			BmpInfo.bmiHeader.biSize = sizeof(BITMAPINFO) - sizeof(RGBQUAD);
			BmpInfo.bmiHeader.biWidth = bx;
			BmpInfo.bmiHeader.biHeight = 0 - (int)100; //by
			BmpInfo.bmiHeader.biPlanes = 1;
			BmpInfo.bmiHeader.biBitCount = 32;		////0xAARRGGBB
			BmpInfo.bmiHeader.biCompression = BI_RGB;
			BmpInfo.bmiHeader.biSizeImage = 0;
			BmpInfo.bmiHeader.biXPelsPerMeter = 0;
			BmpInfo.bmiHeader.biYPelsPerMeter = 0;
			BmpInfo.bmiHeader.biClrUsed = 0;
			BmpInfo.bmiHeader.biClrImportant = 0;

			SetDIBits(hdc, hImage, 0, 100, p, &BmpInfo, DIB_RGB_COLORS);
			//SetBitmapBits(hImage, bx * 100 * 4, p);
		}

		delete[] p;
*/
    	// 화면의 (50, 50) 위치에 원본 크기로 원본의 (0, 0) 위치부터 추출하여 그린다
		BitBlt(hdc, 50, 50, bx, by, hMemDC, 0, 0, SRCCOPY);

    	// RGB(255, 0, 255)(핑크색)을 투명하게 만들고 그린다 //msimg32.lib
		TransparentBlt(hdc, 350, 50, bx, by, hMemDC, 0, 0, bx, by, RGB(255, 0, 255));

		// 이번에는 왼쪽을 보고 있고 한발을 내딛은 캐릭터(60, 200)를 그린다 
		TransparentBlt(hdc, 650, 50, (bx / 4) * 1, (by / 4) * 1,
			hMemDC, 60, 200, bx / 4, by / 4, RGB(255, 0, 255));

		SelectObject(hMemDC, hOldBitmap); // hImage 선택을 해제하기 위해 hOldBitmap을 선택한다
		DeleteObject(hImage); // 선택 해제된 비트맵을 제거한다

		DeleteDC(hMemDC); // 메모리 DC를 제거한다
		EndPaint(hWnd, &ps);
	}
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASS wc;

	wchar_t my_class_name[] = L"MyClassName";
	wc.cbClsExtra = NULL;
	wc.cbWndExtra = NULL;
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance = hInstance;
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = my_class_name;
	wc.lpszMenuName = NULL;
	wc.style = CS_HREDRAW | CS_VREDRAW;

	RegisterClass(&wc);

	HWND hWnd = CreateWindow(my_class_name, L"Test WinAPI", WS_OVERLAPPEDWINDOW,
		0, 0, 800, 600, NULL, NULL, hInstance, NULL);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

