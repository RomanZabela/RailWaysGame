#pragma once
#include <Windows.h>

typedef struct Train {
	POINT head;
	POINT tail;
	POINT block;
	POINT preBlock;
	int Destination;
	COLORREF Color;
	byte Stop;
	byte MouseStop;
} Train;

typedef struct Road {
	int leftBottom;
	int bottomRight;
	int topRight;
	int leftTop;
	int horizontal;
	int vertical;
	byte isRoad;
} Road;

typedef struct NewRoad {
	POINT block;
	struct Road road;
} NewRoad;

typedef struct City {
	POINT block;
	COLORREF Color; //BGR
} City;