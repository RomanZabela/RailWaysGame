#include <windows.h>
#include <time.h>
#include <math.h> 

#define TWOPI (2 * 3.14159)

struct Road {
	int leftBottom;
	int bottomRight;
	int topRight;
	int leftTop;
	int horizontal;
	int vertical;
	byte isRoad;
};

/*struct Color
{
	int Red;
	int Green;
	int Blue;
};*/

struct City {
	int xBlock;
	int yBlock;
	COLORREF Color; //BGR
};

struct Train {
	int headX;
	int headY;
	int tailX;
	int tailY;
	int blockX;
	int blockY;
	int preBlockX;
	int preBlockY;
	COLORREF Color;
};

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void DrawTrain(HWND, HDC, PAINTSTRUCT);
void RestartTimer(HWND);
void DrawingRailWays(HWND, HDC, PAINTSTRUCT);
void DrawStraightRails(HWND, HDC, PAINTSTRUCT, int, int, BOOL);
void DrawingRotedRails(HWND, HDC, PAINTSTRUCT, int, int, int);
void CityDrawing(HWND, HDC, PAINTSTRUCT, int*);
void RandomCities(const int);
BYTE FindNutUsingPosition(const int*, const int*);
BYTE FindColor(const int*, const int*);

const COLORREF BankOfColors[] = {0x000099, 0x9999FF, 0x8000FF, 0x00994c,
								0x009999, 0x004c99, 0xCC0000, 0x994c00,
								0x660000, 0xFF3399, 0xFF9933, 0x999900,
								0x4c9900, 0x990099};

const int TimerID = 51;
const int newTrainTimer = 1200;
const int newCityTimer = 600;
int timer = 1001;
int trainsOnTheMap = -1;
int citiesOnTheMap = -1;

/*map[x][y][z]
* z: 0 = left-bottom
*	1 = bottom-right
*	2 = top-right
*	3 = left-top
*	4 = horizontal
*	5 = vertical
* def: 2 = train can ride
*		1 = road not available now
*		0 = no road;
*/
struct Road map[14][10];

struct City cities[14];

/*trainDirection[x][y]
* x: train - "-1" no train on the map
* y:	0 - headX
*		1 - headY
*		2 - tailX
*		3 - tailY
*		4 - current BlockX
*		5 - current BlockY
*		6 - previous BlockX = "-1" train just started
*		7 - previous BlockY
*/
struct Train trains[20];

int rightButton;
POINT mouse, nextMouse;

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
	CreateWindowW(wc.lpszClassName, L"RailWays", WS_SYSMENU | WS_MINIMIZEBOX| WS_VISIBLE, 200, 25, 1400, 1050, NULL, NULL, hInstance, NULL);

	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	srand(time(NULL));

	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {	
	
	PAINTSTRUCT ps;

	HDC hdc;

	int rectLeft = trains[0].blockX * 100 - 100;
	int rectRigth = trains[0].blockX * 100 + 140;
	int rectTop = trains[0].blockY * 100 - 100;
	int rectBottom = trains[0].blockY * 100 + 140;

	RECT redrawingRect = {rectLeft, rectTop, rectRigth, rectBottom};

	switch (msg)
	{
	case WM_CREATE:
		RestartTimer(hwnd);
		rightButton = -1;
		citiesOnTheMap = 1;

		RandomCities(0);
		RandomCities(1);
		break;
	case WM_TIMER:
		if (wParam == TimerID) {
			timer++;
			RestartTimer(hwnd);
			//InvalidateRect(hwnd, NULL, TRUE);
		}
		if (timer % newTrainTimer == 0) {
			trainsOnTheMap++;
			trains[trainsOnTheMap].headX = 0;
			trains[trainsOnTheMap].headY = 50;
			trains[trainsOnTheMap].tailX = -50;
			trains[trainsOnTheMap].tailY = 50;
			trains[trainsOnTheMap].blockX = 0;
			trains[trainsOnTheMap].blockY = 0;
			trains[trainsOnTheMap].preBlockX = 0;
			trains[trainsOnTheMap].preBlockY = 0;
			
		}
		if (timer % newCityTimer == 0) {

			if (citiesOnTheMap < 13) {
				citiesOnTheMap++;

				RandomCities(citiesOnTheMap);
			}

		}
		if (timer % (newCityTimer + 1) == 0) {

			if (citiesOnTheMap < 13) {
				redrawingRect.left = cities[citiesOnTheMap].xBlock * 100;
				redrawingRect.top = cities[citiesOnTheMap].yBlock * 100;
				redrawingRect.right = cities[citiesOnTheMap].xBlock * 100 + 100;
				redrawingRect.bottom = cities[citiesOnTheMap].yBlock * 100 + 100;

				InvalidateRect(hwnd, &redrawingRect, TRUE);
			}

		}

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		DrawingRailWays(hwnd, hdc, ps);
		DrawTrain(hwnd, hdc, ps);
		CityDrawing(hwnd, hdc, ps, &citiesOnTheMap);
		EndPaint(hwnd, &ps);		

		break;
	case WM_RBUTTONDOWN:
		if (rightButton == 5) {
			rightButton = -1;
		}
		else {
			rightButton++;
		}
		break;
	case WM_RBUTTONUP:	

		mouse.x = LOWORD(lParam) / 100;
		mouse.y = HIWORD(lParam) / 100;

		if (mouse.x != 0 && mouse.x != 13) {
			switch (rightButton)
			{
			case 0:
				map[mouse.x][mouse.y].horizontal = 2;
				map[mouse.x][mouse.y].isRoad = TRUE;
				break;
			case 1:
				map[mouse.x][mouse.y].horizontal = 0;
				map[mouse.x][mouse.y].vertical = 2;
				map[mouse.x][mouse.y].isRoad = TRUE;
				break;
			case 2:
				map[mouse.x][mouse.y].vertical = 0;
				map[mouse.x][mouse.y].leftBottom = 2;
				map[mouse.x][mouse.y].isRoad = TRUE;
				break;
			case 3:
				map[mouse.x][mouse.y].leftBottom = 0;
				map[mouse.x][mouse.y].bottomRight = 2;
				map[mouse.x][mouse.y].isRoad = TRUE;
				break;
			case 4:
				map[mouse.x][mouse.y].bottomRight = 0;
				map[mouse.x][mouse.y].topRight = 2;
				map[mouse.x][mouse.y].isRoad = TRUE;
				break;
			case 5:
				map[mouse.x][mouse.y].topRight = 0;
				map[mouse.x][mouse.y].leftTop = 2;
				map[mouse.x][mouse.y].isRoad = TRUE;
				break;
			case -1:
				map[mouse.x][mouse.y].leftTop = 0;
				map[mouse.x][mouse.y].isRoad = FALSE;
			default:
				break;
			}

			redrawingRect.left = mouse.x * 100;
			redrawingRect.top = mouse.y * 100;
			redrawingRect.right = (mouse.x * 100) + 100;
			redrawingRect.bottom = (mouse.y * 100) + 100;

			InvalidateRect(hwnd, &redrawingRect, TRUE);
		}
		break;
	case WM_LBUTTONDOWN:
		break;
	case WM_LBUTTONUP:
		break;
	case WM_MOUSEMOVE:

		nextMouse.x = LOWORD(lParam) / 100;
		nextMouse.y = HIWORD(lParam) / 100;

		if (&mouse != &nextMouse) {
			rightButton = -1;
		}

		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	};

	return DefWindowProcW(hwnd, msg, wParam, lParam);
}

void RestartTimer(HWND hwnd) {
	SetTimer(hwnd, TimerID, 50, NULL);
}

int CorrectTail(int Tail, const int Block) {

	if (Tail - Block < 50) {
		return Tail + 1;
	}
	else if (Tail - Block > 50) {
		return Tail - 1;
	}
	else {
		return Tail;
	}
}

void DirectMoving(int numberTrain, byte Vertical) {
	
	int headX = trains[numberTrain].headX;
	int headY = trains[numberTrain].headY;
	int tailX = trains[numberTrain].tailX;
	int tailY = trains[numberTrain].tailY;
	int blockX = (trains[numberTrain].blockX * 100);
	int blockY = (trains[numberTrain].blockY * 100);
	
	byte forwardVertical, forwardHorizontal;
	
	forwardVertical = trains[numberTrain].blockY - trains[numberTrain].preBlockY < 0;
	forwardHorizontal = trains[numberTrain].blockX - trains[numberTrain].preBlockX < 0;
	
	if (Vertical) {
		if (forwardVertical) {
			headY--;
			tailY--;

			tailX = CorrectTail(tailX, blockX);
		}
		else {
			headY++;
			tailY++;

			tailX = CorrectTail(tailX, blockX);
		}

	}
	else {
		if (forwardHorizontal) {
			headX--;
			tailX--;

			tailY = CorrectTail(tailY, blockY);
		}
		else
		{
			headX++;
			tailX++;

			tailY = CorrectTail(tailY, blockY);
		}
	}

	trains[numberTrain].headX = headX;
	trains[numberTrain].headY = headY;
	trains[numberTrain].tailX = tailX;
	trains[numberTrain].tailY = tailY;
}

void TurningTrainLeftBottom(int numberTrain) {

	int headX = trains[numberTrain].headX;
	int headY = trains[numberTrain].headY;
	int tailX = trains[numberTrain].tailX;
	int tailY = trains[numberTrain].tailY;
	int blockX = (trains[numberTrain].blockX * 100);
	int blockY = (trains[numberTrain].blockY * 100);

	tailY = CorrectTail(tailY, blockY);

	if (headX < (blockX + 20)) {
		headX++;
		tailX++;
	}

	if (headX >= (blockX + 20) && headX < (blockX + 50)) {
		headX++;
		tailX += 2;
		headY++;
	}

	if (headY >= (blockY + 79) && headY <= (blockY + 100)) {
		tailX++;
		headY += 2;
	}

	trains[numberTrain].headX = headX;
	trains[numberTrain].headY = headY;
	trains[numberTrain].tailX = tailX;
	trains[numberTrain].tailY = tailY;
}

void TurningTrainBottomLeft(int numberTrain) {

	int headX = trains[numberTrain].headX;
	int headY = trains[numberTrain].headY;
	int tailX = trains[numberTrain].tailX;
	int tailY = trains[numberTrain].tailY;
	int blockX = (trains[numberTrain].blockX * 100);
	int blockY = (trains[numberTrain].blockY * 100 + 100);

	tailX = CorrectTail(tailX, blockX);

	if (headY > (blockY - 20)) {
		headY--;
		tailY--;
	}

	if (headY <= (blockY - 20) && headY > (blockY - 50)) {
		headY--;
		tailY -= 2;
		headX--;
	}

	if (headX <= (blockX + 29) && headX >= (blockX)) {
		tailY--;
		headX -= 2;
	}

	trains[numberTrain].headX = headX;
	trains[numberTrain].headY = headY;
	trains[numberTrain].tailX = tailX;
	trains[numberTrain].tailY = tailY;
}

void TurningTrainTopRight(int numberTrain) {

	int headX = trains[numberTrain].headX;
	int headY = trains[numberTrain].headY;
	int tailX = trains[numberTrain].tailX;
	int tailY = trains[numberTrain].tailY;
	int blockX = (trains[numberTrain].blockX * 100);
	int blockY = (trains[numberTrain].blockY * 100);

	tailX = CorrectTail(tailX, blockX);

	if (headY < (blockY + 20)) {
		headY++;
		tailY++;
	}

	if (headY >= (blockY + 20) && headY < (blockY + 50)) {
		headY++;
		tailY += 2;
		headX++;
	}

	if (headX >= (blockX + 79) && headX <= (blockX + 100)) {
		tailY++;
		headX += 2;
	}

	trains[numberTrain].headX = headX;
	trains[numberTrain].headY = headY;
	trains[numberTrain].tailX = tailX;
	trains[numberTrain].tailY = tailY;
}

void TurningTrainRightTop(int numberTrain) {

	int headX = trains[numberTrain].headX;
	int headY = trains[numberTrain].headY;
	int tailX = trains[numberTrain].tailX;
	int tailY = trains[numberTrain].tailY;
	int blockX = (trains[numberTrain].blockX * 100 + 100);
	int blockY = (trains[numberTrain].blockY * 100);

	tailY = CorrectTail(tailY, blockY);

	if (headX > (blockX - 20)) {
		headX--;
		tailX--;
	}

	if (headX <= (blockX - 20) && headX > (blockX - 50)) {
		headX--;
		tailX -= 2;
		headY--;
	}

	if (headY <= (blockY + 29) && headY >= (blockY)) {
		tailX--;
		headY -= 2;
	}

	trains[numberTrain].headX = headX;
	trains[numberTrain].headY = headY;
	trains[numberTrain].tailX = tailX;
	trains[numberTrain].tailY = tailY;
}

void TurningTrainLeftTop(int numberTrain) {
	
	int headX = trains[numberTrain].headX;
	int headY = trains[numberTrain].headY;
	int tailX = trains[numberTrain].tailX;
	int tailY = trains[numberTrain].tailY;
	int blockX = (trains[numberTrain].blockX * 100);
	int blockY = (trains[numberTrain].blockY * 100);

	tailY = CorrectTail(tailY, blockY);

	if (headX < (blockX + 20)) {
		headX++;
		tailX++;
	}

	if (headX >= (blockX + 20) && headX < (blockX + 50)) {
		headX++;
		tailX += 2;
		headY--;
	}

	if (headY <= (blockY + 29) && headY >= (blockY)) {
		tailX++;
		headY -= 2;
	}

	trains[numberTrain].headX = headX;
	trains[numberTrain].headY = headY;
	trains[numberTrain].tailX = tailX;
	trains[numberTrain].tailY = tailY;
}

void TurningTrainTopLeft(int numberTrain) {

	int headX = trains[numberTrain].headX;
	int headY = trains[numberTrain].headY;
	int tailX = trains[numberTrain].tailX;
	int tailY = trains[numberTrain].tailY;
	int blockX = (trains[numberTrain].blockX * 100);
	int blockY = (trains[numberTrain].blockY * 100);

	tailX = CorrectTail(tailX, blockX);

	if (headY < (blockY + 20)) {
		headY++;
		tailY++;
	}

	if (headY >= (blockY + 20) && headY < (blockY + 50)) {
		headY++;
		tailY += 2;
		headX--;
	}

	if (headX <= (blockX + 29) && headX >= (blockX)) {
		tailY++;
		headX -= 2;
	}

	trains[numberTrain].headX = headX;
	trains[numberTrain].headY = headY;
	trains[numberTrain].tailX = tailX;
	trains[numberTrain].tailY = tailY;
}

void TurningTrainRightBottom(int numberTrain) {

	int headX = trains[numberTrain].headX;
	int headY = trains[numberTrain].headY;
	int tailX = trains[numberTrain].tailX;
	int tailY = trains[numberTrain].tailY;
	int blockX = (trains[numberTrain].blockX * 100 + 100);
	int blockY = (trains[numberTrain].blockY * 100);

	tailY = CorrectTail(tailY, blockY);

	if (headX > (blockX - 20)) {
		headX--;
		tailX--;
	}

	if (headX <= (blockX - 20) && headX > (blockX - 50)) {
		headX--;
		tailX -= 2;
		headY++;
	}

	if (headY >= (blockY + 79) && headY <= (blockY + 100)) {
		tailX--;
		headY += 2;
	}

	trains[numberTrain].headX = headX;
	trains[numberTrain].headY = headY;
	trains[numberTrain].tailX = tailX;
	trains[numberTrain].tailY = tailY;
}

void TurningTrainBottomRight(int numberTrain) {

	int headX = trains[numberTrain].headX;
	int headY = trains[numberTrain].headY;
	int tailX = trains[numberTrain].tailX;
	int tailY = trains[numberTrain].tailY;
	int blockX = (trains[numberTrain].blockX * 100);
	int blockY = (trains[numberTrain].blockY * 100 + 100);

	tailX = CorrectTail(tailX, blockX);

	if (headY > (blockY - 20)) {
		headY--;
		tailY--;
	}

	if (headY <= (blockY - 20) && headY > (blockY - 50)) {
		headY--;
		tailY -= 2;
		headX++;
	}

	if (headX >= (blockX + 79) && headX <= (blockX + 100)) {
		tailY--;
		headX += 2;
	}

	trains[numberTrain].headX = headX;
	trains[numberTrain].headY = headY;
	trains[numberTrain].tailX = tailX;
	trains[numberTrain].tailY = tailY;
}

void DrawTrain(HWND hwnd, HDC hdc, PAINTSTRUCT ps) {

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

	if (trainsOnTheMap != -1) {
		for (int i = 0; i <= trainsOnTheMap; i++) {

			if (map[trains[i].blockX][trains[i].blockY].leftBottom == 2) {
				if (trains[i].blockY - trains[i].preBlockY == 0) {
					TurningTrainLeftBottom(i);
				}
				else {
					TurningTrainBottomLeft(i);
				}
			}

			if (map[trains[i].blockX][trains[i].blockY].bottomRight == 2) {
				if (trains[i].blockY - trains[i].preBlockY == 0) {
					TurningTrainRightBottom(i);
				}
				else {
					TurningTrainBottomRight(i);
				}
			}

			if (map[trains[i].blockX][trains[i].blockY].topRight == 2) {
				if (trains[i].blockX - trains[i].preBlockX == 0) {
					TurningTrainTopRight(i);
				}
				else {
					TurningTrainRightTop(i);
				}
			}

			if (map[trains[i].blockX][trains[i].blockY].leftTop == 2) {
				if (trains[i].blockY - trains[i].preBlockY == 0) {
					TurningTrainLeftTop(i);
				}
				else
				{
					TurningTrainTopLeft(i);
				}
			}

			//Horizontal
			if (map[trains[i].blockX][trains[i].blockY].horizontal == 2 && (trains[i].blockY - trains[i].preBlockY == 0)) {
				DirectMoving(i, FALSE);
			}

			//Vertical
			if (map[trains[i].blockX][trains[i].blockY].vertical == 2 && (trains[i].blockX - trains[i].preBlockX == 0)) {
				DirectMoving(i, TRUE);
			}

			if (trains[i].headX % 100 == 0) {
				if (trains[i].blockX == trains[i].headX / 100 && map[(trains[i].headX / 100) - 1][trains[i].blockY].isRoad) {
					trains[i].preBlockX = trains[i].blockX;
					trains[i].preBlockY = trains[i].blockY;

					trains[i].blockX = (trains[i].headX / 100) - 1;
				}
				else if (trains[i].blockX != trains[i].headX / 100 && map[(trains[i].headX / 100)][trains[i].blockY].isRoad) {
					trains[i].preBlockX = trains[i].blockX;
					trains[i].preBlockY = trains[i].blockY;

					trains[i].blockX = trains[i].headX / 100;
				}
				else {
					trains[i].blockX = -1;
					trains[i].blockY = -1;
				}
			}
			if (trains[i].headY % 100 == 0) {
				if (trains[i].blockY == trains[i].headY / 100 && map[trains[i].blockX][(trains[i].headY / 100) - 1].isRoad) {
					trains[i].preBlockX = trains[i].blockX;
					trains[i].preBlockY = trains[i].blockY;

					trains[i].blockY = (trains[i].headY / 100) - 1;
				}
				else  if (trains[i].blockY != trains[i].headY / 100 && map[trains[i].blockX][trains[i].headY / 100].isRoad) {
					trains[i].preBlockX = trains[i].blockX;
					trains[i].preBlockY = trains[i].blockY;

					trains[i].blockY = trains[i].headY / 100;
				}
				else {
					trains[i].blockX = -1;
					trains[i].blockY = -1;
				}

			}

			if (trainsOnTheMap != -1) {
				MoveToEx(hdc, trains[i].headX, trains[i].headY, NULL); //530; 350
				LineTo(hdc, trains[i].tailX, trains[i].tailY);			//580; 350
			}
		}
	}
		/*
			//draw 3 cars
		//for (int i = 0; i < 180; i += 60)
		//{
		//	MoveToEx(hdc, 0 - 230 + i + horizontalMove, 50, NULL); //350; 350
		//	LineTo(hdc, 0 - 200 + i + horizontalMove, 50);			//380; 350
		//}

		//draw train
		MoveToEx(hdc, headX, headY, NULL); //530; 350
		LineTo(hdc, tailX - 50, tailY);			//580; 350

		MoveToEx(hdc, 0 - 230 + horizontalMove, 50, NULL); //350; 350
		LineTo(hdc, 0 - 200 + horizontalMove, 50);			//380; 350

		MoveToEx(hdc, 0 - 230 + 60 + horizontalMove, 50, NULL); //350; 350
		LineTo(hdc, 0 - 200 + 60 + horizontalMove, 50);			//380; 350

		MoveToEx(hdc, 0 - 230 + 120 + horizontalMove, 50, NULL); //350; 350
		LineTo(hdc, 0 - 200 + 120 + horizontalMove, 50);			//380; 350

		MoveToEx(hdc, 0 - 230 + 180 + horizontalMove, 50, NULL); //350; 350
		LineTo(hdc, 0 - 200 + 180 + horizontalMove, 50);			//380; 350
		
		brush.lbColor = RGB(150, 150, 240);

		hPen = ExtCreatePen(pen_style, 15, &brush, 0, NULL);

		SelectObject(hdc, hPen);

		//visualisation for cars
		//for (int i = 0; i <= 120; i += 60)
		//{
		//	MoveToEx(hdc, 0 - 228 + i + horizontalMove, 50, NULL); //352 ; 350
		//	LineTo(hdc, 0 - 202 + i + horizontalMove, 50);			//378 ; 350
		//};

		//visualisation for train
		/*MoveToEx(hdc, 0 - 48 + horizontalMove, 50, NULL);	//532 ; 350
		LineTo(hdc, 0 - 2 + horizontalMove, 50);			//578 ; 350

		MoveToEx(hdc, 0 - 228 + horizontalMove, 50, NULL); //352 ; 350
		LineTo(hdc, 0 - 202 + horizontalMove, 50);			//378 ; 350

		MoveToEx(hdc, 0 - 228 + 60 + horizontalMove, 50, NULL); //352 ; 350
		LineTo(hdc, 0 - 202 + 60 + horizontalMove, 50);			//378 ; 350

		MoveToEx(hdc, 0 - 228 + 120 + horizontalMove, 50, NULL); //352 ; 350
		LineTo(hdc, 0 - 202 + 120 + horizontalMove, 50);			//378 ; 350
		
		brush.lbColor = RGB(100, 100, 220);

		hPen = ExtCreatePen(pen_style, 3, &brush, 0, NULL);
		SelectObject(hdc, hPen);

		hBrush = CreateSolidBrush(RGB(0, 0, 0));
		SelectObject(hdc, hBrush);

		//visualisation for cars - oil
		//for (int i = 0; i < 39; i += 13)
		//{
		//	Ellipse(hdc, 0 - 112 + i + horizontalMove, 46, 0 - 102 + i + horizontalMove, 55);	//468 ; 346 ; 478 ; 355
		//	Ellipse(hdc, 0 - 233 + i + horizontalMove, 46, 0 - 223 + i + horizontalMove, 55); //347 ; 346 ; 357 ; 355
		//};

		//visualisation for train
		//for (int i = 0; i < 54; i += 9)
		//{
		//	if ((i <= 9) || (i >= 36)) {
		//		Ellipse(hdc, 0 - 49 + i + horizontalMove, 46 + verticalMove, 0 - 45 + i + horizontalMove, 55 + verticalMove); //531; 346; 535; 355
		//	}
		//}

		/*Ellipse(hdc, 0 - 49 + horizontalMove, 46, 0 - 45 + horizontalMove, 55); //531; 346; 535; 355
		Ellipse(hdc, 0 - 49 + 9 + horizontalMove, 46, 0 - 45 + 9 + horizontalMove, 55); //531; 346; 535; 355
		Ellipse(hdc, 0 - 49 + 36 + horizontalMove, 46, 0 - 45 + 36 + horizontalMove, 55); //531; 346; 535; 355
		Ellipse(hdc, 0 - 49 + 45 + horizontalMove, 46, 0 - 45 + 45 + horizontalMove, 55); //531; 346; 535; 355

		Ellipse(hdc, 0 - 112 + horizontalMove, 46, 0 - 102 + horizontalMove, 55);	//468 ; 346 ; 478 ; 355
		Ellipse(hdc, 0 - 233 + horizontalMove, 46, 0 - 223 + horizontalMove, 55); //347 ; 346 ; 357 ; 355

		Ellipse(hdc, 0 - 112 + 13 + horizontalMove, 46, 0 - 102 + 13 + horizontalMove, 55);	//468 ; 346 ; 478 ; 355
		Ellipse(hdc, 0 - 233 + 13 + horizontalMove, 46, 0 - 223 + 13 + horizontalMove, 55); //347 ; 346 ; 357 ; 355

		Ellipse(hdc, 0 - 112 + 26 + horizontalMove, 46, 0 - 102 + 26 + horizontalMove, 55);	//468 ; 346 ; 478 ; 355
		Ellipse(hdc, 0 - 233 + 26 + horizontalMove, 46, 0 - 223 + 26 + horizontalMove, 55); //347 ; 346 ; 357 ; 355
		
		brush.lbColor = RGB(153, 76, 0);

		hPen = ExtCreatePen(pen_style, 3, &brush, 0, NULL);
		SelectObject(hdc, hPen);

		//visualisation for cars - woods
		/*for (int i = 0; i <= 16; i += 4) {
			MoveToEx(hdc, 0 - 175 + horizontalMove, 42 + i, NULL);	//405; 342
			LineTo(hdc, 0 - 135 + horizontalMove, 42 + i);			//445; 342
		};

		brush.lbColor = RGB(0, 0, 0);

		hPen = ExtCreatePen(pen_style, 5, &brush, 0, NULL);
		SelectObject(hdc, hPen);

		//connections between the train and cars
		//for (int i = 0; i <= 120; i += 60) {
		//	MoveToEx(hdc, 0 - 187 + i + horizontalMove, 50, NULL);	//393; 350
		//	LineTo(hdc, 0 - 183 + i + horizontalMove, 50);			//397; 350
		//}	

		/*MoveToEx(hdc, 0 - 187 + horizontalMove, 50, NULL);	//393; 350
		LineTo(hdc, 0 - 183 + horizontalMove, 50);			//397; 350

		MoveToEx(hdc, 0 - 187 + 60 + horizontalMove, 50, NULL);	//393; 350
		LineTo(hdc, 0 - 183 + 60 + horizontalMove, 50);			//397; 350

		MoveToEx(hdc, 0 - 187 + 120 + horizontalMove, 50, NULL);	//393; 350
		LineTo(hdc, 0 - 183 + 120 + horizontalMove, 50);			//397; 350
		*/
	
	SelectObject(hdc, hOldBrush);
	SelectObject(hdc, hOldPen);

	DeleteObject(hPen);
	DeleteObject(hBrush);
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

	for (int i = 0; i < 15; i++) {
		for (int j = 0; j < 10; j++) {
			if (map[i][j].isRoad) {
				if (map[i][j].horizontal == 2) {
					DrawStraightRails(hwnd, hdc, ps, i, j, TRUE);
				}
				if (map[i][j].vertical == 2) {
					DrawStraightRails(hwnd, hdc, ps, i, j, FALSE);
				}
				if (map[i][j].leftBottom == 2) {
					DrawingRotedRails(hwnd, hdc, ps, i, j, 1);
				}
				if (map[i][j].bottomRight == 2) {
					DrawingRotedRails(hwnd, hdc, ps, i, j, 2);
				}
				if (map[i][j].topRight == 2) {
					DrawingRotedRails(hwnd, hdc, ps, i, j, 3);
				}
				if (map[i][j].leftTop == 2) {
					DrawingRotedRails(hwnd, hdc, ps, i, j, 4);
				}
			}
		}
	}
}

void CityDrawing(HWND hwnd, HDC hdc, PAINTSTRUCT ps, int* numberOfCities) {

	RECT rect;

	GetClientRect(hwnd, &rect);

	LOGBRUSH brush;
	DWORD pen_style = PS_SOLID | PS_GEOMETRIC | PS_JOIN_ROUND;

	brush.lbStyle = BS_SOLID;
	brush.lbColor = RGB(0, 0, 0);
	brush.lbHatch = 0;

	HPEN hPenBuild = ExtCreatePen(pen_style, 2, &brush, 0, NULL);

	brush.lbColor = RGB(252, 252, 25);

	HPEN hPenWindows = ExtCreatePen(pen_style, 1, &brush, 0, NULL);

	HPEN hOldPen = SelectObject(hdc, hPenBuild);

	
	HBRUSH hBrushWindows = CreateSolidBrush(RGB(252, 252, 25));

	POINT triangle[3];
	int x, y;

	for (int i = 0; i <= *numberOfCities; i++) {

		HBRUSH hBrushBuild = CreateSolidBrush(cities[i].Color);

		SelectObject(hdc, hPenBuild);
		HBRUSH hOldBrush = SelectObject(hdc, hBrushBuild);
		
		x = cities[i].xBlock * 100;
		y = cities[i].yBlock * 100;

		triangle[0].x = 36 + x;
		triangle[0].y = 70 + y;

		triangle[1].x = 50 + x;
		triangle[1].y = 60 + y;

		triangle[2].x = 62 + x;
		triangle[2].y = 70 + y;

		Rectangle(hdc, 15 + x, 10 + y, 45 + x, 50 + y);

		SelectObject(hdc, hBrushWindows);
		SelectObject(hdc, hPenWindows);

		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 2; j++) {
				Rectangle(hdc, 20 + (15 * i) + x, 15 + (20 * j) + y,
						25 + (15 * i) + x, 25 + (20 * j) + y);
			}
		}

		SelectObject(hdc, hBrushBuild);
		SelectObject(hdc, hPenBuild);

		Rectangle(hdc, 5 + x, 40 + y, 35 + x, 80 + y);
		Rectangle(hdc, 40 + x, 70 + y, 60 + x, 90 + y);
		Rectangle(hdc, 55 + x, 15 + y, 75 + x, 35 + y);

		Polygon(hdc, triangle, 3);

		triangle[0].x = 51 + x;
		triangle[0].y = 15 + y;

		triangle[1].x = 65 + x;
		triangle[1].y = 5 + y;

		triangle[2].x = 77 + x;
		triangle[2].y = 15 + y;

		Polygon(hdc, triangle, 3);

		SelectObject(hdc, hPenWindows);

		SelectObject(hdc, hBrushWindows);

		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 2; j++) {
				Rectangle(hdc, 10 + (15 * i) + x, 45 + (20 * j) + y,
						15 + (15 * i) + x, 55 + (20 * j) + y);
			}
		}

		Rectangle(hdc, 45 + x, 75 + y, 50 + x, 80 + y);
		Rectangle(hdc, 60 + x, 20 + y, 65 + x, 25 + y);

		SelectObject(hdc, hOldBrush);
		DeleteObject(hBrushBuild);
	}

	
	SelectObject(hdc, hOldPen);

	DeleteObject(hPenWindows);
	DeleteObject(hBrushWindows);
	DeleteObject(hPenBuild);	
}

void RandomCities(const int numberCities) {

	int randomNumber = rand() % 13;

	while (FindNutUsingPosition(&randomNumber, &numberCities)) {
		randomNumber = rand() % 13;
	};

	if (randomNumber < 7) {
		cities[numberCities].yBlock = randomNumber;
		cities[numberCities].xBlock = 0;

		map[0][randomNumber].horizontal = 2;
		map[0][randomNumber].isRoad = TRUE;
	}
	else if (randomNumber >= 7) {
		cities[numberCities].yBlock = randomNumber - 7;
		cities[numberCities].xBlock = 13;

		map[13][randomNumber - 7].horizontal = 2;
		map[13][randomNumber - 7].isRoad = TRUE;
	};

	randomNumber = rand() % 13;

	while (FindColor(&randomNumber, &numberCities)) {
		randomNumber = rand() % 13;
	}

	cities[numberCities].Color = BankOfColors[randomNumber];
}

BYTE FindColor(const int* ColorNumber, const int* numberCities) {

	BYTE result = FALSE;

	for (int i = 0; i < *numberCities; i++) {
		if (BankOfColors[*ColorNumber] == cities[i].Color) {
			result = TRUE;
		}
	}

	return result;
}

BYTE FindNutUsingPosition(const int* randNumber, const int* numberCities) {
	
	BYTE result = FALSE;

	if (*randNumber < 7) {
		for (int i = 0; i < *numberCities; i++) {
			if (cities[i].yBlock == *randNumber && cities[i].xBlock == 0) {
				result = TRUE;
			}
		}
	}
	else if (*randNumber >= 7) {
		for (int i = 0; i <= *numberCities; i++) {
			if (cities[i].yBlock == *randNumber && cities[i].xBlock == 13) {
				result = TRUE;
			}
		}
	}

	return result;
}