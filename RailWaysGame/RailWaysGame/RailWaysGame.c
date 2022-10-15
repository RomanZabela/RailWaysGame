#include <windows.h>
#include <time.h>
#include "DrawingRails.h"
#include "TrainMove.h"
#include "MyStructures.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void DrawTrain(HWND, HDC, PAINTSTRUCT, RECT*);
void RestartTimer(HWND);
void DrawingRailWays(HWND, HDC, PAINTSTRUCT);
void CityDrawing(HWND, HDC, PAINTSTRUCT, int*);
void NewCity(const int);
void NewTrain(int*, int*);
void ResetNewRoad();
BYTE FindNutUsingPosition(const int*, const int*);
BYTE FindColor(const int*, const int*);

const COLORREF BankOfColors[] = {0x000099, 0x9999FF, 0x8000FF, 0x00994c,
								0x009999, 0x004c99, 0xCC0000, 0x994c00,
								0x660000, 0xFF3399, 0xFF9933, 0x999900,
								0x4c9900, 0x990099};

const int TimerID = 51;
const int newTrainTimer = 700;
const int newCityTimer = 3600;
int timer = 501;
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
struct NewRoad newRoadBlock;

struct City cities[14];

struct Train trains[20];

RECT trainsRedraw[20];

int rightButton, leftButton;
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

	RECT redrawingRect;

	switch (msg)
	{
	case WM_CREATE:
		RestartTimer(hwnd);
		rightButton = -1;
		leftButton = -1;
		citiesOnTheMap = 1;

		NewCity(0);
		NewCity(1);

		break;

	case WM_TIMER:
		if (wParam == TimerID) {
			timer++;
			RestartTimer(hwnd);

			if (trainsOnTheMap != -1) {
				for (int i = 0; i <= trainsOnTheMap; i++) {
					InvalidateRect(hwnd, &trainsRedraw[i], TRUE);
				}
			}
			
		}
		if (timer % newTrainTimer == 0) {
			
			if (trainsOnTheMap < 20) {

				trainsOnTheMap++;

				NewTrain(&trainsOnTheMap, &citiesOnTheMap);
			}
			
		}
		if (timer % newCityTimer == 0) {

			if (citiesOnTheMap < 13) {
				citiesOnTheMap++;

				NewCity(citiesOnTheMap);
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
		DrawTrain(hwnd, hdc, ps, trainsRedraw);
		CityDrawing(hwnd, hdc, ps, &citiesOnTheMap);
		EndPaint(hwnd, &ps);
		break;
	case WM_RBUTTONDOWN:

		mouse.x = LOWORD(lParam) / 100;
		mouse.y = HIWORD(lParam) / 100;

		if (rightButton == 5) {
			rightButton = -1;
		}
		else {
			rightButton++;
		}
		break;
	case WM_RBUTTONUP:	

		if (mouse.x != 0 && mouse.x != 13) {

			if (map[mouse.x][mouse.y].isRoad) {
				if (rightButton == 0 && map[mouse.x][mouse.y].horizontal != 0) {
					rightButton++;
				}
				if (rightButton == 1 && map[mouse.x][mouse.y].vertical != 0) {
					rightButton++;
				}
				if (rightButton == 2 && map[mouse.x][mouse.y].leftBottom != 0) {
					rightButton++;
				}
				if (rightButton == 3 && map[mouse.x][mouse.y].bottomRight != 0) {
					rightButton++;
				}
				if (rightButton == 4 && map[mouse.x][mouse.y].topRight != 0) {
					rightButton++;
				}
				if (rightButton == 5 && map[mouse.x][mouse.y].leftTop != 0) {
					rightButton = -1;
				}
			}

			switch (rightButton)
			{
			case 0:
				newRoadBlock.road.horizontal = 1;
				newRoadBlock.road.isRoad = TRUE;
				break;
			case 1:

				newRoadBlock.road.horizontal = 0;
				newRoadBlock.road.vertical = 1;
				newRoadBlock.road.isRoad = TRUE;

				break;
			case 2:

				newRoadBlock.road.vertical = 0;
				newRoadBlock.road.leftBottom = 1;
				newRoadBlock.road.isRoad = TRUE;

				break;
			case 3:

				newRoadBlock.road.leftBottom = 0;
				newRoadBlock.road.bottomRight = 1;
				newRoadBlock.road.isRoad = TRUE;

				break;
			case 4:

				newRoadBlock.road.bottomRight = 0;
				newRoadBlock.road.topRight = 1;
				newRoadBlock.road.isRoad = TRUE;

				break;
			case 5:

				newRoadBlock.road.topRight = 0;
				newRoadBlock.road.leftTop = 1;
				newRoadBlock.road.isRoad = TRUE;

				break;
			case -1:

				newRoadBlock.road.leftTop = 0;
				newRoadBlock.road.isRoad = FALSE;

				break;
			default:
				break;
			}

			newRoadBlock.xBlock = mouse.x;
			newRoadBlock.yBlock = mouse.y;

			redrawingRect.left = mouse.x * 100;
			redrawingRect.top = mouse.y * 100;
			redrawingRect.right = (mouse.x * 100) + 100;
			redrawingRect.bottom = (mouse.y * 100) + 100;

			InvalidateRect(hwnd, &redrawingRect, TRUE);
		}
		break;
	case WM_LBUTTONDOWN:

		if (rightButton != -1) {
			rightButton = -1;

			if (map[mouse.x][mouse.y].isRoad) {
				if (newRoadBlock.road.horizontal == 1 && newRoadBlock.road.horizontal != 2) {
					map[mouse.x][mouse.y].horizontal = 1;
				};
				if (newRoadBlock.road.vertical == 1 && newRoadBlock.road.vertical != 2) {
					map[mouse.x][mouse.y].vertical = 1;
				};
				if (newRoadBlock.road.bottomRight == 1 && newRoadBlock.road.bottomRight != 2) {
					map[mouse.x][mouse.y].bottomRight = 1;
				};
				if (newRoadBlock.road.leftBottom == 1 && newRoadBlock.road.leftBottom != 2) {
					map[mouse.x][mouse.y].leftBottom = 1;
				};
				if (newRoadBlock.road.leftTop == 1 && newRoadBlock.road.leftTop != 2) {
					map[mouse.x][mouse.y].leftTop = 1;
				};
				if (newRoadBlock.road.topRight == 1 && newRoadBlock.road.topRight != 2) {
					map[mouse.x][mouse.y].topRight = 1;
				};

				redrawingRect.left = mouse.x * 100;
				redrawingRect.top = mouse.y * 100;
				redrawingRect.right = (mouse.x * 100) + 100;
				redrawingRect.bottom = (mouse.y * 100) + 100;

				InvalidateRect(hwnd, &redrawingRect, TRUE);

			}
			else if (!map[mouse.x][mouse.y].isRoad) {
				if (newRoadBlock.road.horizontal == 1) {
					map[mouse.x][mouse.y].horizontal = 2;
					map[mouse.x][mouse.y].isRoad = TRUE;
				};
				if (newRoadBlock.road.vertical == 1) {
					map[mouse.x][mouse.y].vertical = 2;
					map[mouse.x][mouse.y].isRoad = TRUE;
				};
				if (newRoadBlock.road.bottomRight == 1) {
					map[mouse.x][mouse.y].bottomRight = 2;
					map[mouse.x][mouse.y].isRoad = TRUE;
				};
				if (newRoadBlock.road.leftBottom == 1) {
					map[mouse.x][mouse.y].leftBottom = 2;
					map[mouse.x][mouse.y].isRoad = TRUE;
				};
				if (newRoadBlock.road.leftTop == 1) {
					map[mouse.x][mouse.y].leftTop = 2;
					map[mouse.x][mouse.y].isRoad = TRUE;
				};
				if (newRoadBlock.road.topRight == 1) {
					map[mouse.x][mouse.y].topRight = 2;
					map[mouse.x][mouse.y].isRoad = TRUE;
				};
			}

			ResetNewRoad();
		}
		else {
			if (leftButton == 3) {
				leftButton = -1
			}
			else {
				leftButton++;
			}
		}
		
		break;
	case WM_LBUTTONUP:

		break;
	case WM_MOUSEMOVE:

		nextMouse.x = LOWORD(lParam) / 100;
		nextMouse.y = HIWORD(lParam) / 100;

		if ((mouse.x != nextMouse.x) && (mouse.y != nextMouse.y) && (rightButton != -1)) {
			rightButton = -1;

			redrawingRect.left = mouse.x * 100;
			redrawingRect.top = mouse.y * 100;
			redrawingRect.right = (mouse.x * 100) + 100;
			redrawingRect.bottom = (mouse.y * 100) + 100;

			InvalidateRect(hwnd, &redrawingRect, TRUE);

			ResetNewRoad();
		}
			

		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	};

	return DefWindowProcW(hwnd, msg, wParam, lParam);
}

void ResetNewRoad() {
	newRoadBlock.road.horizontal = 0;
	newRoadBlock.road.vertical = 0;
	newRoadBlock.road.bottomRight = 0;
	newRoadBlock.road.leftBottom = 0;
	newRoadBlock.road.topRight = 0;
	newRoadBlock.road.leftTop = 0;
	mouse.x = -1;
	mouse.y = -1;
}

void RestartTimer(HWND hwnd) {
	SetTimer(hwnd, TimerID, 50, NULL);
}

void DrawTrain(HWND hwnd, HDC hdc, PAINTSTRUCT ps, RECT trainRedraw[20]) {

	RECT rect;

	BYTE movingLeft, movingRight, movingUp, movingDown;

	GetClientRect(hwnd, &rect);

	if (trainsOnTheMap != -1) {
		for (int i = 0; i <= trainsOnTheMap; i++) {

			int BlockX = trains[i].blockX;
			int BlockY = trains[i].blockY;
			int preBlockX = trains[i].preBlockX;
			int preBlockY = trains[i].preBlockY;

			//continue moving after train stop
			if (BlockX == -1) {
				if (trains[i].headX % 100 == 0) {
					if ((trains[i].headX / 100 == preBlockX + 1) && ((map[preBlockX + 1][preBlockY].horizontal != 0) ||
						(map[preBlockX + 1][preBlockY].leftBottom != 0) || (map[preBlockX + 1][preBlockY].leftTop != 0))) {
						trains[i].blockX = preBlockX + 1;
						trains[i].blockY = preBlockY;

						BlockX = preBlockX + 1;
						BlockY = preBlockY;
					}
					if ((trains[i].headX / 100 == preBlockX) && ((map[preBlockX - 1][preBlockY].horizontal != 0) ||
						(map[preBlockX - 1][preBlockY].topRight != 0) || (map[preBlockX - 1][preBlockY].bottomRight != 0))) {
						trains[i].blockX = preBlockX - 1;
						trains[i].blockY = preBlockY;

						BlockX = preBlockX - 1;
						BlockY = preBlockY;
					}
				}
				else if (trains[i].headY % 100 == 0) {
					if ((trains[i].headY / 100 == preBlockY + 1) && ((map[preBlockX][preBlockY + 1].vertical != 0) ||
						(map[preBlockX][preBlockY + 1].leftTop != 0) || (map[preBlockX][preBlockY + 1].topRight != 0))) {
						trains[i].blockX = preBlockX;
						trains[i].blockY = preBlockY + 1;

						BlockX = preBlockX;
						BlockY = preBlockY + 1;
					}

					if ((trains[i].headY / 100 == preBlockY) && ((map[preBlockX][preBlockY - 1].vertical != 0) ||
						(map[preBlockX][preBlockY - 1].leftBottom != 0) || (map[preBlockX][preBlockY - 1].bottomRight != 0))) {
						trains[i].blockX = preBlockX;
						trains[i].blockY = preBlockY - 1;

						BlockX = preBlockX;
						BlockY = preBlockY - 1;
					}
				}
			}
			
			if (map[BlockX][BlockY].leftBottom == 2) {
				if (BlockY - preBlockY == 0) {
					TurningTrainLeftBottom(&i, trains);

					trainRedraw[i].left = preBlockX * 100;
					trainRedraw[i].top = BlockY * 100;
					trainRedraw[i].right = BlockX * 100 + 100;
					trainRedraw[i].bottom = BlockY * 100 + 100 + 50;
				}
				else {
					TurningTrainBottomLeft(&i, trains);

					trainRedraw[i].left = BlockX * 100 - 20;
					trainRedraw[i].top = BlockY * 100;
					trainRedraw[i].right = BlockX * 100 + 100;
					trainRedraw[i].bottom = preBlockY * 100 + 100;
				}
			}

			if (map[BlockX][BlockY].bottomRight == 2) {
				if (BlockY - preBlockY == 0) {
					TurningTrainRightBottom(&i, trains);

					trainRedraw[i].left = BlockX * 100;
					trainRedraw[i].top = BlockY * 100;
					trainRedraw[i].right = preBlockX * 100 + 100;
					trainRedraw[i].bottom = preBlockY * 100 + 100;
				}
				else {
					TurningTrainBottomRight(&i, trains);

					trainRedraw[i].left = BlockX * 100;
					trainRedraw[i].top = BlockY * 100;
					trainRedraw[i].right = BlockX * 100 + 100 + 20;
					trainRedraw[i].bottom = preBlockY * 100 + 100;
				}
			}

			if (map[BlockX][BlockY].topRight == 2) {
				if (BlockX - preBlockX == 0) {
					TurningTrainTopRight(&i, trains);

					trainRedraw[i].left = preBlockX * 100;
					trainRedraw[i].top = preBlockY * 100;
					trainRedraw[i].right = BlockX * 100 + 100 + 20;
					trainRedraw[i].bottom = BlockY * 100 + 100;
				}
				else {
					TurningTrainRightTop(&i, trains);

					trainRedraw[i].left = BlockX * 100;
					trainRedraw[i].top = BlockY * 100 - 20;
					trainRedraw[i].right = preBlockX * 100 + 100;
					trainRedraw[i].bottom = preBlockY * 100 + 100;
				}
			}

			if (map[BlockX][BlockY].leftTop == 2) {
				if (BlockY - preBlockY == 0) {
					TurningTrainLeftTop(&i, trains);

					trainRedraw[i].left = preBlockX * 100;
					trainRedraw[i].top = preBlockY * 100 - 20;
					trainRedraw[i].right = BlockX * 100 + 100;
					trainRedraw[i].bottom = BlockY * 100 + 100;
				}
				else
				{
					TurningTrainTopLeft(&i, trains);

					trainRedraw[i].left = preBlockX * 100 - 20;
					trainRedraw[i].top = preBlockY * 100;
					trainRedraw[i].right = BlockX * 100 + 100;
					trainRedraw[i].bottom = BlockY * 100 + 100;
				}
			}

			//Horizontal
			if ((map[BlockX][BlockY].horizontal == 2 && (BlockY - preBlockY == 0)) ||
				(map[BlockX][BlockY].horizontal == 1 && (BlockY - preBlockY == 0) && map[BlockX][BlockY].vertical == 2)) {
				if (DirectMoving(&i, FALSE, trains)) {
					trainRedraw[i].left = trains[i].headX - 30;
					trainRedraw[i].top = BlockY * 100;
					trainRedraw[i].right = trains[i].tailX + 30;
					trainRedraw[i].bottom = BlockY * 100 + 100;
				}
				else {
					trainRedraw[i].left = trains[i].tailX - 30;
					trainRedraw[i].top = BlockY * 100;
					trainRedraw[i].right = trains[i].headX + 30;
					trainRedraw[i].bottom = BlockY * 100 + 100;
				}

				
			}

			//Vertical
			if ((map[BlockX][BlockY].vertical == 2 && (BlockX - preBlockX == 0)) ||
				(map[BlockX][BlockY].horizontal == 2 && (BlockX - preBlockX == 0) && (map[BlockX][BlockY].vertical == 1))) {
				if (DirectMoving(&i, TRUE, trains)) {
					trainRedraw[i].left = BlockX * 100;
					trainRedraw[i].top = trains[i].headY - 30;
					trainRedraw[i].right = BlockX * 100 + 100;
					trainRedraw[i].bottom = trains[i].tailY + 30;
				}
				else
				{
					trainRedraw[i].left = BlockX * 100;
					trainRedraw[i].top = trains[i].tailY - 30;
					trainRedraw[i].right = BlockX * 100 + 100;
					trainRedraw[i].bottom = trains[i].headY + 30;
				}
			}

			if (trains[i].headX % 100 == 0) {

				movingLeft = ((map[(trains[i].headX / 100) - 1][BlockY].isRoad) && (map[(trains[i].headX / 100) - 1][BlockY].horizontal != 0 ||
					map[(trains[i].headX / 100) - 1][BlockY].topRight != 0 || map[(trains[i].headX / 100) - 1][BlockY].bottomRight != 0));

				movingRight = ((map[(trains[i].headX / 100)][BlockY].isRoad) && (map[(trains[i].headX / 100)][BlockY].horizontal != 0 ||
					map[(trains[i].headX / 100)][BlockY].leftTop != 0 || map[(trains[i].headX / 100)][BlockY].leftBottom != 0));

				if ((BlockX == trains[i].headX / 100) && movingLeft) { 
					trains[i].preBlockX = BlockX;
					trains[i].preBlockY = BlockY;

					trains[i].blockX = (trains[i].headX / 100) - 1;
				}
				else if ((BlockX + 1 == (trains[i].headX / 100)) && movingRight) { 
					trains[i].preBlockX = BlockX;
					trains[i].preBlockY = BlockY;

					trains[i].blockX = trains[i].headX / 100;
				}
				else if ((BlockX == 0 || BlockX == 13) && (cities[trains[i].Destination].xBlock == BlockX) && (cities[trains[i].Destination].yBlock == BlockY)) {
					FinishTrain(&i, &trainsOnTheMap, trains);
				}
				//stop moving
				else if (BlockX != -1) {
					trains[i].preBlockX = BlockX;
					trains[i].preBlockY = BlockY;
					trains[i].blockX = -1;
					trains[i].blockY = -1;
				}
			}
			if (trains[i].headY % 100 == 0) {

				movingUp = (map[BlockX][(trains[i].headY / 100) - 1].isRoad && (map[BlockX][(trains[i].headY / 100) - 1].vertical != 0 ||
					map[BlockX][(trains[i].headY / 100) - 1].bottomRight != 0 || map[BlockX][(trains[i].headY / 100) - 1].leftBottom != 0));

				movingDown = (map[BlockX][(trains[i].headY / 100)].isRoad && (map[BlockX][(trains[i].headY / 100)].vertical != 0 ||
					map[BlockX][(trains[i].headY / 100)].topRight != 0 || map[BlockX][(trains[i].headY / 100)].leftTop != 0));

				if ((BlockY == trains[i].headY / 100) && movingUp) { 
					trains[i].preBlockX = BlockX;
					trains[i].preBlockY = BlockY;

					trains[i].blockY = (trains[i].headY / 100) - 1;
				}
				else  if ((BlockY + 1 == (trains[i].headY / 100)) && movingDown) { 
					trains[i].preBlockX = BlockX;
					trains[i].preBlockY = BlockY;

					trains[i].blockY = trains[i].headY / 100;
				}
				//stop moving
				else if (BlockY != -1) {
					trains[i].preBlockX = BlockX;
					trains[i].preBlockY = BlockY;
					trains[i].blockX = -1;
					trains[i].blockY = -1;
				}

			}

			if (trainsOnTheMap != -1) {

				LOGBRUSH brush;
				DWORD pen_style = PS_GEOMETRIC; //PS_SOLID | PS_GEOMETRIC | PS_JOIN_BEVEL;

				brush.lbStyle = BS_SOLID;
				brush.lbColor = trains[i].Color;
				brush.lbHatch = 0;

				HPEN hPen = ExtCreatePen(pen_style, 25, &brush, 0, NULL);

				HPEN hOldPen = SelectObject(hdc, hPen);

				MoveToEx(hdc, trains[i].headX, trains[i].headY, NULL); //530; 350
				LineTo(hdc, trains[i].tailX, trains[i].tailY);			//580; 350

				SelectObject(hdc, hOldPen);

				DeleteObject(hPen);
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
	
	//SelectObject(hdc, hOldBrush);
	//DeleteObject(hBrush);
}

void DrawingRailWays(HWND hwnd, HDC hdc, PAINTSTRUCT ps) {

	int type = 1;

	for (int i = 0; i < 14; i++) {
		for (int j = 0; j < 10; j++) {
			HelpForDrawingRoads(&hwnd, &hdc, &ps, map[i][j], &type, &i, &j);
		}
	}

	HelpForDrawingRoads(&hwnd, &hdc, &ps, newRoadBlock.road, &type, &newRoadBlock.xBlock, &newRoadBlock.yBlock);

	type = 2;

	for (int i = 0; i < 14; i++) {
		for (int j = 0; j < 10; j++) {
			HelpForDrawingRoads(&hwnd, &hdc, &ps, map[i][j], &type, &i, &j);
		}
	}
		
}

void CityDrawing(HWND hwnd, HDC hdc, PAINTSTRUCT ps, int* numberOfCities) {

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

void NewCity(const int numberCities) {

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

void NewTrain(int* numberOfTrains, int* numberOfCities) {

	int citySource, cityDist;

	citySource = rand() % (* numberOfCities + 1);
	cityDist = rand() % (* numberOfCities + 1);

	while (citySource == cityDist) {
		cityDist = rand() % (* numberOfCities + 1);
	};

	if (citySource < 7) {
		trains[*numberOfTrains].headX = cities[citySource].xBlock * 100;
		trains[*numberOfTrains].headY = cities[citySource].yBlock * 100 + 50;
		trains[*numberOfTrains].tailX = cities[citySource].xBlock * 100 - 50;
		trains[*numberOfTrains].tailY = cities[citySource].yBlock * 100 + 50;
		trains[*numberOfTrains].blockX = cities[citySource].xBlock;
		trains[*numberOfTrains].blockY = cities[citySource].yBlock;
		trains[*numberOfTrains].preBlockX = cities[citySource].xBlock;
		trains[*numberOfTrains].preBlockY = cities[citySource].yBlock;
		trains[*numberOfTrains].Destination = cityDist;
		trains[*numberOfTrains].Color = cities[cityDist].Color;
	}
	else if (citySource >= 7) {
		trains[*numberOfTrains].headX = cities[citySource].xBlock * 100 + 100;
		trains[*numberOfTrains].headY = cities[citySource].yBlock * 100 + 50;
		trains[*numberOfTrains].tailX = cities[citySource].xBlock * 100 + 100 + 50;
		trains[*numberOfTrains].tailY = cities[citySource].yBlock * 100 + 50;
		trains[*numberOfTrains].blockX = cities[citySource].xBlock;
		trains[*numberOfTrains].blockY = cities[citySource].yBlock;
		trains[*numberOfTrains].preBlockX = cities[citySource].xBlock;
		trains[*numberOfTrains].preBlockY = cities[citySource].yBlock;
		trains[*numberOfTrains].Destination = cityDist;
		trains[*numberOfTrains].Color = cities[cityDist].Color;
	}
}