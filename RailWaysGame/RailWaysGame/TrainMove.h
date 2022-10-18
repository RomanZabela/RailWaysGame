#pragma once
#include "MyStructures.h"

BYTE DirectMoving(int*, byte, struct Train[]);
void TurningTrainLeftBottom(int*, struct Train[]);
void TurningTrainBottomLeft(int*, struct Train[]);
void TurningTrainTopRight(int*, struct Train[]);
void TurningTrainRightTop(int*, struct Train[]);
void TurningTrainLeftTop(int*, struct Train[]);
void TurningTrainTopLeft(int*, struct Train[]);
void TurningTrainRightBottom(int*, struct Train[]);
void TurningTrainBottomRight(int*, struct Train[]);
void FinishTrain(int*, int*, struct Train[]);

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

BYTE DirectMoving(int* numberTrain, byte Vertical, struct Train trains[]) {

	int headX = trains[*numberTrain].headX;
	int headY = trains[*numberTrain].headY;
	int tailX = trains[*numberTrain].tailX;
	int tailY = trains[*numberTrain].tailY;
	int blockX = (trains[*numberTrain].blockX * 100);
	int blockY = (trains[*numberTrain].blockY * 100);

	byte forwardVertical, forwardHorizontal;

	forwardVertical = trains[*numberTrain].blockY - trains[*numberTrain].preBlockY < 0;
	forwardHorizontal = trains[*numberTrain].blockX - trains[*numberTrain].preBlockX < 0;

	if (Vertical) {
		if (forwardVertical) {
			headY--;
			tailY--;

			tailX = CorrectTail(&tailX, &blockX);
		}
		else {
			headY++;
			tailY++;

			tailX = CorrectTail(&tailX, &blockX);
		}

	}
	else {
		if (forwardHorizontal) {
			headX--;
			tailX--;

			tailY = CorrectTail(&tailY, &blockY);
		}
		else
		{
			headX++;
			tailX++;

			tailY = CorrectTail(&tailY, &blockY);
		}
	}

	trains[*numberTrain].headX = headX;
	trains[*numberTrain].headY = headY;
	trains[*numberTrain].tailX = tailX;
	trains[*numberTrain].tailY = tailY;

	return forwardHorizontal || forwardVertical;
}

void TurningTrainLeftBottom(int* numberTrain, struct Train trains[]) {

	int headX = trains[*numberTrain].headX;
	int headY = trains[*numberTrain].headY;
	int tailX = trains[*numberTrain].tailX;
	int tailY = trains[*numberTrain].tailY;
	int blockX = (trains[*numberTrain].blockX * 100);
	int blockY = (trains[*numberTrain].blockY * 100);

	tailY = CorrectTail(&tailY, &blockY);

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

	trains[*numberTrain].headX = headX;
	trains[*numberTrain].headY = headY;
	trains[*numberTrain].tailX = tailX;
	trains[*numberTrain].tailY = tailY;
}

void TurningTrainBottomLeft(int* numberTrain, struct Train trains[]) {

	int headX = trains[*numberTrain].headX;
	int headY = trains[*numberTrain].headY;
	int tailX = trains[*numberTrain].tailX;
	int tailY = trains[*numberTrain].tailY;
	int blockX = (trains[*numberTrain].blockX * 100);
	int blockY = (trains[*numberTrain].blockY * 100 + 100);

	tailX = CorrectTail(&tailX, &blockX);

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

	trains[*numberTrain].headX = headX;
	trains[*numberTrain].headY = headY;
	trains[*numberTrain].tailX = tailX;
	trains[*numberTrain].tailY = tailY;
}

void TurningTrainTopRight(int* numberTrain, struct Train trains[]) {

	int headX = trains[*numberTrain].headX;
	int headY = trains[*numberTrain].headY;
	int tailX = trains[*numberTrain].tailX;
	int tailY = trains[*numberTrain].tailY;
	int blockX = (trains[*numberTrain].blockX * 100);
	int blockY = (trains[*numberTrain].blockY * 100);

	tailX = CorrectTail(&tailX, &blockX);

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

	trains[*numberTrain].headX = headX;
	trains[*numberTrain].headY = headY;
	trains[*numberTrain].tailX = tailX;
	trains[*numberTrain].tailY = tailY;
}

void TurningTrainRightTop(int* numberTrain, struct Train trains[]) {

	int headX = trains[*numberTrain].headX;
	int headY = trains[*numberTrain].headY;
	int tailX = trains[*numberTrain].tailX;
	int tailY = trains[*numberTrain].tailY;
	int blockX = (trains[*numberTrain].blockX * 100 + 100);
	int blockY = (trains[*numberTrain].blockY * 100);

	tailY = CorrectTail(&tailY, &blockY);

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

	trains[*numberTrain].headX = headX;
	trains[*numberTrain].headY = headY;
	trains[*numberTrain].tailX = tailX;
	trains[*numberTrain].tailY = tailY;
}

void TurningTrainLeftTop(int* numberTrain, struct Train trains[]) {

	int headX = trains[*numberTrain].headX;
	int headY = trains[*numberTrain].headY;
	int tailX = trains[*numberTrain].tailX;
	int tailY = trains[*numberTrain].tailY;
	int blockX = (trains[*numberTrain].blockX * 100);
	int blockY = (trains[*numberTrain].blockY * 100);

	tailY = CorrectTail(&tailY, &blockY);

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

	trains[*numberTrain].headX = headX;
	trains[*numberTrain].headY = headY;
	trains[*numberTrain].tailX = tailX;
	trains[*numberTrain].tailY = tailY;
}

void TurningTrainTopLeft(int* numberTrain, struct Train trains[]) {

	int headX = trains[*numberTrain].headX;
	int headY = trains[*numberTrain].headY;
	int tailX = trains[*numberTrain].tailX;
	int tailY = trains[*numberTrain].tailY;
	int blockX = (trains[*numberTrain].blockX * 100);
	int blockY = (trains[*numberTrain].blockY * 100);

	tailX = CorrectTail(&tailX, &blockX);

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

	trains[*numberTrain].headX = headX;
	trains[*numberTrain].headY = headY;
	trains[*numberTrain].tailX = tailX;
	trains[*numberTrain].tailY = tailY;
}

void TurningTrainRightBottom(int* numberTrain, struct Train trains[]) {

	int headX = trains[*numberTrain].headX;
	int headY = trains[*numberTrain].headY;
	int tailX = trains[*numberTrain].tailX;
	int tailY = trains[*numberTrain].tailY;
	int blockX = (trains[*numberTrain].blockX * 100 + 100);
	int blockY = (trains[*numberTrain].blockY * 100);

	tailY = CorrectTail(&tailY, &blockY);

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

	trains[*numberTrain].headX = headX;
	trains[*numberTrain].headY = headY;
	trains[*numberTrain].tailX = tailX;
	trains[*numberTrain].tailY = tailY;
}

void TurningTrainBottomRight(int* numberTrain, struct Train trains[]) {

	int headX = trains[*numberTrain].headX;
	int headY = trains[*numberTrain].headY;
	int tailX = trains[*numberTrain].tailX;
	int tailY = trains[*numberTrain].tailY;
	int blockX = (trains[*numberTrain].blockX * 100);
	int blockY = (trains[*numberTrain].blockY * 100 + 100);

	tailX = CorrectTail(&tailX, &blockX);

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

	trains[*numberTrain].headX = headX;
	trains[*numberTrain].headY = headY;
	trains[*numberTrain].tailX = tailX;
	trains[*numberTrain].tailY = tailY;
}

void FinishTrain(int* finishedTrain, int* trainsOnTheMap, struct Train trains[]) {

	if (trains[*finishedTrain].blockX == 0) {

		if (trains[*finishedTrain].tailX > 0) {
			trains[*finishedTrain].headX--;
			trains[*finishedTrain].tailX--;
		}
		else if (trains[*finishedTrain].tailX == -50) {
			trains[*finishedTrain] = trains[*trainsOnTheMap];
			(*trainsOnTheMap)--;
		}
	}

	if (trains[*finishedTrain].blockX == 0) {

		if (trains[*finishedTrain].tailX < 1400 && trains[*finishedTrain].tailX > 1300) {
			trains[*finishedTrain].headX++;
			trains[*finishedTrain].tailX++;
		}
		else if (trains[*finishedTrain].tailX == 1450) {
			trains[*finishedTrain] = trains[*trainsOnTheMap];
			(*trainsOnTheMap)--;
		}
	}
}