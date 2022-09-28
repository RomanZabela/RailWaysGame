#include <windows.h>
#include <time.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void DrawTrain(HWND);
void RestartTimer(HWND);

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
		DrawTrain(hwnd);
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

	HBRUSH hBrush = CreateSolidBrush(RGB(11, 100, 150));
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