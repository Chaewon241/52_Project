#pragma once
namespace render
{
	void InitRender();

	void BeginDraw();

	void EndDraw();

	void ReleaseRender();

	void DrawLine(int x1, int y1, int x2, int y2, COLORREF color);

	void DrawRect(int x, int y, int width, int height, COLORREF color);

	void DrawCircle(int x, int y, int radius, COLORREF color);

	void DrawPolygon(POINT points[], int count, COLORREF color);

	void DrawText(int x, int y, const char* text, COLORREF color);

	void DrawBitmap(int x, int y, HBITMAP hBitmap);

	HBITMAP LoadImdage(const char* path);

	void ReleaseImage(HBITMAP hBitmap);
}