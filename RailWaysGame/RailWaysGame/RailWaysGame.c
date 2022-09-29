#include <windows.h>
#include <time.h>
#include <math.h>

#define TWOPI (2 * 3.14159)

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void DrawTrain(HWND);
void RestartTimer(HWND);
void DrawingRailWays(HWND);

const int TimerID = 1001;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR lpCmdLine, int nCmdShow) {
	
	MSG msg;
	WNDCLASS wc = { 0 };

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpszClassName = L"RailWays";
	wc.hInstance = hInstance;
	wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
	wc.lpfnWndProc = WndProc;
	wc.hCursor = LoadCursor(0, IDC_ARROW);

	RegisterClassW(&wc);
	CreateWindowW(wc.lpszClassName, L"RailWays", WS_SYSMENU | WS_MINIMIZEBOX| WS_VISIBLE, 200, 25, 1500, 1000, NULL, NULL, hInstance, NULL);

	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {	
	
	switch (msg)
	{
	case WM_PAINT:
		//DrawTrain(hwnd);
		DrawingRailWays(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProcW(hwnd, msg, wParam, lParam);
}

void RestartTimer(HWND hwnd) {
	SetTimer(hwnd, TimerID, 50, NULL);
}

void DrawTrain(HWND hwnd) {

	PAINTSTRUCT ps;

	HDC hdc = BeginPaint(hwnd, &ps);

	RECT rect;

	GetClientRect(hwnd, &rect);

	HPEN hPen = CreatePen(PS_NULL, 1, RGB(120, 0, 0));
	HPEN hOldPen = SelectObject(hdc, hPen);

	HBRUSH hBrush = CreateSolidBrush(RGB(51, 141, 108));
	HBRUSH hOldBrush = SelectObject(hdc, hBrush);

	Rectangle(hdc, 600, 450, 900, 550);
	Rectangle(hdc, 630, 400, 740, 451);
	Rectangle(hdc, 600, 440, 631, 451);
	Ellipse(hdc, 850, 450, 950, 550);

	hBrush = CreateSolidBrush(RGB(20, 20, 100));
	SelectObject(hdc, hBrush);

	Rectangle(hdc, 640, 430, 680, 470);
	Rectangle(hdc, 690, 430, 730, 470);

	hBrush = CreateSolidBrush(RGB(120, 20, 0));
	SelectObject(hdc, hBrush);

	Ellipse(hdc, 590, 500, 690, 600);
	Ellipse(hdc, 700, 500, 800, 600);

	Ellipse(hdc, 820, 540, 880, 600);

	SelectObject(hdc, hOldBrush);
	SelectObject(hdc, hOldPen);

	DeleteObject(hPen);
	DeleteObject(hBrush);

	EndPaint(hwnd, &ps);
}

void RotatePoint(POINT pt[], int iAngle)
{
	POINT ptTemp;

	for (int i = 0; i < 2; i++)
	{
		ptTemp.x = (int)(pt[i].x * cos(TWOPI * iAngle / 360) +
			pt[i].y * sin(TWOPI * iAngle / 360));

		ptTemp.y = (int)(pt[i].y * cos(TWOPI * iAngle / 360) -
			pt[i].x * sin(TWOPI * iAngle / 360));

		pt[i] = ptTemp;
	}
	
}

//drawing railways

void DrawStraightRails(HWND hwnd, HDC hdc, PAINTSTRUCT ps, int xBlock, int yBlock, BOOL HorizontalFlag) {
	
	LOGBRUSH brush;
	DWORD pen_style = PS_SOLID | PS_GEOMETRIC | PS_JOIN_BEVEL;

	brush.lbStyle = BS_SOLID;
	brush.lbColor = RGB(121, 121, 121);
	brush.lbHatch = 0;

	HPEN hPenRailSteel = ExtCreatePen(pen_style, 3, &brush, 0, NULL);


	brush.lbColor = RGB(196, 107, 29);
	HPEN hPenRailWood = ExtCreatePen(pen_style, 4, &brush, 0, NULL);
	HPEN hOldPen = SelectObject(hdc, hPenRailWood);

	int xMove = (100 * xBlock);
	int yMove = (100 * yBlock);
	int xTemp, yTemp;

	if (HorizontalFlag) {
		for (int i = 0; i < 10; i++)
		{
			xTemp = (i * 10) + 5 + xMove;
			MoveToEx(hdc, xTemp, (40 + yMove), NULL);
			LineTo(hdc, xTemp, (60 + yMove));
		};
	}
	else {
		for (int i = 0; i < 10; i++)
		{
			yTemp = (i * 10) + 5 + yMove;
			MoveToEx(hdc, (40 + xMove), yTemp, NULL);
			LineTo(hdc, (60 + xMove), yTemp);
		};
	};	

	SelectObject(hdc, hPenRailSteel);

	if (HorizontalFlag) {
		for (int i = 0; i < 2; i++) {
			yTemp = 45 + (10 * i) + yMove;
			MoveToEx(hdc, (xMove), (yTemp), NULL);
			LineTo(hdc, (100 + xMove), (yTemp));
		}
	}
	else {
		for (int i = 0; i < 2; i++) {
			xTemp = 45 + (10 * i) + xMove;
			MoveToEx(hdc, xTemp, yMove, NULL);
			LineTo(hdc, xTemp, (100 + yMove), NULL);
		};
	};
	

	SelectObject(hdc, hOldPen);
	DeleteObject(hPenRailSteel);
	DeleteObject(hPenRailWood);
}

void DrawingRotedRails(HWND hwnd, HDC hdc, PAINTSTRUCT ps, int xBlock, int yBlock, int iAngle) {
	LOGBRUSH brush;
	DWORD pen_style = PS_SOLID | PS_GEOMETRIC | PS_JOIN_BEVEL;

	brush.lbStyle = BS_SOLID;
	brush.lbColor = RGB(121, 121, 121);
	brush.lbHatch = 0;

	HPEN hPenRailSteel = ExtCreatePen(pen_style, 3, &brush, 0, NULL);

	brush.lbColor = RGB(196, 107, 29);
	HPEN hPenRailWood = ExtCreatePen(pen_style, 4, &brush, 0, NULL);
	HPEN hOldPen = SelectObject(hdc, hPenRailWood);

	POINT pt[2];
	int x = 100 * xBlock;
	int y = 100 * yBlock;
	int tempAngle = 90 * iAngle;
	
	for (int i = (tempAngle + 10); i < tempAngle + 90; i += 10)
	{
		pt[0].x = 0;
		pt[0].y = 40;

		pt[1].x = 0;
		pt[1].y = 60;

		RotatePoint(pt, i);

		pt[0].x = pt[0].x + x;
		pt[0].y = pt[0].y + y;

		pt[1].x = pt[1].x + x;
		pt[1].y = pt[1].y + y;

		MoveToEx(hdc, pt[0].x, pt[0].y, NULL);
		LineTo(hdc, pt[1].x, pt[1].y);
	};

	for (int i = 0; i < 2; i++) {
		Arc(hdc, 45, 45, 155, 155, 155, 100, 100, 45);
	}
	
	//Arc(hdc, 55, 55, 145, 145, 145, 100, 100, 55);

	SelectObject(hdc, hOldPen);
	DeleteObject(hPenRailSteel);
	DeleteObject(hPenRailWood);
}

void DrawingRailWays(HWND hwnd) {
	
	LOGBRUSH brush;
	DWORD pen_style = PS_SOLID | PS_GEOMETRIC | PS_JOIN_BEVEL;

	brush.lbStyle = BS_SOLID;
	brush.lbColor = RGB(121, 121, 121);
	brush.lbHatch = 0;

	PAINTSTRUCT ps;

	HDC hdc = BeginPaint(hwnd, &ps);

	HPEN hPenRailSteel = ExtCreatePen(pen_style, 3, &brush, 0, NULL);
	

	brush.lbColor = RGB(196, 107, 29);
	HPEN hPenRailWood = ExtCreatePen(pen_style, 4, &brush, 0, NULL);
	HPEN hOldPen = SelectObject(hdc, hPenRailWood);

	//painting wood part of railways

	//painting steelness part of railways

	SelectObject(hdc, hPenRailSteel);
	
	//Arc(hdc, xLeft, yTop, xRight, yBottom, xStart, yStart, xEnd, yEnd) ;
	Arc(hdc, 45, 45, 155, 155, 155, 100, 100, 45);
	Arc(hdc, 55, 55, 145, 145, 145, 100, 100, 55);

	Arc(hdc, 145, 145, 255, 255, 155, 200, 200, 245);
	Arc(hdc, 155, 155, 245, 245, 145, 200, 200, 255);

	SelectObject(hdc, hOldPen);

	DrawingRotedRails(hwnd, hdc, ps, 1, 1, 1);
	DrawingRotedRails(hwnd, hdc, ps, 2, 2, 3);

	DrawStraightRails(hwnd, hdc, ps, 2, 2, TRUE);
	
	DrawStraightRails(hwnd, hdc, ps, 0, 0, TRUE);

	DrawStraightRails(hwnd, hdc, ps, 1, 1, FALSE);

	DrawStraightRails(hwnd, hdc, ps, 2, 2, FALSE);

	DeleteObject(hPenRailSteel);
	DeleteObject(hPenRailWood);
	EndPaint(hwnd, &ps);
}