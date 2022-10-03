#include <windows.h>
#include <time.h>
#include <math.h>

#define TWOPI (2 * 3.14159)

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void DrawTrain(HWND, HDC, PAINTSTRUCT);
void RestartTimer(HWND);
void DrawingRailWays(HWND, HDC, PAINTSTRUCT);

const int TimerID = 51;
int moving = 0;

/*map[x][y][z]
* z: 0 = left-bottom
*	1 = bottom-right
*	2 = top-right
*	3 = left-top
*	4 = horizontal
*	5 = vertical
* def: 2 = train can ride
*		1 = road not available now
*		0 = not road;
*/
int map[15][10][6];

int trainDirection[20];

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
	
	PAINTSTRUCT ps;

	HDC hdc;

	RECT redrawingRect = {0, 30, 1800, 70};

	switch (msg)
	{
	case WM_CREATE:
		RestartTimer(hwnd);
		break;
	case WM_TIMER:
		if (wParam == TimerID) {
			moving++;
			RestartTimer(hwnd);
			InvalidateRect(hwnd, &redrawingRect, TRUE);
		}
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		DrawingRailWays(hwnd, hdc, ps);
		DrawTrain(hwnd, hdc, ps);
		EndPaint(hwnd, &ps);
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

void DrawTrain(HWND hwnd, HDC hdc, PAINTSTRUCT ps) {

	int horizontalMove, verticalMove;
	RECT rect;

	GetClientRect(hwnd, &rect);

	LOGBRUSH brush;
	DWORD pen_style = PS_GEOMETRIC; //PS_SOLID | PS_GEOMETRIC | PS_JOIN_BEVEL;

	brush.lbStyle = BS_SOLID;
	brush.lbColor = RGB(100, 120, 240);
	brush.lbHatch = 0;

	HPEN hPen = ExtCreatePen(pen_style, 25, &brush, 0, NULL);

	HPEN hOldPen = SelectObject(hdc, hPen);

	HBRUSH hBrush = CreateSolidBrush(RGB(100, 120, 240));
	HBRUSH hOldBrush = SelectObject(hdc, hBrush);

	
	if (trainDirection[0] == 105) {
		if (map[1][0][0] == 2 || map[1][0][3] == 2 || map[1][0][4] == 2) {
			moving = moving - 1;
		}
	}
	horizontalMove = moving;

	//draw 3 cars
	for (int i = 0; i < 180; i += 60)
	{
		MoveToEx(hdc, 0 - 230 + i + horizontalMove, 50, NULL); //350; 350
		LineTo(hdc, 0 - 200 + i + horizontalMove, 50);			//380; 350
	};

	//draw train
	MoveToEx(hdc, 0 - 50 + horizontalMove, 50, NULL); //530; 350
	LineTo(hdc, 0 + horizontalMove, 50);			//580; 350

	brush.lbColor = RGB(150, 150, 240);

	hPen = ExtCreatePen(pen_style, 15, &brush, 0, NULL);

	SelectObject(hdc, hPen);

	//visualisation for cars
	for (int i = 0; i <= 120; i += 60)
	{
		MoveToEx(hdc, 0 - 228 + i + horizontalMove, 50, NULL); //352 ; 350
		LineTo(hdc, 0 - 202 + i + horizontalMove, 50);			//378 ; 350
	};

	//visualisation for train
	MoveToEx(hdc, 0 - 48 + horizontalMove, 50, NULL);	//532 ; 350
	LineTo(hdc, 0 - 2 + horizontalMove, 50);			//578 ; 350

	brush.lbColor = RGB(100, 100, 220);

	hPen = ExtCreatePen(pen_style, 3, &brush, 0, NULL);
	SelectObject(hdc, hPen);

	hBrush = CreateSolidBrush(RGB(0, 0, 0));
	SelectObject(hdc, hBrush);

	//visualisation for cars - oil
	for (int i = 0; i < 39; i += 13)
	{
		Ellipse(hdc, 0 - 112 + i + horizontalMove, 46, 0 - 102 + i + horizontalMove, 55);	//468 ; 346 ; 478 ; 355
		Ellipse(hdc, 0 - 233 + i + horizontalMove, 46, 0 - 223 + i + horizontalMove, 55); //347 ; 346 ; 357 ; 355
	};

	//visualisation for train
	for (int i = 0; i < 54; i += 9)
	{
		if ((i <= 9) || (i >= 36)) {
			Ellipse(hdc, 0 - 49 + i + horizontalMove, 46, 0 - 45 + i + horizontalMove, 55); //531; 346; 535; 355
		}
	}

	brush.lbColor = RGB(153, 76, 0);

	hPen = ExtCreatePen(pen_style, 3, &brush, 0, NULL);
	SelectObject(hdc, hPen);

	//visualisation for cars - woods
	for (int i = 0; i <= 16; i += 4) {
		MoveToEx(hdc, 0 - 175 + horizontalMove, 42 + i, NULL);	//405; 342
		LineTo(hdc, 0 - 135 + horizontalMove, 42 + i);			//445; 342
	};

	brush.lbColor = RGB(0, 0, 0);

	hPen = ExtCreatePen(pen_style, 5, &brush, 0, NULL);
	SelectObject(hdc, hPen);

	//connections between the train and cars
	for (int i = 0; i <= 120; i += 60) {
		MoveToEx(hdc, 0 - 187 + i + horizontalMove, 50, NULL);	//393; 350
		LineTo(hdc, 0 - 183 + i + horizontalMove, 50);			//397; 350
	}	

	SelectObject(hdc, hOldBrush);
	SelectObject(hdc, hOldPen);

	DeleteObject(hPen);
	DeleteObject(hBrush);

	trainDirection[0] = horizontalMove;
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

//drawing straight railways

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

//drawing turning railways

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

	switch (iAngle)
	{
	case 1:
		y = y + 100;
		break;
	case 2:
		x = x + 100;
		y = y + 100;
		break;
	case 3:
		x = x + 100;
	default:
		break;
	}

	int tempAngle = 90 * iAngle;
	
	for (int i = (tempAngle + 10); i < tempAngle + 91; i += 10)
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

	SelectObject(hdc, hPenRailSteel);

	//Arc(hdc, xLeft, yTop, xRight, yBottom, xStart, yStart, xEnd, yEnd) ;

	for (int i = 0; i < 2; i++) {
		switch (iAngle)
		{
		case 1:
			Arc(hdc, (x - 55 + (i * 10)), (y - 55 + (i * 10)), (x + 55 - (i * 10)),
				(y + 55 - (i * 10)), (x + 55 - (i * 10)), y, x, (y - 55 + (i * 10)));
			break;
		case 2:
			Arc(hdc, (x - 55 + (i * 10)), (y - 55 + (i * 10)), (x + 55 - (i * 10)),
				(y + 55 - (i * 10)), x, (y - 55 + (i * 10)), (x - 55 + (i * 10)), y);
			break;
		case 3:
			Arc(hdc, (x - 55 + (i * 10)), (y - 55 + (i * 10)), (x + 55 - (i * 10)),
				(y + 55 - (i * 10)), (x - 55 + (i * 10)), y, x, (y + 45 + (i * 10)));
			break;
		case 4:
			Arc(hdc, (x - 55 + (i * 10)), (y - 55 + (i * 10)), (x + 55 - (i * 10)),
				(y + 55 - (i * 10)), x, (y + 55 - (i * 10)), (x + 55 - (i * 10)), y);
			break;

		}
	}

	SelectObject(hdc, hOldPen);
	DeleteObject(hPenRailSteel);
	DeleteObject(hPenRailWood);
}

void DrawingRailWays(HWND hwnd, HDC hdc, PAINTSTRUCT ps) {

	DrawingRotedRails(hwnd, hdc, ps, 1, 0, 1);
	map[1][0][0] = 2;

	DrawingRotedRails(hwnd, hdc, ps, 2, 1, 2);
	map[2][1][1] = 2;

	DrawingRotedRails(hwnd, hdc, ps, 1, 2, 3);
	map[1][2][2] = 2;

	DrawingRotedRails(hwnd, hdc, ps, 3, 2, 4);
	map[3][2][3] = 2;

	DrawingRotedRails(hwnd, hdc, ps, 3, 1, 1);
	map[3][1][0] = 2;

	DrawingRotedRails(hwnd, hdc, ps, 2, 3, 3);
	map[2][3][2] = 2;

	DrawStraightRails(hwnd, hdc, ps, 2, 2, TRUE);
	map[2][2][4] = 2;

	DrawStraightRails(hwnd, hdc, ps, 0, 0, TRUE);
	map[0][0][4] = 2;

	DrawStraightRails(hwnd, hdc, ps, 3, 3, TRUE);
	map[3][3][4] = 2;

	DrawStraightRails(hwnd, hdc, ps, 4, 3, TRUE);
	map[4][3][4] = 2;

	DrawStraightRails(hwnd, hdc, ps, 5, 3, TRUE);
	map[5][3][4] = 2;

	DrawStraightRails(hwnd, hdc, ps, 6, 3, TRUE);
	map[6][3][4] = 2;

	DrawStraightRails(hwnd, hdc, ps, 7, 3, TRUE);
	map[7][3][4] = 2;

	DrawStraightRails(hwnd, hdc, ps, 8, 3, TRUE);
	map[8][3][4] = 2;

	DrawStraightRails(hwnd, hdc, ps, 1, 1, FALSE);
	map[1][1][5] = 2;

	DrawStraightRails(hwnd, hdc, ps, 2, 2, FALSE);
	map[2][2][5] = 2;
}