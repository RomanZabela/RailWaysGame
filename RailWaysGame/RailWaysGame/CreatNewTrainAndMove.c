#include "Structures.h"
#include "CreatNewTrainAndMove.h"
#include "DrawingInterface.h"

//moving funcitions
BYTE DirectMoving(int*, byte, Train[]);
void TurningTrainLeftBottom(int*, Train[]);
void TurningTrainBottomLeft(int*, Train[]);
void TurningTrainTopRight(int*, Train[]);
void TurningTrainRightTop(int*, Train[]);
void TurningTrainLeftTop(int*, Train[]);
void TurningTrainTopLeft(int*, Train[]);
void TurningTrainRightBottom(int*, Train[]);
void TurningTrainBottomRight(int*, Train[]);

//delete the train from the map
void FinishTrain(int*, int*, Train[], RECT*);

//looking for the train on the current block
BYTE IsAnyTrainInTheBlock(Train[], POINT, const int*, int*);

//add new train
void AddNewTrain(int*, int*, City*, Train*, RECT*);

//help procedures for moving trains
void InizializeVariablesForTrainMovingCalculation(POINT*, POINT*, POINT*, Train*);
void AssignedVaiablesBackAfterCalculation(POINT*, POINT*, Train*);
void MovingRightOrTopBeforeTurning(int*, int*, int*);
void MovingLeftOrBottomBeforeTurning(int*, int*, int*);

void DrawTrains(HWND* hLaFinishedTrains, HDC* hdc, PAINTSTRUCT* ps, RECT* trainRedraw, int* amountTrainsOnTheMap,
	Train* trains, Road map[CLIENT_AREA_X][CLIENT_AREA_Y], City* cities, int* finishedTrains, int* Money) {

	BYTE movingLeft, movingRight, movingUp, movingDown;

	if (*amountTrainsOnTheMap != -1) {
		for (int i = 0; i <= *amountTrainsOnTheMap; i++) {

			if (!trains[i].MouseStop) {
				POINT drawTrainBlock = trains[i].block;
				POINT drawPreBlock = trains[i].preBlock;
				POINT tempTrainBlock;

				if (map[drawTrainBlock.x][drawTrainBlock.y].leftBottom == 2 && !trains[i].Stop) {
					if (drawTrainBlock.y - drawPreBlock.y == 0) {
						TurningTrainLeftBottom(&i, trains);

						trainRedraw[i].left = drawPreBlock.x * 100;
						trainRedraw[i].top = drawTrainBlock.y * 100 + 30;
						trainRedraw[i].right = drawTrainBlock.x * 100 + 100;
						trainRedraw[i].bottom = drawTrainBlock.y * 100 + 100 + 50 + 30;
					}
					else {
						TurningTrainBottomLeft(&i, trains);

						trainRedraw[i].left = drawTrainBlock.x * 100 - 20;
						trainRedraw[i].top = drawTrainBlock.y * 100 + 30;
						trainRedraw[i].right = drawTrainBlock.x * 100 + 100;
						trainRedraw[i].bottom = drawPreBlock.y * 100 + 100 + 30;
					}
				}

				if (map[drawTrainBlock.x][drawTrainBlock.y].bottomRight == 2 && !trains[i].Stop) {
					if (drawTrainBlock.y - drawPreBlock.y == 0) {
						TurningTrainRightBottom(&i, trains);

						trainRedraw[i].left = drawTrainBlock.x * 100;
						trainRedraw[i].top = drawTrainBlock.y * 100 + 30;
						trainRedraw[i].right = drawPreBlock.x * 100 + 100;
						trainRedraw[i].bottom = drawPreBlock.y * 100 + 100 + 15 + 30;
					}
					else {
						TurningTrainBottomRight(&i, trains);

						trainRedraw[i].left = drawTrainBlock.x * 100;
						trainRedraw[i].top = drawTrainBlock.y * 100 + 30;
						trainRedraw[i].right = drawTrainBlock.x * 100 + 100 + 20;
						trainRedraw[i].bottom = drawPreBlock.y * 100 + 100 + 30;
					}
				}

				if (map[drawTrainBlock.x][drawTrainBlock.y].topRight == 2 && !trains[i].Stop) {
					if (drawTrainBlock.x - drawPreBlock.x == 0) {
						TurningTrainTopRight(&i, trains);

						trainRedraw[i].left = drawPreBlock.x * 100;
						trainRedraw[i].top = drawPreBlock.y * 100 + 30;
						trainRedraw[i].right = drawTrainBlock.x * 100 + 100 + 20;
						trainRedraw[i].bottom = drawTrainBlock.y * 100 + 100 + 30;
					}
					else {
						TurningTrainRightTop(&i, trains);

						trainRedraw[i].left = drawTrainBlock.x * 100;
						trainRedraw[i].top = drawTrainBlock.y * 100 - 20 + 30;
						trainRedraw[i].right = drawPreBlock.x * 100 + 100;
						trainRedraw[i].bottom = drawPreBlock.y * 100 + 100 + 30;
					}
				}

				if (map[drawTrainBlock.x][drawTrainBlock.y].leftTop == 2 && !trains[i].Stop) {
					if (drawTrainBlock.y - drawPreBlock.y == 0) {
						TurningTrainLeftTop(&i, trains);

						trainRedraw[i].left = drawPreBlock.x * 100;
						trainRedraw[i].top = drawPreBlock.y * 100 - 20 + 30;
						trainRedraw[i].right = drawTrainBlock.x * 100 + 100;
						trainRedraw[i].bottom = drawTrainBlock.y * 100 + 100 + 30;
					}
					else
					{
						TurningTrainTopLeft(&i, trains);

						trainRedraw[i].left = drawPreBlock.x * 100 - 20;
						trainRedraw[i].top = drawPreBlock.y * 100 + 30;
						trainRedraw[i].right = drawTrainBlock.x * 100 + 100;
						trainRedraw[i].bottom = drawTrainBlock.y * 100 + 100 + 30;
					}
				}

				//Horizontal
				if ((map[drawTrainBlock.x][drawTrainBlock.y].horizontal == 2 && (drawTrainBlock.y - drawPreBlock.y == 0) && !trains[i].Stop)
					|| (map[drawTrainBlock.x][drawTrainBlock.y].horizontal == 1 && (drawTrainBlock.y - drawPreBlock.y == 0) &&
						map[drawTrainBlock.x][drawTrainBlock.y].vertical == 2) && !trains[i].Stop) {

					if (DirectMoving(&i, FALSE, trains)) {
						trainRedraw[i].left = trains[i].head.x - 30;
						trainRedraw[i].top = drawTrainBlock.y * 100 + 30;
						trainRedraw[i].right = trains[i].tail.x + 30;
						trainRedraw[i].bottom = drawTrainBlock.y * 100 + 100 + 30;
					}
					else {
						trainRedraw[i].left = trains[i].tail.x - 30;
						trainRedraw[i].top = drawTrainBlock.y * 100 + 30;
						trainRedraw[i].right = trains[i].head.x + 30;
						trainRedraw[i].bottom = drawTrainBlock.y * 100 + 100 + 30;
					}


				}

				//Vertical
				if ((map[drawTrainBlock.x][drawTrainBlock.y].vertical == 2 && (drawTrainBlock.x - drawPreBlock.x == 0) && !trains[i].Stop)
					|| (map[drawTrainBlock.x][drawTrainBlock.y].horizontal == 2 && (drawTrainBlock.x - drawPreBlock.x == 0) &&
						(map[drawTrainBlock.x][drawTrainBlock.y].vertical == 1) && !trains[i].Stop)) {

					if (DirectMoving(&i, TRUE, trains)) {
						trainRedraw[i].left = drawTrainBlock.x * 100;
						trainRedraw[i].top = trains[i].head.y - 30 + 30;
						trainRedraw[i].right = drawTrainBlock.x * 100 + 100;
						trainRedraw[i].bottom = trains[i].tail.y + 30 + 30;
					}
					else
					{
						trainRedraw[i].left = drawTrainBlock.x * 100;
						trainRedraw[i].top = trains[i].tail.y - 30 + 30;
						trainRedraw[i].right = drawTrainBlock.x * 100 + 100;
						trainRedraw[i].bottom = trains[i].head.y + 30 + 30;
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

					movingLeft = ((map[(trains[i].head.x / 100) - 1][drawTrainBlock.y].isRoad) && ((map[(trains[i].head.x / 100) - 1][drawTrainBlock.y].horizontal == 2 ||
						map[(trains[i].head.x / 100) - 1][drawTrainBlock.y].topRight == 2 || map[(trains[i].head.x / 100) - 1][drawTrainBlock.y].bottomRight == 2) ||
						(map[(trains[i].head.x / 100) - 1][drawTrainBlock.y].horizontal == 1 && map[(trains[i].head.x / 100) - 1][drawTrainBlock.y].vertical == 2)));

					movingRight = ((map[(trains[i].head.x / 100)][drawTrainBlock.y].isRoad) && ((map[(trains[i].head.x / 100)][drawTrainBlock.y].horizontal == 2 ||
						map[(trains[i].head.x / 100)][drawTrainBlock.y].leftTop == 2 || map[(trains[i].head.x / 100)][drawTrainBlock.y].leftBottom == 2) ||
						(map[(trains[i].head.x / 100)][drawTrainBlock.y].horizontal == 1 && map[trains[i].head.x / 100][drawTrainBlock.y].vertical == 2)));

					if ((trains[i].head.x - trains[i].tail.x < 0) && movingLeft) {
						trains[i].preBlock.x = drawTrainBlock.x;
						trains[i].preBlock.y = drawTrainBlock.y;

						trains[i].block.x = (trains[i].head.x / 100) - 1;

						//(*Money) = (*Money) - 5;
					}
					else if ((trains[i].head.x - trains[i].tail.x > 0) && movingRight) {
						trains[i].preBlock.x = drawTrainBlock.x;
						trains[i].preBlock.y = drawTrainBlock.y;

						trains[i].block.x = trains[i].head.x / 100;

						//(*Money) = (*Money) - 5;
					}
					else if ((drawTrainBlock.x == 0 || drawTrainBlock.x == 13) && (cities[trains[i].Destination].block.x == drawTrainBlock.x) &&
						(cities[trains[i].Destination].block.y == drawTrainBlock.y)) {
						FinishTrain(&i, amountTrainsOnTheMap, trains, trainRedraw);
						if (trains[i].head.x == -1 || trains[i].head.x == 1401) {
							(*finishedTrains)++;
							(*Money) = (*Money) + 150;
							DrawingLabelFinishedTrains(hLaFinishedTrains, hdc, finishedTrains);
						}
						
					}
					//if wrong city, need to turn arround the train
					else if ((drawTrainBlock.x == 0 || drawTrainBlock.x == 13) && (trains[i].head.x == 0 || trains[i].head.x == 1400) &&
						(cities[trains[i].Destination].block.x != drawTrainBlock.x || cities[trains[i].Destination].block.y != drawTrainBlock.y)) {
						tempTrainBlock = trains[i].head;
						trains[i].head = trains[i].tail;
						trains[i].tail = tempTrainBlock;
						trains[i].preBlock = trains[i].block;

						if (drawTrainBlock.x == 13) {
							trains[i].preBlock.x++;
						}
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

						//(*Money) = (*Money) - 5;
					}
					//down
					else  if ((trains[i].head.y - trains[i].tail.y > 0) && movingDown) {
						trains[i].preBlock.x = drawTrainBlock.x;
						trains[i].preBlock.y = drawTrainBlock.y;

						trains[i].block.y = trains[i].head.y / 100;

						//(*Money) = (*Money) - 5;
					}
					//stop moving
					else if (!trains[i].Stop) {
						trains[i].Stop = TRUE;
					}

				}
			}

			if (*amountTrainsOnTheMap != -1) {

				LOGBRUSH brush = { 0 };
				DWORD pen_style = PS_GEOMETRIC; //PS_SOLID | PS_GEOMETRIC | PS_JOIN_BEVEL;

				brush.lbStyle = BS_SOLID;
				brush.lbColor = trains[i].Color;
				brush.lbHatch = 0;

				HPEN hPen = ExtCreatePen(pen_style, 25, &brush, 0, NULL);

				HPEN hOldPen = SelectObject(*hdc, hPen);

				MoveToEx(*hdc, trains[i].head.x, trains[i].head.y + 30, NULL);
				LineTo(*hdc, trains[i].tail.x, trains[i].tail.y + 30);

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

int CorrectTail(int* Tail, const int* Block) {

	if (*Tail - *Block < 50) {
		return *Tail + 1;
	}
	else if (*Tail - *Block > 50) {
		return *Tail - 1;
	}
	else {
		return *Tail;
	}
}

BYTE DirectMoving(int* numberTrain, byte Vertical, Train trains[]) {

	POINT head, tail, block;

	InizializeVariablesForTrainMovingCalculation(&head, &tail, &block, &trains[*numberTrain]);

	byte forwardVertical = trains[*numberTrain].block.y - trains[*numberTrain].preBlock.y < 0;
	byte forwardHorizontal = trains[*numberTrain].block.x - trains[*numberTrain].preBlock.x < 0;

	if (Vertical) {
		if (forwardVertical) {
			head.y--;
			tail.y--;

			tail.x = CorrectTail(&tail.x, &block.x);
		}
		else {
			head.y++;
			tail.y++;

			tail.x = CorrectTail(&tail.x, &block.x);
		}

	}
	else {
		if (forwardHorizontal) {
			head.x--;
			tail.x--;

			tail.y = CorrectTail(&tail.y, &block.y);
		}
		else
		{
			head.x++;
			tail.x++;

			tail.y = CorrectTail(&tail.y, &block.y);
		}
	}

	AssignedVaiablesBackAfterCalculation(&head, &tail, &trains[*numberTrain]);

	return forwardHorizontal || forwardVertical;
}

void TurningTrainLeftBottom(int* numberTrain, Train trains[]) {

	POINT head, tail, block;

	InizializeVariablesForTrainMovingCalculation(&head, &tail, &block, &trains[*numberTrain]);

	tail.y = CorrectTail(&tail.y, &block.y);

	MovingRightOrTopBeforeTurning(&head.x, &tail.x, &block.x);

	if (head.x >= (block.x + 20) && head.x < (block.x + 50)) {
		head.x++;
		tail.x += 2;
		head.y++;
	}

	if (head.y >= (block.y + 79) && head.y <= (block.y + 100)) {
		tail.x++;
		head.y += 2;
	}

	AssignedVaiablesBackAfterCalculation(&head, &tail, &trains[*numberTrain]);
}

void TurningTrainBottomLeft(int* numberTrain, Train trains[]) {

	POINT head, tail, block;

	InizializeVariablesForTrainMovingCalculation(&head, &tail, &block, &trains[*numberTrain]);
	
	block.y = block.y + 100;

	tail.x = CorrectTail(&tail.x, &block.x);

	MovingLeftOrBottomBeforeTurning(&head.y, &tail.y, &block.y);

	if (head.y <= (block.y - 20) && head.y > (block.y - 50)) {
		head.y--;
		tail.y -= 2;
		head.x--;
	}

	if (head.x <= (block.x + 29) && head.x >= (block.x)) {
		tail.y--;
		head.x -= 2;
	}

	AssignedVaiablesBackAfterCalculation(&head, &tail, &trains[*numberTrain]);
}

void TurningTrainTopRight(int* numberTrain, Train trains[]) {

	POINT head, tail, block;

	InizializeVariablesForTrainMovingCalculation(&head, &tail, &block, &trains[*numberTrain]);

	tail.x = CorrectTail(&tail.x, &block.x);

	MovingRightOrTopBeforeTurning(&head.y, &tail.y, &block.y);

	if (head.y >= (block.y + 20) && head.y < (block.y + 50)) {
		head.y++;
		tail.y += 2;
		head.x++;
	}

	if (head.x >= (block.x + 79) && head.x <= (block.x + 100)) {
		tail.y++;
		head.x += 2;
	}

	AssignedVaiablesBackAfterCalculation(&head, &tail, &trains[*numberTrain]);
}

void TurningTrainRightTop(int* numberTrain, Train trains[]) {

	POINT head, tail, block;

	InizializeVariablesForTrainMovingCalculation(&head, &tail, &block, &trains[*numberTrain]);

	block.x = block.x + 100;

	tail.y = CorrectTail(&tail.y, &block.y);

	MovingLeftOrBottomBeforeTurning(&head.x, &tail.x, &block.x);

	if (head.x <= (block.x - 20) && head.x > (block.x - 50)) {
		head.x--;
		tail.x -= 2;
		head.y--;
	}

	if (head.y <= (block.y + 29) && head.y >= (block.y)) {
		tail.x--;
		head.y -= 2;
	}

	AssignedVaiablesBackAfterCalculation(&head, &tail, &trains[*numberTrain]);
}

void TurningTrainLeftTop(int* numberTrain, Train trains[]) {

	POINT head, tail, block;

	InizializeVariablesForTrainMovingCalculation(&head, &tail, &block, &trains[*numberTrain]);

	tail.y = CorrectTail(&tail.y, &block.y);

	MovingRightOrTopBeforeTurning(&head.x, &tail.x, &block.x);

	if (head.x >= (block.x + 20) && head.x < (block.x + 50)) {
		head.x++;
		tail.x += 2;
		head.y--;
	}

	if (head.y <= (block.y + 29) && head.y >= (block.y)) {
		tail.x++;
		head.y -= 2;
	}

	AssignedVaiablesBackAfterCalculation(&head, &tail, &trains[*numberTrain]);
}

void TurningTrainTopLeft(int* numberTrain, Train trains[]) {

	POINT head, tail, block;

	InizializeVariablesForTrainMovingCalculation(&head, &tail, &block, &trains[*numberTrain]);

	tail.x = CorrectTail(&tail.x, &block.x);

	MovingRightOrTopBeforeTurning(&head.y, &tail.y, &block.y);

	if (head.y >= (block.y + 20) && head.y < (block.y + 50)) {
		head.y++;
		tail.y += 2;
		head.x--;
	}

	if (head.x <= (block.x + 29) && head.x >= (block.x)) {
		tail.y++;
		head.x -= 2;
	}

	AssignedVaiablesBackAfterCalculation(&head, &tail, &trains[*numberTrain]);
}

void TurningTrainRightBottom(int* numberTrain, Train trains[]) {

	POINT head, tail, block;

	InizializeVariablesForTrainMovingCalculation(&head, &tail, &block, &trains[*numberTrain]);

	block.x = block.x + 100;

	tail.y = CorrectTail(&tail.y, &block.y);

	MovingLeftOrBottomBeforeTurning(&head.x, &tail.x, &block.x);

	if (head.x <= (block.x - 20) && head.x > (block.x - 50)) {
		head.x--;
		tail.x -= 2;
		head.y++;
	}

	if (head.y >= (block.y + 79) && head.y <= (block.y + 100)) {
		tail.x--;
		head.y += 2;
	}

	AssignedVaiablesBackAfterCalculation(&head, &tail, &trains[*numberTrain]);
}

void TurningTrainBottomRight(int* numberTrain, Train trains[]) {

	POINT head, tail, block;

	InizializeVariablesForTrainMovingCalculation(&head, &tail, &block, &trains[*numberTrain]);

	block.y = block.y + 100;

	tail.x = CorrectTail(&tail.x, &block.x);

	MovingLeftOrBottomBeforeTurning(&head.y, &tail.y, &block.y);

	if (head.y <= (block.y - 20) && head.y > (block.y - 50)) {
		head.y--;
		tail.y -= 2;
		head.x++;
	}

	if (head.x >= (block.x + 79) && head.x <= (block.x + 100)) {
		tail.y--;
		head.x += 2;
	}

	AssignedVaiablesBackAfterCalculation(&head, &tail, &trains[*numberTrain]);
}

void FinishTrain(int* finishedTrain, int* trainsOnTheMap, Train trains[], RECT* trainRedrawRect) {

	if (trains[*finishedTrain].block.x == 0) {

		if (trains[*finishedTrain].tail.x > 0) {
			trains[*finishedTrain].head.x--;
			trains[*finishedTrain].tail.x--;
		}
		else if (trains[*finishedTrain].tail.x == -50) {
			trains[*finishedTrain] = trains[*trainsOnTheMap];
			trainRedrawRect[*finishedTrain] = trainRedrawRect[*trainsOnTheMap];
			(*trainsOnTheMap)--;
		}
	}

	if (trains[*finishedTrain].block.x == 13) {

		if (trains[*finishedTrain].tail.x < 1400) {
			trains[*finishedTrain].head.x++;
			trains[*finishedTrain].tail.x++;
		}
		else if (trains[*finishedTrain].tail.x == 1450) {
			trains[*finishedTrain] = trains[*trainsOnTheMap];
			trainRedrawRect[*finishedTrain] = trainRedrawRect[*trainsOnTheMap];
			(*trainsOnTheMap)--;
		}
	}
}

BYTE IsAnyTrainInTheBlock(Train train[], POINT Block, const int* trainOnTheMap, int* foundTrain) {
	BYTE result = FALSE;

	for (int i = 0; i <= *trainOnTheMap; i++) {
		if (train[i].block.x == Block.x && train[i].block.y == Block.y) {
			result = TRUE;
			*foundTrain = i;
		}
	}

	return result;
}

void AddNewTrain(int* numberOfTrains, int* numberOfCities, City* cities, Train* trains, RECT* trainsRedraw) {

	srand((unsigned int)time(NULL));

	int citySource, cityDest;

	citySource = rand() % (*numberOfCities + 1);
	cityDest = rand() % (*numberOfCities + 1);

	while (citySource == cityDest) {
		cityDest = rand() % (*numberOfCities + 1);
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

		trainsRedraw->left = trains[*numberOfTrains].tail.x - 30;
		trainsRedraw->bottom = trains[*numberOfTrains].head.y - 30;
		trainsRedraw->right = trains[*numberOfTrains].head.x + 30;
		trainsRedraw->top = trains[*numberOfTrains].head.x + 30;

	}
	//train sarts from right side
	else if (cities[citySource].block.x == CLIENT_AREA_X - 1) {
		trains[*numberOfTrains].head.x = cities[citySource].block.x * 100 + (100 - 1);
		trains[*numberOfTrains].head.y = cities[citySource].block.y * 100 + 50;
		trains[*numberOfTrains].tail.x = cities[citySource].block.x * 100 + (100 + 50 - 1);
		trains[*numberOfTrains].tail.y = cities[citySource].block.y * 100 + 50;
		trains[*numberOfTrains].block = cities[citySource].block;
		trains[*numberOfTrains].preBlock.x = cities[citySource].block.x + 1;
		trains[*numberOfTrains].preBlock.y = cities[citySource].block.y;
		trains[*numberOfTrains].Destination = cityDest;
		trains[*numberOfTrains].Color = cities[cityDest].Color;
		trains[*numberOfTrains].Stop = FALSE;
		trains[*numberOfTrains].MouseStop = FALSE;

		trainsRedraw->left = trains[*numberOfTrains].head.x - 30;
		trainsRedraw->bottom = trains[*numberOfTrains].head.y - 30;
		trainsRedraw->right = trains[*numberOfTrains].tail.x - 30;
		trainsRedraw->top = trains[*numberOfTrains].head.x + 30;
	}
}

void InizializeVariablesForTrainMovingCalculation(POINT* Head, POINT* Tail, POINT* Block, Train* Train) {

	*Head = Train->head;
	*Tail = Train->tail;
	Block->x = (Train->block.x * 100);
	Block->y = (Train->block.y * 100);
}

void AssignedVaiablesBackAfterCalculation(POINT* Head, POINT* Tail, Train* Train) {
	Train->head = *Head;
	Train->tail = *Tail;
}

void MovingRightOrTopBeforeTurning(int* head, int* tail, int* block) {
	
	if (*head < (*block + 20)) {
		(*head)++;
		(*tail)++;
	}
}

void MovingLeftOrBottomBeforeTurning(int* head, int* tail, int* block) {
	if (*head > (*block - 20)) {
		(*head)--;
		(*tail)--;
	}
}