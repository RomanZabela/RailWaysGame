#pragma once
#include <Windows.h>
#include "Structures.h"

void RestartTimer(HWND*, int*);
BYTE TrainInTheBlock(Train, POINT, const int*, int*);
int MousePosition(POINT, POINT);
NewRoad ResetNewRoad(NewRoad);

int MousePosition(POINT mousePosition, POINT Block) {
	int result = -1;

	if ((mousePosition.x - (Block.x * 100) > 5) && (mousePosition.x - (Block.x * 100) < 35) &&
		(mousePosition.y - (Block.y * 100) > 40) && (mousePosition.y - (Block.y * 100) < 60)) {
		result = 1;
	};

	if ((mousePosition.x - (Block.x * 100) > 40) && (mousePosition.x - (Block.x * 100) < 60) &&
		(mousePosition.y - (Block.y * 100) > 65) && (mousePosition.y - (Block.y * 100) < 95)) {
		result = 2;
	};

	if ((mousePosition.x - (Block.x * 100) > 65) && (mousePosition.x - (Block.x * 100) < 95) &&
		(mousePosition.y - (Block.y * 100) > 40) && (mousePosition.y - (Block.y * 100) < 60)) {
		result = 3;
	};

	if ((mousePosition.x - (Block.x * 100) > 40) && (mousePosition.x - (Block.x * 100) < 60) &&
		(mousePosition.y - (Block.y * 100) > 5) && (mousePosition.y - (Block.y * 100) < 35)) {
		result = 4;
	}

	return result;
}

struct NewRoad ResetNewRoad(NewRoad newRoadBlock) {
	newRoadBlock.road.horizontal = 0;
	newRoadBlock.road.vertical = 0;
	newRoadBlock.road.bottomRight = 0;
	newRoadBlock.road.leftBottom = 0;
	newRoadBlock.road.topRight = 0;
	newRoadBlock.road.leftTop = 0;

	return newRoadBlock;
}

void RestartTimer(HWND* hwnd, int* TimerID) {
	SetTimer(*hwnd, *TimerID, 60, NULL);
}

BYTE TrainInTheBlock(Train train[], POINT Block, const int* trainOnTheMap, int* foundTrain) {
	BYTE result = FALSE;

	for (int i = 0; i <= *trainOnTheMap; i++) {
		if (train[i].block.x == Block.x && train[i].block.y == Block.y) {
			result = TRUE;
			*foundTrain = i;
		}
	}

	return result;
}