#pragma once
#include <windows.h>
#include <math.h>
#include "MyStructures.h"

#define TWOPI (2 * 3.14159)

void DrawingRotedRails(HWND*, HDC*, PAINTSTRUCT*, int*, int*, int, BYTE);
void DrawStraightRails(HWND*, HDC*, PAINTSTRUCT*, int*, int*, BYTE);
void HelpForDrawingRoads(HWND*, HDC*, PAINTSTRUCT*, struct Road, int*, int*, int*);
void RotatePoint(POINT[], int);

void DrawingRotedRails(HWND* hwnd, HDC* hdc, PAINTSTRUCT* ps, int* xBlock, int* yBlock, int iAngle, BYTE MainRoad) {

	LOGBRUSH brush;
	DWORD pen_style = PS_GEOMETRIC;

	brush.lbStyle = BS_SOLID;
	
	if (MainRoad) {
		brush.lbColor = RGB(0, 0, 0);
	}
	else {
		brush.lbColor = RGB(121, 121, 121);
	}
	
	brush.lbHatch = 0;

	HPEN hPenRailSteel = ExtCreatePen(pen_style, 3 + (1*MainRoad), &brush, 0, NULL);

	brush.lbColor = RGB(196, 107, 29);
	HPEN hPenRailWood = ExtCreatePen(pen_style, 4, &brush, 0, NULL);
	HPEN hOldPen = SelectObject(*hdc, hPenRailWood);

	POINT pt[2];
	int x = 100 * *xBlock;
	int y = 100 * *yBlock;

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

		MoveToEx(*hdc, pt[0].x, pt[0].y, NULL);
		LineTo(*hdc, pt[1].x, pt[1].y);
	};

	SelectObject(*hdc, hPenRailSteel);

	//Arc(hdc, xLeft, yTop, xRight, yBottom, xStart, yStart, xEnd, yEnd) ;

	for (int i = 0; i < 2; i++) {
		switch (iAngle)
		{
		case 1:
			Arc(*hdc, (x - 55 + (i * 10)), (y - 55 + (i * 10)), (x + 55 - (i * 10)),
				(y + 55 - (i * 10)), (x + 55 - (i * 10)), y, x, (y - 55 + (i * 10)));
			break;
		case 2:
			Arc(*hdc, (x - 55 + (i * 10)), (y - 55 + (i * 10)), (x + 55 - (i * 10)),
				(y + 55 - (i * 10)), x, (y - 55 + (i * 10)), (x - 55 + (i * 10)), y);
			break;
		case 3:
			Arc(*hdc, (x - 55 + (i * 10)), (y - 55 + (i * 10)), (x + 55 - (i * 10)),
				(y + 55 - (i * 10)), (x - 55 + (i * 10)), y, x, (y + 45 + (i * 10)));
			break;
		case 4:
			Arc(*hdc, (x - 55 + (i * 10)), (y - 55 + (i * 10)), (x + 55 - (i * 10)),
				(y + 55 - (i * 10)), x, (y + 55 - (i * 10)), (x + 55 - (i * 10)), y);
			break;

		}
	}

	SelectObject(*hdc, hOldPen);
	DeleteObject(hPenRailSteel);
	DeleteObject(hPenRailWood);
}

void DrawStraightRails(HWND* hwnd, HDC* hdc, PAINTSTRUCT* ps, int* xBlock, int* yBlock, BYTE HorizontalFlag, BYTE MainRoad) {

	LOGBRUSH brush;
	DWORD pen_style = PS_GEOMETRIC; // | PS_JOIN_BEVEL | PS_SOLID;

	brush.lbStyle = BS_SOLID;
	if (MainRoad) {
		brush.lbColor = RGB(0, 0, 0);
	}
	else {
		brush.lbColor = RGB(121, 121, 121);
	}
	
	brush.lbHatch = 0;

	HPEN hPenRailSteel = ExtCreatePen(pen_style, 3 + (1 * MainRoad), &brush, 0, NULL);

	brush.lbColor = RGB(196, 107, 29);
	HPEN hPenRailWood = ExtCreatePen(pen_style, 4, &brush, 0, NULL);
	HPEN hOldPen = (HPEN) SelectObject(*hdc, hPenRailWood);

	int xMove = (100 * *xBlock);
	int yMove = (100 * *yBlock);
	int xTemp, yTemp;

	if (HorizontalFlag) {
		for (int i = 0; i < 10; i++)
		{
			xTemp = (i * 10) + 5 + xMove;
			MoveToEx(*hdc, xTemp, (40 + yMove), NULL);
			LineTo(*hdc, xTemp, (60 + yMove));
		};
	}
	else {
		for (int i = 0; i < 10; i++)
		{
			yTemp = (i * 10) + 5 + yMove;
			MoveToEx(*hdc, (40 + xMove), yTemp, NULL);
			LineTo(*hdc, (60 + xMove), yTemp);
		};
	};

	SelectObject(*hdc, hPenRailSteel);

	if (HorizontalFlag) {
		for (int i = 0; i < 2; i++) {
			yTemp = 45 + (10 * i) + yMove;
			MoveToEx(*hdc, (xMove), (yTemp), NULL);
			LineTo(*hdc, (100 + xMove), (yTemp));
		}
	}
	else {
		for (int i = 0; i < 2; i++) {
			xTemp = 45 + (10 * i) + xMove;
			MoveToEx(*hdc, xTemp, yMove, NULL);
			LineTo(*hdc, xTemp, (100 + yMove));
		};
	};


	SelectObject(*hdc, hOldPen);
	DeleteObject(hPenRailSteel);
	DeleteObject(hPenRailWood);
}

void HelpForDrawingRoads(HWND* hwnd, HDC* hdc, PAINTSTRUCT* ps, struct Road road, int* Type, int* BlockX, int* BlockY) {

	BYTE MainRoad = FALSE; // turn main way on

	if (road.isRoad) {
		if (road.horizontal == *Type) {

			if (*Type == 2 && (road.leftBottom == 1 || road.leftTop == 1 || road.bottomRight == 1 || road.topRight == 1)) {
				MainRoad = TRUE;
			}
			else {
				MainRoad = FALSE;
			}
			DrawStraightRails(hwnd, hdc, ps, BlockX, BlockY, TRUE, MainRoad);
		}
		if (road.vertical == *Type) {

			if (*Type == 2 && (road.leftBottom == 1 || road.leftTop == 1 || road.bottomRight == 1 || road.topRight == 1)) {
				MainRoad = TRUE;
			}
			else {
				MainRoad = FALSE;
			}

			DrawStraightRails(hwnd, hdc, ps, BlockX, BlockY, FALSE, MainRoad);

		}
		if (road.leftBottom == *Type) {

			if (*Type == 2 && (road.horizontal == 1 || road.vertical == 1 || road.leftTop == 1 || road.bottomRight == 1)) {
				MainRoad = TRUE;
			}
			else {
				MainRoad = FALSE;
			}
			DrawingRotedRails(hwnd, hdc, ps, BlockX, BlockY, 1, MainRoad);

		}
		if (road.bottomRight == *Type) {

			if (*Type == 2 && (road.horizontal == 1 || road.vertical == 1 || road.leftBottom == 1 || road.topRight == 1)) {
				MainRoad = TRUE;
			}
			else {
				MainRoad = FALSE;
			}

			DrawingRotedRails(hwnd, hdc, ps, BlockX, BlockY, 2, MainRoad);
		}
		if (road.topRight == *Type) {

			if (*Type == 2 && (road.horizontal == 1 || road.vertical == 1 || road.bottomRight == 1 || road.leftTop == 1)) {
				MainRoad = TRUE;
			}
			else {
				MainRoad = FALSE;
			}

			DrawingRotedRails(hwnd, hdc, ps, BlockX, BlockY, 3, MainRoad);
		}
		if (road.leftTop == *Type) {

			if (*Type == 2 && (road.horizontal == 1 || road.vertical == 1 || road.topRight == 1 || road.leftBottom == 1)) {
				MainRoad = TRUE;
			}
			else {
				MainRoad = FALSE;
			}

			DrawingRotedRails(hwnd, hdc, ps, BlockX, BlockY, 4, MainRoad);
		}
	}
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