#include <windows.h>
#include <time.h>
#include "DrawingRails.h"
#include "TrainMove.h"
#include "Structures.h"
#include "HelpNewCity.h"
#include "HelpForMain.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void DrawTrain(HWND*, HDC*, PAINTSTRUCT*, RECT*);
void DrawingRailWays(HWND, HDC, PAINTSTRUCT);
void CityDrawing(HDC*, int*);
void NewCity(const int);
void NewTrain(int*, int*);
void DrawingLabelFinishedTrains(HDC*);

const COLORREF BankOfColors[] = { 0x300055, 0x9999FF, 0x8000FF, 0x00554c,
								0x00AA99, 0x004c99, 0xCC0000, 0x99A000,
								0x660000, 0xFF3399, 0xFF9933, 0x999900,
								0x4cBB00, 0xAA5599 };

const int TIMERID = 61; // time when game clock change
const int NEWTRAINTIMER = 700; // time when new train is creaiting
const int NEWCITYTIMER = 2600; // time when new city is creating

int timer = 501;
int trainsOnTheMap = -1;
int citiesOnTheMap = -1;
int finishedTrains = 0;
int rightButton, leftButton;

Road map[14][10];
NewRoad newRoadBlock;
City cities[14];
Train trains[20];

RECT trainsRedraw[20];
POINT mouse, mousePosition, mapBlock;

HWND hLaFinishedTrains;

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
	POINT nextMouse;
	BYTE trainInTheBlockResult;

	int foundTrain = -1;

	switch (msg)
	{
	case WM_CREATE:
		RestartTimer(&hwnd, &TIMERID);
		rightButton = -1;
		leftButton = -1;
		citiesOnTheMap = 1;

		hLaFinishedTrains = CreateWindowW(L"Static", L"No Finished Trains Yet", WS_CHILD | WS_VISIBLE, 650, 5, 750, 30, hwnd, (HMENU)1, NULL, NULL);

		NewCity(0);
		NewCity(1);

		break;

	case WM_TIMER:

		//redraw the Trains
		if (wParam == TIMERID) {
			timer++;
			RestartTimer(&hwnd, &TIMERID);

			if (trainsOnTheMap != -1) {
				for (int i = 0; i <= trainsOnTheMap; i++) {
					InvalidateRect(hwnd, &trainsRedraw[i], TRUE);
				}
			}
			
		}
		//adding new Train
		if (timer % NEWTRAINTIMER == 0) {
			
			if (trainsOnTheMap < 20) {

				trainsOnTheMap++;

				NewTrain(&trainsOnTheMap, &citiesOnTheMap);
			}
			
		}
		//creating new City
		if (timer % NEWCITYTIMER == 0) {

			if (citiesOnTheMap < 13) {
				citiesOnTheMap++;

				NewCity(citiesOnTheMap);
			}

		}
		if (timer % (NEWCITYTIMER + 1) == 0) {

			if (citiesOnTheMap < 13) {

				redrawingRect.left = cities[citiesOnTheMap].block.x * 100;
				redrawingRect.top = cities[citiesOnTheMap].block.y * 100;
				redrawingRect.right = cities[citiesOnTheMap].block.x * 100 + 100;
				redrawingRect.bottom = cities[citiesOnTheMap].block.y * 100 + 100;

				InvalidateRect(hwnd, &redrawingRect, TRUE);
			}

		}

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		DrawingRailWays(hwnd, hdc, ps);
		DrawTrain(&hwnd, &hdc, &ps, trainsRedraw);
		CityDrawing(&hdc, &citiesOnTheMap);
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
				newRoadBlock = ResetNewRoad(newRoadBlock);
				newRoadBlock.road.horizontal = 1;
				newRoadBlock.road.isRoad = TRUE;
				break;
			case 1:
				newRoadBlock = ResetNewRoad(newRoadBlock);
				newRoadBlock.road.vertical = 1;
				newRoadBlock.road.isRoad = TRUE;

				break;
			case 2:
				newRoadBlock = ResetNewRoad(newRoadBlock);
				newRoadBlock.road.leftBottom = 1;
				newRoadBlock.road.isRoad = TRUE;

				break;
			case 3:
				newRoadBlock = ResetNewRoad(newRoadBlock);
				newRoadBlock.road.bottomRight = 1;
				newRoadBlock.road.isRoad = TRUE;

				break;
			case 4:
				newRoadBlock = ResetNewRoad(newRoadBlock);
				newRoadBlock.road.topRight = 1;
				newRoadBlock.road.isRoad = TRUE;

				break;
			case 5:
				newRoadBlock = ResetNewRoad(newRoadBlock);
				newRoadBlock.road.leftTop = 1;
				newRoadBlock.road.isRoad = TRUE;

				break;
			case -1:
				newRoadBlock = ResetNewRoad(newRoadBlock);
				newRoadBlock.road.isRoad = FALSE;

				break;
			default:
				break;
			}

			newRoadBlock.block.x = mouse.x;
			newRoadBlock.block.y = mouse.y;

			redrawingRect.left = mouse.x * 100;
			redrawingRect.top = mouse.y * 100;
			redrawingRect.right = (mouse.x * 100) + 100;
			redrawingRect.bottom = (mouse.y * 100) + 100;

			InvalidateRect(hwnd, &redrawingRect, TRUE);
		}
		break;
	case WM_LBUTTONDOWN:
		mousePosition.x = LOWORD(lParam);
		mousePosition.y = HIWORD(lParam);
		mapBlock.x = mousePosition.x / 100;
		mapBlock.y = mousePosition.y / 100;

		if (rightButton != -1 && !TrainInTheBlock(trains, mapBlock, &trainsOnTheMap, &foundTrain)) {
			
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

			newRoadBlock = ResetNewRoad(newRoadBlock);
			mouse.x = -1;
			mouse.y = -1;
		}
		else {
			if (leftButton == 3) {
				leftButton = -1;
			}
			else {
				leftButton++;
			}
		}
		
		break;
	case WM_LBUTTONUP:

		trainInTheBlockResult = TrainInTheBlock(trains, mapBlock, &trainsOnTheMap, &foundTrain);

		if (map[mapBlock.x][mapBlock.y].isRoad && !trainInTheBlockResult) {
			switch (MousePosition(mousePosition, mapBlock))
			{
			case 1: //left
				if (map[mapBlock.x][mapBlock.y].horizontal == 2) {
					if (map[mapBlock.x][mapBlock.y].leftBottom == 1) {
						map[mapBlock.x][mapBlock.y].horizontal = 1;
						map[mapBlock.x][mapBlock.y].leftBottom = 2;
					}
					else if (map[mapBlock.x][mapBlock.y].leftTop == 1) {
						map[mapBlock.x][mapBlock.y].horizontal = 1;
						map[mapBlock.x][mapBlock.y].leftTop = 2;
					}
				}
				else if (map[mapBlock.x][mapBlock.y].leftBottom == 2) {
					if (map[mapBlock.x][mapBlock.y].leftTop == 1) {
						map[mapBlock.x][mapBlock.y].leftBottom = 1;
						map[mapBlock.x][mapBlock.y].leftTop = 2;
						
					}
					else if (map[mapBlock.x][mapBlock.y].horizontal == 1) {
						map[mapBlock.x][mapBlock.y].leftBottom = 1;
						map[mapBlock.x][mapBlock.y].horizontal = 2;
						
					}
				}
				else if (map[mapBlock.x][mapBlock.y].leftTop == 2) {
					if (map[mapBlock.x][mapBlock.y].horizontal == 1) {
						map[mapBlock.x][mapBlock.y].leftTop = 1;
						map[mapBlock.x][mapBlock.y].horizontal = 2;
						
					}
					else if (map[mapBlock.x][mapBlock.y].leftBottom == 1) {
						map[mapBlock.x][mapBlock.y].leftTop = 1;
						map[mapBlock.x][mapBlock.y].leftBottom = 2;
						
					}
				}
				break;

			case 2: // bottom
				if (map[mapBlock.x][mapBlock.y].vertical == 2) {
					if (map[mapBlock.x][mapBlock.y].bottomRight == 1) {
						map[mapBlock.x][mapBlock.y].vertical = 1;
						map[mapBlock.x][mapBlock.y].bottomRight = 2;
					}
					else if (map[mapBlock.x][mapBlock.y].leftBottom == 1) {
						map[mapBlock.x][mapBlock.y].vertical = 1;
						map[mapBlock.x][mapBlock.y].leftBottom = 2;
					}
				}
				else if (map[mapBlock.x][mapBlock.y].bottomRight == 2) {
					if (map[mapBlock.x][mapBlock.y].leftBottom == 1) {
						map[mapBlock.x][mapBlock.y].bottomRight = 1;
						map[mapBlock.x][mapBlock.y].leftBottom = 2;
					}
					else if (map[mapBlock.x][mapBlock.y].vertical == 1) {
						map[mapBlock.x][mapBlock.y].bottomRight = 1;
						map[mapBlock.x][mapBlock.y].vertical = 2;
					}
				}
				else if (map[mapBlock.x][mapBlock.y].leftBottom == 2) {
					if (map[mapBlock.x][mapBlock.y].vertical == 1) {
						map[mapBlock.x][mapBlock.y].leftBottom = 1;
						map[mapBlock.x][mapBlock.y].vertical = 2;
					}
					else if (map[mapBlock.x][mapBlock.y].bottomRight == 1) {
						map[mapBlock.x][mapBlock.y].leftBottom = 1;
						map[mapBlock.x][mapBlock.y].bottomRight = 2;
					}
				}
				break;

			case 3: //rigth
				if (map[mapBlock.x][mapBlock.y].horizontal == 2) {
					if (map[mapBlock.x][mapBlock.y].bottomRight == 1) {
						map[mapBlock.x][mapBlock.y].horizontal = 1;
						map[mapBlock.x][mapBlock.y].bottomRight = 2;
					}
					else if (map[mapBlock.x][mapBlock.y].topRight == 1) {
						map[mapBlock.x][mapBlock.y].horizontal = 1;
						map[mapBlock.x][mapBlock.y].topRight = 2;
					}
				}
				else if (map[mapBlock.x][mapBlock.y].bottomRight == 2) {
					if (map[mapBlock.x][mapBlock.y].topRight == 1) {
						map[mapBlock.x][mapBlock.y].bottomRight = 1;
						map[mapBlock.x][mapBlock.y].topRight = 2;
					}
					else if (map[mapBlock.x][mapBlock.y].horizontal == 1) {
						map[mapBlock.x][mapBlock.y].bottomRight = 1;
						map[mapBlock.x][mapBlock.y].horizontal = 2;
					}
				}
				else if (map[mapBlock.x][mapBlock.y].topRight == 2) {
					if (map[mapBlock.x][mapBlock.y].horizontal == 1) {
						map[mapBlock.x][mapBlock.y].topRight = 1;
						map[mapBlock.x][mapBlock.y].horizontal = 2;
					}
					else if (map[mapBlock.x][mapBlock.y].bottomRight == 1) {
						map[mapBlock.x][mapBlock.y].topRight = 1;
						map[mapBlock.x][mapBlock.y].bottomRight = 2;
					}
				}
				break;

			case 4: //Top
				if (map[mapBlock.x][mapBlock.y].vertical == 2) {
					if (map[mapBlock.x][mapBlock.y].topRight == 1) {
						map[mapBlock.x][mapBlock.y].vertical = 1;
						map[mapBlock.x][mapBlock.y].topRight = 2;
					}
					else if (map[mapBlock.x][mapBlock.y].leftTop == 1) {
						map[mapBlock.x][mapBlock.y].vertical = 1;
						map[mapBlock.x][mapBlock.y].leftTop = 2;
					}
				}
				else if (map[mapBlock.x][mapBlock.y].topRight == 2) {
					if (map[mapBlock.x][mapBlock.y].leftTop == 1) {
						map[mapBlock.x][mapBlock.y].topRight = 1;
						map[mapBlock.x][mapBlock.y].leftTop = 2;
					}
					else if (map[mapBlock.x][mapBlock.y].vertical == 1) {
						map[mapBlock.x][mapBlock.y].topRight = 1;
						map[mapBlock.x][mapBlock.y].vertical = 2;
					}
				}
				else if (map[mapBlock.x][mapBlock.y].leftTop == 2) {
					if (map[mapBlock.x][mapBlock.y].vertical == 1) {
						map[mapBlock.x][mapBlock.y].leftTop = 1;
						map[mapBlock.x][mapBlock.y].vertical = 2;
					}
					else if (map[mapBlock.x][mapBlock.y].topRight == 1) {
						map[mapBlock.x][mapBlock.y].leftTop = 1;
						map[mapBlock.x][mapBlock.y].topRight = 2;
					}
				}
				break;

			default:
				break;
			} 

			redrawingRect.left = mapBlock.x * 100;
			redrawingRect.top = mapBlock.y * 100;
			redrawingRect.right = (mapBlock.x * 100) + 100;
			redrawingRect.bottom = (mapBlock.y * 100) + 100;

			InvalidateRect(hwnd, &redrawingRect, TRUE);

		}
		//stop the train
		else if (trainInTheBlockResult) {
			if ((mousePosition.x >= trains[foundTrain].tail.x && mousePosition.x <= trains[foundTrain].head.x &&		//moving right
				mousePosition.y >= trains[foundTrain].head.y - 10 && mousePosition.y <= trains[foundTrain].head.y + 10) ||
				(mousePosition.x >= trains[foundTrain].head.x && mousePosition.x <= trains[foundTrain].tail.x &&		//moving left
				mousePosition.y >= trains[foundTrain].head.y - 10 && mousePosition.y <= trains[foundTrain].head.y + 10) ||
				(mousePosition.x >= trains[foundTrain].head.x - 10 && mousePosition.x <= trains[foundTrain].head.x + 10 &&	//moving up
				mousePosition.y >= trains[foundTrain].head.y && mousePosition.y <= trains[foundTrain].tail.y) ||
				(mousePosition.x >= trains[foundTrain].head.x - 10 && mousePosition.x <= trains[foundTrain].head.x + 10 &&	//moving down
				mousePosition.y >= trains[foundTrain].tail.y && mousePosition.y <= trains[foundTrain].head.y)) {
				if (trains[foundTrain].MouseStop) {
					trains[foundTrain].MouseStop = FALSE;
				}
				else {
					trains[foundTrain].MouseStop = TRUE;
				}
			}
		}
		break;

	case WM_MOUSEMOVE:

		nextMouse.x = LOWORD(lParam) / 100;
		nextMouse.y = HIWORD(lParam) / 100;

		if (((mouse.x != nextMouse.x) || (mouse.y != nextMouse.y)) && (rightButton != -1)) {
			rightButton = -1;

			redrawingRect.left = mouse.x * 100;
			redrawingRect.top = mouse.y * 100;
			redrawingRect.right = (mouse.x * 100) + 100;
			redrawingRect.bottom = (mouse.y * 100) + 100;

			InvalidateRect(hwnd, &redrawingRect, TRUE);

			newRoadBlock = ResetNewRoad(newRoadBlock);
			mouse.x = -1;
			mouse.y = -1;
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	};

	return DefWindowProcW(hwnd, msg, wParam, lParam);
}

void DrawTrain(HWND* hwnd, HDC* hdc, PAINTSTRUCT* ps, RECT trainRedraw[20]) {

	BYTE movingLeft, movingRight, movingUp, movingDown;

	if (trainsOnTheMap != -1) {
		for (int i = 0; i <= trainsOnTheMap; i++) {

			if (!trains[i].MouseStop) {
				POINT drawTrainBlock = trains[i].block;
				POINT drawPreBlock = trains[i].preBlock;

				if (map[drawTrainBlock.x][drawTrainBlock.y].leftBottom == 2 && !trains[i].Stop) {
					if (drawTrainBlock.y - drawPreBlock.y == 0) {
						TurningTrainLeftBottom(&i, trains);

						trainRedraw[i].left = drawPreBlock.x * 100;
						trainRedraw[i].top = drawTrainBlock.y * 100;
						trainRedraw[i].right = drawTrainBlock.x * 100 + 100;
						trainRedraw[i].bottom = drawTrainBlock.y * 100 + 100 + 50;
					}
					else {
						TurningTrainBottomLeft(&i, trains);

						trainRedraw[i].left = drawTrainBlock.x * 100 - 20;
						trainRedraw[i].top = drawTrainBlock.y * 100;
						trainRedraw[i].right = drawTrainBlock.x * 100 + 100;
						trainRedraw[i].bottom = drawPreBlock.y * 100 + 100;
					}
				}

				if (map[drawTrainBlock.x][drawTrainBlock.y].bottomRight == 2 && !trains[i].Stop) {
					if (drawTrainBlock.y - drawPreBlock.y == 0) {
						TurningTrainRightBottom(&i, trains);

						trainRedraw[i].left = drawTrainBlock.x * 100;
						trainRedraw[i].top = drawTrainBlock.y * 100;
						trainRedraw[i].right = drawPreBlock.x * 100 + 100;
						trainRedraw[i].bottom = drawPreBlock.y * 100 + 100;
					}
					else {
						TurningTrainBottomRight(&i, trains);

						trainRedraw[i].left = drawTrainBlock.x * 100;
						trainRedraw[i].top = drawTrainBlock.y * 100;
						trainRedraw[i].right = drawTrainBlock.x * 100 + 100 + 20;
						trainRedraw[i].bottom = drawPreBlock.y * 100 + 100;
					}
				}

				if (map[drawTrainBlock.x][drawTrainBlock.y].topRight == 2 && !trains[i].Stop) {
					if (drawTrainBlock.x - drawPreBlock.x == 0) {
						TurningTrainTopRight(&i, trains);

						trainRedraw[i].left = drawPreBlock.x * 100;
						trainRedraw[i].top = drawPreBlock.y * 100;
						trainRedraw[i].right = drawTrainBlock.x * 100 + 100 + 20;
						trainRedraw[i].bottom = drawTrainBlock.y * 100 + 100;
					}
					else {
						TurningTrainRightTop(&i, trains);

						trainRedraw[i].left = drawTrainBlock.x * 100;
						trainRedraw[i].top = drawTrainBlock.y * 100 - 20;
						trainRedraw[i].right = drawPreBlock.x * 100 + 100;
						trainRedraw[i].bottom = drawPreBlock.y * 100 + 100;
					}
				}

				if (map[drawTrainBlock.x][drawTrainBlock.y].leftTop == 2 && !trains[i].Stop) {
					if (drawTrainBlock.y - drawPreBlock.y == 0) {
						TurningTrainLeftTop(&i, trains);

						trainRedraw[i].left = drawPreBlock.x * 100;
						trainRedraw[i].top = drawPreBlock.y * 100 - 20;
						trainRedraw[i].right = drawTrainBlock.x * 100 + 100;
						trainRedraw[i].bottom = drawTrainBlock.y * 100 + 100;
					}
					else
					{
						TurningTrainTopLeft(&i, trains);

						trainRedraw[i].left = drawPreBlock.x * 100 - 20;
						trainRedraw[i].top = drawPreBlock.y * 100;
						trainRedraw[i].right = drawTrainBlock.x * 100 + 100;
						trainRedraw[i].bottom = drawTrainBlock.y * 100 + 100;
					}
				}

				//Horizontal
				if ((map[drawTrainBlock.x][drawTrainBlock.y].horizontal == 2 && (drawTrainBlock.y - drawPreBlock.y == 0) && !trains[i].Stop)
					|| (map[drawTrainBlock.x][drawTrainBlock.y].horizontal == 1 && (drawTrainBlock.y - drawPreBlock.y == 0) &&
						map[drawTrainBlock.x][drawTrainBlock.y].vertical == 2) && !trains[i].Stop) {

					if (DirectMoving(&i, FALSE, trains)) {
						trainRedraw[i].left = trains[i].head.x - 30;
						trainRedraw[i].top = drawTrainBlock.y * 100;
						trainRedraw[i].right = trains[i].tail.x + 30;
						trainRedraw[i].bottom = drawTrainBlock.y * 100 + 100;
					}
					else {
						trainRedraw[i].left = trains[i].tail.x - 30;
						trainRedraw[i].top = drawTrainBlock.y * 100;
						trainRedraw[i].right = trains[i].head.x + 30;
						trainRedraw[i].bottom = drawTrainBlock.y * 100 + 100;
					}


				}

				//Vertical
				if ((map[drawTrainBlock.x][drawTrainBlock.y].vertical == 2 && (drawTrainBlock.x - drawPreBlock.x == 0) && !trains[i].Stop)
					|| (map[drawTrainBlock.x][drawTrainBlock.y].horizontal == 2 && (drawTrainBlock.x - drawPreBlock.x == 0) &&
						(map[drawTrainBlock.x][drawTrainBlock.y].vertical == 1) && !trains[i].Stop)) {

					if (DirectMoving(&i, TRUE, trains)) {
						trainRedraw[i].left = drawTrainBlock.x * 100;
						trainRedraw[i].top = trains[i].head.y - 30;
						trainRedraw[i].right = drawTrainBlock.x * 100 + 100;
						trainRedraw[i].bottom = trains[i].tail.y + 30;
					}
					else
					{
						trainRedraw[i].left = drawTrainBlock.x * 100;
						trainRedraw[i].top = trains[i].tail.y - 30;
						trainRedraw[i].right = drawTrainBlock.x * 100 + 100;
						trainRedraw[i].bottom = trains[i].head.y + 30;
					}
				}

				if (trains[i].Stop) {
					if (trains[i].head.x % 100 == 0) {
						//continue moving from left to next new any road in right block
						if ((trains[i].head.x / 100 == drawTrainBlock.x + 1) && ((map[drawTrainBlock.x + 1][drawTrainBlock.y].horizontal == 2) ||
							(map[drawTrainBlock.x + 1][drawTrainBlock.y].leftBottom == 2) || (map[drawTrainBlock.x + 1][drawTrainBlock.y].leftTop == 2)) ||
							(map[drawTrainBlock.x + 1][drawTrainBlock.y].horizontal == 1 && map[drawTrainBlock.x + 1][drawTrainBlock.y].vertical == 2)) {

							trains[i].Stop = FALSE;
						}
						//continue moving from right to next new any road in left block
						if ((trains[i].head.x / 100 == drawTrainBlock.x) && ((map[drawTrainBlock.x - 1][drawTrainBlock.y].horizontal == 2) ||
							(map[drawTrainBlock.x - 1][drawTrainBlock.y].topRight == 2) || (map[drawTrainBlock.x - 1][drawTrainBlock.y].bottomRight == 2)) ||
							(map[drawTrainBlock.x - 1][drawTrainBlock.y].horizontal == 1 && map[drawTrainBlock.x - 1][drawTrainBlock.y].vertical == 2)) {

							trains[i].Stop = FALSE;
						}
					}
					else if (trains[i].head.y % 100 == 0) {
						//continue moving from top to next new any road in bottom block
						if ((trains[i].head.y / 100 == drawTrainBlock.y + 1) && ((map[drawTrainBlock.x][drawTrainBlock.y + 1].vertical == 2) ||
							(map[drawTrainBlock.x][drawTrainBlock.y + 1].leftTop == 2) || (map[drawTrainBlock.x][drawTrainBlock.y + 1].topRight == 2)) ||
							(map[drawTrainBlock.x][drawTrainBlock.y + 1].horizontal == 2 && map[drawTrainBlock.x][drawTrainBlock.y + 1].vertical == 1)) {

							trains[i].Stop = FALSE;
						}
						//continue moving from bottom to next new any road in top block
						if ((trains[i].head.y / 100 == drawTrainBlock.y) && ((map[drawTrainBlock.x][drawTrainBlock.y - 1].vertical == 2) ||
							(map[drawTrainBlock.x][drawTrainBlock.y - 1].leftBottom == 2) || (map[drawTrainBlock.x][drawTrainBlock.y - 1].bottomRight == 2)) ||
							(map[drawTrainBlock.x][drawTrainBlock.y - 1].horizontal == 2 && map[drawTrainBlock.x][drawTrainBlock.y - 1].vertical == 1)) {

							trains[i].Stop = FALSE;
						}
					}
				}

				if (trains[i].head.x % 100 == 0) {

					movingLeft = ((map[(trains[i].head.x / 100) - 1][drawTrainBlock.y].isRoad) && (map[(trains[i].head.x / 100) - 1][drawTrainBlock.y].horizontal == 2 ||
						map[(trains[i].head.x / 100) - 1][drawTrainBlock.y].topRight == 2 || map[(trains[i].head.x / 100) - 1][drawTrainBlock.y].bottomRight == 2) ||
						(map[(trains[i].head.x / 100) - 1][drawTrainBlock.y].horizontal == 1 && map[trains[i].head.x - 1][drawTrainBlock.y].vertical == 2));

					movingRight = ((map[(trains[i].head.x / 100)][drawTrainBlock.y].isRoad) && (map[(trains[i].head.x / 100)][drawTrainBlock.y].horizontal == 2 ||
						map[(trains[i].head.x / 100)][drawTrainBlock.y].leftTop == 2 || map[(trains[i].head.x / 100)][drawTrainBlock.y].leftBottom == 2) ||
						(map[(trains[i].head.x / 100)][drawTrainBlock.y].horizontal == 1 && map[trains[i].head.x][drawTrainBlock.y].vertical == 2));

					if ((trains[i].head.x - trains[i].tail.x < 0) && movingLeft) {
						trains[i].preBlock.x = drawTrainBlock.x;
						trains[i].preBlock.y = drawTrainBlock.y;

						trains[i].block.x = (trains[i].head.x / 100) - 1;
					}
					else if ((trains[i].head.x - trains[i].tail.x > 0) && movingRight) {
						trains[i].preBlock.x = drawTrainBlock.x;
						trains[i].preBlock.y = drawTrainBlock.y;

						trains[i].block.x = trains[i].head.x / 100;
					}
					else if ((drawTrainBlock.x == 0 || drawTrainBlock.x == 13) && (cities[trains[i].Destination].block.x == drawTrainBlock.x) &&
						(cities[trains[i].Destination].block.y == drawTrainBlock.y)) {
						FinishTrain(&i, &trainsOnTheMap, trains);
						if (trains[i].head.x == -1 || trains[i].head.x == 1401) {
							finishedTrains++;
							DrawingLabelFinishedTrains(hdc);
						}
					}
					//if wrong city, need to turn arround the train
					else if ((drawTrainBlock.x == 0 || drawTrainBlock.x == 13) && (trains[i].head.x == 0 || trains[i].head.x == 1400) &&
						(cities[trains[i].Destination].block.x != drawTrainBlock.x || cities[trains[i].Destination].block.y != drawTrainBlock.y)) {
						drawTrainBlock = trains[i].head;
						trains[i].head = trains[i].tail;
						trains[i].tail = drawTrainBlock;
						trains[i].preBlock = trains[i].block;
					}
					//stop moving
					else if (!trains[i].Stop) {
						trains[i].Stop = TRUE;
					}
				}

				if (trains[i].head.y % 100 == 0) {

					movingUp = (map[drawTrainBlock.x][(trains[i].head.y / 100) - 1].isRoad && (map[drawTrainBlock.x][(trains[i].head.y / 100) - 1].vertical == 2 ||
						map[drawTrainBlock.x][(trains[i].head.y / 100) - 1].bottomRight == 2 || map[drawTrainBlock.x][(trains[i].head.y / 100) - 1].leftBottom == 2) ||
						(map[drawTrainBlock.x][(trains[i].head.y / 100) - 1].horizontal == 2 && map[drawTrainBlock.x][(trains[i].head.y / 100) - 1].vertical == 1));

					movingDown = (map[drawTrainBlock.x][(trains[i].head.y / 100)].isRoad && (map[drawTrainBlock.x][(trains[i].head.y / 100)].vertical == 2 ||
						map[drawTrainBlock.x][(trains[i].head.y / 100)].topRight == 2 || map[drawTrainBlock.x][(trains[i].head.y / 100)].leftTop == 2) ||
						(map[drawTrainBlock.x][(trains[i].head.y / 100)].horizontal == 2 && map[drawTrainBlock.x][(trains[i].head.y / 100)].vertical == 1));
					//up
					if ((trains[i].head.y - trains[i].tail.y < 0) && movingUp) {
						trains[i].preBlock.x = drawTrainBlock.x;
						trains[i].preBlock.y = drawTrainBlock.y;

						trains[i].block.y = (trains[i].head.y / 100) - 1;
					}
					//down
					else  if ((trains[i].head.y - trains[i].tail.y > 0) && movingDown) {
						trains[i].preBlock.x = drawTrainBlock.x;
						trains[i].preBlock.y = drawTrainBlock.y;

						trains[i].block.y = trains[i].head.y / 100;
					}
					//stop moving
					else if (!trains[i].Stop) {
						trains[i].Stop = TRUE;
					}

				}
			}

			if (trainsOnTheMap != -1) {

				LOGBRUSH brush;
				DWORD pen_style = PS_GEOMETRIC; //PS_SOLID | PS_GEOMETRIC | PS_JOIN_BEVEL;

				brush.lbStyle = BS_SOLID;
				brush.lbColor = trains[i].Color;
				brush.lbHatch = 0;

				HPEN hPen = ExtCreatePen(pen_style, 25, &brush, 0, NULL);

				HPEN hOldPen = SelectObject(*hdc, hPen);

				MoveToEx(*hdc, trains[i].head.x, trains[i].head.y, NULL);
				LineTo(*hdc, trains[i].tail.x, trains[i].tail.y);

				SelectObject(*hdc, hOldPen);

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
	
	//drawing unactive rails
	int type = 1;

	for (int i = 0; i < 14; i++) {
		for (int j = 0; j < 10; j++) {
			HelpForDrawingRoads(&hwnd, &hdc, &ps, map[i][j], &type, &i, &j);
		}
	}

	//drawing building rails
	HelpForDrawingRoads(&hwnd, &hdc, &ps, newRoadBlock.road, &type, &newRoadBlock.block.x, &newRoadBlock.block.y);

	//drawing active rails
	type = 2;

	for (int i = 0; i < 14; i++) {
		for (int j = 0; j < 10; j++) {
			HelpForDrawingRoads(&hwnd, &hdc, &ps, map[i][j], &type, &i, &j);
		}
	}
		
}

void CityDrawing(HDC* hdc, int* numberOfCities) {

	LOGBRUSH brush;
	DWORD pen_style = PS_SOLID | PS_GEOMETRIC | PS_JOIN_ROUND;

	brush.lbStyle = BS_SOLID;
	brush.lbColor = RGB(0, 0, 0);
	brush.lbHatch = 0;

	HPEN hPenBuild = ExtCreatePen(pen_style, 2, &brush, 0, NULL);

	brush.lbColor = RGB(252, 252, 25);

	HPEN hPenWindows = ExtCreatePen(pen_style, 1, &brush, 0, NULL);

	HPEN hOldPen = SelectObject(*hdc, hPenBuild);

	
	HBRUSH hBrushWindows = CreateSolidBrush(RGB(252, 252, 25));

	POINT triangle[3], coordinates;

	for (int i = 0; i <= *numberOfCities; i++) {

		HBRUSH hBrushBuild = CreateSolidBrush(cities[i].Color);

		SelectObject(*hdc, hPenBuild);
		HBRUSH hOldBrush = SelectObject(*hdc, hBrushBuild);
		
		coordinates.x = cities[i].block.x * 100;
		coordinates.y = cities[i].block.y * 100;

		triangle[0].x = 36 + coordinates.x;
		triangle[0].y = 70 + coordinates.y;

		triangle[1].x = 50 + coordinates.x;
		triangle[1].y = 60 + coordinates.y;

		triangle[2].x = 62 + coordinates.x;
		triangle[2].y = 70 + coordinates.y;

		Rectangle(*hdc, 15 + coordinates.x, 10 + coordinates.y, 45 + coordinates.x, 50 + coordinates.y);

		SelectObject(*hdc, hBrushWindows);
		SelectObject(*hdc, hPenWindows);

		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 2; j++) {
				Rectangle(*hdc, 20 + (15 * i) + coordinates.x, 15 + (20 * j) + coordinates.y,
						25 + (15 * i) + coordinates.x, 25 + (20 * j) + coordinates.y);
			}
		}

		SelectObject(*hdc, hBrushBuild);
		SelectObject(*hdc, hPenBuild);

		Rectangle(*hdc, 5 + coordinates.x, 40 + coordinates.y, 35 + coordinates.x, 80 + coordinates.y);
		Rectangle(*hdc, 40 + coordinates.x, 70 + coordinates.y, 60 + coordinates.x, 90 + coordinates.y);
		Rectangle(*hdc, 55 + coordinates.x, 15 + coordinates.y, 75 + coordinates.x, 35 + coordinates.y);

		Polygon(*hdc, triangle, 3);

		triangle[0].x = 51 + coordinates.x;
		triangle[0].y = 15 + coordinates.y;

		triangle[1].x = 65 + coordinates.x;
		triangle[1].y = 5 + coordinates.y;

		triangle[2].x = 77 + coordinates.x;
		triangle[2].y = 15 + coordinates.y;

		Polygon(*hdc, triangle, 3);

		SelectObject(*hdc, hPenWindows);

		SelectObject(*hdc, hBrushWindows);

		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 2; j++) {
				Rectangle(*hdc, 10 + (15 * i) + coordinates.x, 45 + (20 * j) + coordinates.y,
						15 + (15 * i) + coordinates.x, 55 + (20 * j) + coordinates.y);
			}
		}

		Rectangle(*hdc, 45 + coordinates.x, 75 + coordinates.y, 50 + coordinates.x, 80 + coordinates.y);
		Rectangle(*hdc, 60 + coordinates.x, 20 + coordinates.y, 65 + coordinates.x, 25 + coordinates.y);

		SelectObject(*hdc, hOldBrush);
		DeleteObject(hBrushBuild);
	}

	
	SelectObject(*hdc, hOldPen);

	DeleteObject(hPenWindows);
	DeleteObject(hBrushWindows);
	DeleteObject(hPenBuild);	
}

void NewCity(const int numberCities) {

 	int randomNumber = rand() % 13;

	while (FindNotUsingPosition(&randomNumber, &numberCities, cities)) {
		randomNumber = rand() % 13;
	};

	//drawing in the left side
	if (randomNumber < 7) {
		cities[numberCities].block.y = randomNumber;
		cities[numberCities].block.x = 0;

		map[0][randomNumber].horizontal = 2;
		map[0][randomNumber].isRoad = TRUE;
	}
	//drawing in the right side
	else if (randomNumber >= 7) {
		cities[numberCities].block.y = randomNumber - 7;
		cities[numberCities].block.x = 13;

		map[13][randomNumber - 7].horizontal = 2;
		map[13][randomNumber - 7].isRoad = TRUE;
	};

	randomNumber = rand() % 13;

	while (FindColor(&randomNumber, &numberCities, &BankOfColors, cities)) {
		randomNumber = rand() % 13;
	}

	cities[numberCities].Color = BankOfColors[randomNumber];
}

void NewTrain(int* numberOfTrains, int* numberOfCities) {

	int citySource, cityDest;

	citySource = rand() % (* numberOfCities + 1);
	cityDest = rand() % (* numberOfCities + 1);

	while (citySource == cityDest) {
		cityDest = rand() % (* numberOfCities + 1);
	};

	//train starts from left side
	if (cities[citySource].block.x == 0) {
		trains[*numberOfTrains].head.x = cities[citySource].block.x * 100;
		trains[*numberOfTrains].head.y = cities[citySource].block.y * 100 + 50;
		trains[*numberOfTrains].tail.x = cities[citySource].block.x * 100 - 50;
		trains[*numberOfTrains].tail.y = cities[citySource].block.y * 100 + 50;
		trains[*numberOfTrains].block = cities[citySource].block;
		trains[*numberOfTrains].preBlock = cities[citySource].block;
		trains[*numberOfTrains].Destination = cityDest;
		trains[*numberOfTrains].Color = cities[cityDest].Color;
		trains[*numberOfTrains].Stop = FALSE;
		trains[*numberOfTrains].MouseStop = FALSE;
	}
	//train sarts from right side
	else if (cities[citySource].block.x == 13) {
		trains[*numberOfTrains].head.x = cities[citySource].block.x * 100 + 100 - 1;
		trains[*numberOfTrains].head.y = cities[citySource].block.y * 100 + 50;
		trains[*numberOfTrains].tail.x = cities[citySource].block.x * 100 + 100 + 50 - 1;
		trains[*numberOfTrains].tail.y = cities[citySource].block.y * 100 + 50;
		trains[*numberOfTrains].block = cities[citySource].block;
		trains[*numberOfTrains].preBlock.x = cities[citySource].block.x;// +1;
		trains[*numberOfTrains].preBlock.y = cities[citySource].block.y;
		trains[*numberOfTrains].Destination = cityDest;
		trains[*numberOfTrains].Color = cities[cityDest].Color;
		trains[*numberOfTrains].Stop = FALSE;
		trains[*numberOfTrains].MouseStop = FALSE;
	}
}

void DrawingLabelFinishedTrains(HDC* hdc) {

	wchar_t buf[21];

	wsprintfW(buf, L"Trains finished: %ld", finishedTrains);

	SetWindowTextW(hLaFinishedTrains, buf);
}