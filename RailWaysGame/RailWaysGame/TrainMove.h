#pragma once
#include "Structures.h"

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

	int headX = trains[*numberTrain].head.x;
	int headY = trains[*numberTrain].head.y;
	int tailX = trains[*numberTrain].tail.x;
	int tailY = trains[*numberTrain].tail.y;
	int blockX = (trains[*numberTrain].block.x * 100);
	int blockY = (trains[*numberTrain].block.y * 100);

	byte forwardVertical, forwardHorizontal;

	forwardVertical = trains[*numberTrain].block.y - trains[*numberTrain].preBlock.y < 0;
	forwardHorizontal = trains[*numberTrain].block.x - trains[*numberTrain].preBlock.x < 0;

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

	trains[*numberTrain].head.x = headX;
	trains[*numberTrain].head.y = headY;
	trains[*numberTrain].tail.x = tailX;
	trains[*numberTrain].tail.y = tailY;

	return forwardHorizontal || forwardVertical;
}

void TurningTrainLeftBottom(int* numberTrain, struct Train trains[]) {

	int headX = trains[*numberTrain].head.x;
	int headY = trains[*numberTrain].head.y;
	int tailX = trains[*numberTrain].tail.x;
	int tailY = trains[*numberTrain].tail.y;
	int blockX = (trains[*numberTrain].block.x * 100);
	int blockY = (trains[*numberTrain].block.y * 100);

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

	trains[*numberTrain].head.x = headX;
	trains[*numberTrain].head.y = headY;
	trains[*numberTrain].tail.x = tailX;
	trains[*numberTrain].tail.y = tailY;
}

void TurningTrainBottomLeft(int* numberTrain, struct Train trains[]) {

	int headX = trains[*numberTrain].head.x;
	int headY = trains[*numberTrain].head.y;
	int tailX = trains[*numberTrain].tail.x;
	int tailY = trains[*numberTrain].tail.y;
	int blockX = (trains[*numberTrain].block.x * 100);
	int blockY = (trains[*numberTrain].block.y * 100 + 100);

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

	trains[*numberTrain].head.x = headX;
	trains[*numberTrain].head.y = headY;
	trains[*numberTrain].tail.x = tailX;
	trains[*numberTrain].tail.y = tailY;
}

void TurningTrainTopRight(int* numberTrain, struct Train trains[]) {

	int headX = trains[*numberTrain].head.x;
	int headY = trains[*numberTrain].head.y;
	int tailX = trains[*numberTrain].tail.x;
	int tailY = trains[*numberTrain].tail.y;
	int blockX = (trains[*numberTrain].block.x * 100);
	int blockY = (trains[*numberTrain].block.y * 100);

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

	trains[*numberTrain].head.x = headX;
	trains[*numberTrain].head.y = headY;
	trains[*numberTrain].tail.x = tailX;
	trains[*numberTrain].tail.y = tailY;
}

void TurningTrainRightTop(int* numberTrain, struct Train trains[]) {

	int headX = trains[*numberTrain].head.x;
	int headY = trains[*numberTrain].head.y;
	int tailX = trains[*numberTrain].tail.x;
	int tailY = trains[*numberTrain].tail.y;
	int blockX = (trains[*numberTrain].block.x * 100 + 100);
	int blockY = (trains[*numberTrain].block.y * 100);

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

	trains[*numberTrain].head.x = headX;
	trains[*numberTrain].head.y = headY;
	trains[*numberTrain].tail.x = tailX;
	trains[*numberTrain].tail.y = tailY;
}

void TurningTrainLeftTop(int* numberTrain, struct Train trains[]) {

	int headX = trains[*numberTrain].head.x;
	int headY = trains[*numberTrain].head.y;
	int tailX = trains[*numberTrain].tail.x;
	int tailY = trains[*numberTrain].tail.y;
	int blockX = (trains[*numberTrain].block.x * 100);
	int blockY = (trains[*numberTrain].block.y * 100);

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

	trains[*numberTrain].head.x = headX;
	trains[*numberTrain].head.y = headY;
	trains[*numberTrain].tail.x = tailX;
	trains[*numberTrain].tail.y = tailY;
}

void TurningTrainTopLeft(int* numberTrain, struct Train trains[]) {

	int headX = trains[*numberTrain].head.x;
	int headY = trains[*numberTrain].head.y;
	int tailX = trains[*numberTrain].tail.x;
	int tailY = trains[*numberTrain].tail.y;
	int blockX = (trains[*numberTrain].block.x * 100);
	int blockY = (trains[*numberTrain].block.y * 100);

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

	trains[*numberTrain].head.x = headX;
	trains[*numberTrain].head.y = headY;
	trains[*numberTrain].tail.x = tailX;
	trains[*numberTrain].tail.y = tailY;
}

void TurningTrainRightBottom(int* numberTrain, struct Train trains[]) {

	int headX = trains[*numberTrain].head.x;
	int headY = trains[*numberTrain].head.y;
	int tailX = trains[*numberTrain].tail.x;
	int tailY = trains[*numberTrain].tail.y;
	int blockX = (trains[*numberTrain].block.x * 100 + 100);
	int blockY = (trains[*numberTrain].block.y * 100);

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

	trains[*numberTrain].head.x = headX;
	trains[*numberTrain].head.y = headY;
	trains[*numberTrain].tail.x = tailX;
	trains[*numberTrain].tail.y = tailY;
}

void TurningTrainBottomRight(int* numberTrain, struct Train trains[]) {

	int headX = trains[*numberTrain].head.x;
	int headY = trains[*numberTrain].head.y;
	int tailX = trains[*numberTrain].tail.x;
	int tailY = trains[*numberTrain].tail.y;
	int blockX = (trains[*numberTrain].block.x * 100);
	int blockY = (trains[*numberTrain].block.y * 100 + 100);

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

	trains[*numberTrain].head.x = headX;
	trains[*numberTrain].head.y = headY;
	trains[*numberTrain].tail.x = tailX;
	trains[*numberTrain].tail.y = tailY;
}

void FinishTrain(int* finishedTrain, int* trainsOnTheMap, struct Train trains[]) {

	if (trains[*finishedTrain].block.x == 0) {

		if (trains[*finishedTrain].tail.x > 0) {
			trains[*finishedTrain].head.x--;
			trains[*finishedTrain].tail.x--;
		}
		else if (trains[*finishedTrain].tail.x == -50) {
			trains[*finishedTrain] = trains[*trainsOnTheMap];
			(*trainsOnTheMap)--;
		}
	}

	if (trains[*finishedTrain].block.x == 0) {

		if (trains[*finishedTrain].tail.x < 1400 && trains[*finishedTrain].tail.x > 1300) {
			trains[*finishedTrain].head.x++;
			trains[*finishedTrain].tail.x++;
		}
		else if (trains[*finishedTrain].tail.x == 1450) {
			trains[*finishedTrain] = trains[*trainsOnTheMap];
			(*trainsOnTheMap)--;
		}
	}
}