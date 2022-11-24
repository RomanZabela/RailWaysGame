#include "Structures.h"
#include "Timer.h"
#include "CreatNewTrainAndMove.h"
#include "DrawingRails.h"
#include "CreateCityAndDrawing.h"
#include "MousePosition.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void RestartTimer(HWND*, const int*);

const COLORREF BankOfColors[] = { 0x300055, 0x9999FF, 0x8000FF, 0x00554c,
								0x00AA99, 0x004c99, 0xCC0000, 0x99A000,
								0x660066, 0xFF3399, 0xFF9933, 0x995500,
								0x4cBB00, 0x70F0A2 };

const int MIN_TIMER_FOR_REDRAW_OBJECTS = 61;
const int CREATE_NEW_TRAIN_TIMER = 500;
const int CREATE_NEW_CITY_TIMER = 1800;

int timer = 301;
int amountTrainsOnTheMap = -1;
int amountCitiesOnTheMap = -1;
int finishedTrains = 0;
int rightButton, leftButton;

Road map[CLIENT_AREA_X][CLIENT_AREA_Y]; // client zone
NewRoad newRoadBlock;
City cities[MAX_CITIES_ON_THE_AREA];
Train trains[20];

RECT trainsRedraw[20];
POINT mouse, mousePosition, mapBlock;

HWND hLaFinishedTrains;

int WINAPI wWinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PWSTR lpCmdLine, _In_ int nCmdShow) {
	
	MSG msg;
	WNDCLASS wc = { 0 };

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpszClassName = L"RailWays";
	wc.hInstance = hInstance;
	wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
	wc.lpfnWndProc = WndProc;
	wc.hCursor = LoadCursor(0, IDC_ARROW);

	RegisterClassW(&wc);
	CreateWindowW(wc.lpszClassName, L"RailWays", WS_SYSMENU | WS_MINIMIZEBOX| WS_VISIBLE, 200, 25, 1400, 980, NULL, NULL, hInstance, NULL);

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
	RECT redrawingRect = {0};
	POINT nextMouse = {0};
	BYTE foundTrainInTheBlock;

	int foundTrain = -1;

	switch (msg)
	{
	case WM_CREATE:
		RestartTimer(&hwnd, &MIN_TIMER_FOR_REDRAW_OBJECTS);
		rightButton = -1;
		leftButton = -1;
		amountCitiesOnTheMap = 1;

		hLaFinishedTrains = CreateWindowW(L"Static", L"No Finished Trains Yet", WS_CHILD | WS_VISIBLE, 650, 5, 750, 30, hwnd, (HMENU)1, NULL, NULL);

		NewCity(0, cities, map, BankOfColors);
		NewCity(1, cities, map, BankOfColors);

		break;

	case WM_TIMER:

		//redraw the Trains
		if (wParam == MIN_TIMER_FOR_REDRAW_OBJECTS) {
			timer++;
			RestartTimer(&hwnd, &MIN_TIMER_FOR_REDRAW_OBJECTS);

			if (amountTrainsOnTheMap != -1) {
				for (int i = 0; i <= amountTrainsOnTheMap; i++) {
					InvalidateRect(hwnd, &trainsRedraw[i], TRUE);
				}
			}
			
		}

		//if (timer % 2) {
		//	if (trainsOnTheMap != -1) {
		//		for (int i = 0; i <= trainsOnTheMap; i++) {
		//			InvalidateRect(hwnd, &trainsRedraw[i], TRUE);
		//		}
		//	}
		//}
		
		//adding new Train

		if (timer % CREATE_NEW_TRAIN_TIMER == 0) {
			
			if (amountTrainsOnTheMap < 20) {

				amountTrainsOnTheMap++;

				AddNewTrain(&amountTrainsOnTheMap, &amountCitiesOnTheMap, cities, trains);
			}
			
		}
		//creating new City
		if (timer % CREATE_NEW_CITY_TIMER == 0) {

			if (amountCitiesOnTheMap <= 13) {
				amountCitiesOnTheMap++;

				NewCity(amountCitiesOnTheMap, cities, map, BankOfColors);
			}

		}
		if (timer % (CREATE_NEW_CITY_TIMER + 1) == 0) {

			if (amountCitiesOnTheMap <= 13) {

				redrawingRect.left = cities[amountCitiesOnTheMap].block.x * 100;
				redrawingRect.top = cities[amountCitiesOnTheMap].block.y * 100 + 30;
				redrawingRect.right = cities[amountCitiesOnTheMap].block.x * 100 + 100;
				redrawingRect.bottom = cities[amountCitiesOnTheMap].block.y * 100 + 100 + 30;

				InvalidateRect(hwnd, &redrawingRect, TRUE);
			}
		}

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		DrawingRailWays(&hdc, &ps, map, &newRoadBlock);
		DrawTrains(&hLaFinishedTrains, &hdc, &ps, trainsRedraw, &amountTrainsOnTheMap, trains, map, cities, &finishedTrains);
		CityDrawing(&hdc, &amountCitiesOnTheMap, cities);
		EndPaint(hwnd, &ps);
		break;
	case WM_RBUTTONDOWN:

		mouse.x = LOWORD(lParam) / 100;
		mouse.y = (HIWORD(lParam) - 30) / 100;

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
				newRoadBlock = ResetNewRoad(&newRoadBlock);
				newRoadBlock.road.horizontal = 1;
				newRoadBlock.road.isRoad = TRUE;
				break;
			case 1:
				newRoadBlock = ResetNewRoad(&newRoadBlock);
				newRoadBlock.road.vertical = 1;
				newRoadBlock.road.isRoad = TRUE;

				break;
			case 2:
				newRoadBlock = ResetNewRoad(&newRoadBlock);
				newRoadBlock.road.leftBottom = 1;
				newRoadBlock.road.isRoad = TRUE;

				break;
			case 3:
				newRoadBlock = ResetNewRoad(&newRoadBlock);
				newRoadBlock.road.bottomRight = 1;
				newRoadBlock.road.isRoad = TRUE;

				break;
			case 4:
				newRoadBlock = ResetNewRoad(&newRoadBlock);
				newRoadBlock.road.topRight = 1;
				newRoadBlock.road.isRoad = TRUE;

				break;
			case 5:
				newRoadBlock = ResetNewRoad(&newRoadBlock);
				newRoadBlock.road.leftTop = 1;
				newRoadBlock.road.isRoad = TRUE;

				break;
			case -1:
				newRoadBlock = ResetNewRoad(&newRoadBlock);
				newRoadBlock.road.isRoad = FALSE;

				break;
			default:
				break;
			}

			newRoadBlock.block.x = mouse.x;
			newRoadBlock.block.y = mouse.y;

			redrawingRect.left = mouse.x * 100;
			redrawingRect.top = mouse.y * 100 + 30;
			redrawingRect.right = (mouse.x * 100) + 100;
			redrawingRect.bottom = (mouse.y * 100) + 100 + 30;

			InvalidateRect(hwnd, &redrawingRect, TRUE);
		}
		break;
	case WM_LBUTTONDOWN:
		mousePosition.x = LOWORD(lParam);
		mousePosition.y = HIWORD(lParam) - 30;
		mapBlock.x = mousePosition.x / 100;
		mapBlock.y = mousePosition.y / 100;

		if (rightButton != -1 && !IsAnyTrainInTheBlock(trains, mapBlock, &amountTrainsOnTheMap, &foundTrain)) {
			
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
				redrawingRect.top = mouse.y * 100 + 30;
				redrawingRect.right = (mouse.x * 100) + 100;
				redrawingRect.bottom = (mouse.y * 100) + 100 + 30;

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

			newRoadBlock = ResetNewRoad(&newRoadBlock);
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

		foundTrainInTheBlock = IsAnyTrainInTheBlock(trains, mapBlock, &amountTrainsOnTheMap, &foundTrain);

		if (map[mapBlock.x][mapBlock.y].isRoad && !foundTrainInTheBlock) {
			switch (MousePositionBlock(mousePosition, mapBlock))
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
			redrawingRect.top = mapBlock.y * 100 + 30;
			redrawingRect.right = (mapBlock.x * 100) + 100;
			redrawingRect.bottom = (mapBlock.y * 100) + 100 + 30;

			InvalidateRect(hwnd, &redrawingRect, TRUE);

		}
		//stop the train
		else if (foundTrainInTheBlock) {
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
		nextMouse.y = (HIWORD(lParam) - 30) / 100;

		if (((mouse.x != nextMouse.x) || (mouse.y != nextMouse.y)) && (rightButton != -1)) {
			rightButton = -1;

			redrawingRect.left = mouse.x * 100;
			redrawingRect.top = mouse.y * 100 + 30;
			redrawingRect.right = (mouse.x * 100) + 100;
			redrawingRect.bottom = (mouse.y * 100) + 100 + 30;

			InvalidateRect(hwnd, &redrawingRect, TRUE);

			newRoadBlock = ResetNewRoad(&newRoadBlock);
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

void RestartTimer(HWND* hwnd, const int* TimerID) {
	SetTimer(*hwnd, *TimerID, 60, NULL);
}