#include "Structures.h"
#include "DrawingInterface.h"

void DrawingLabelFinishedTrains(HWND* hLaFinishedTrains, HDC* hdc, const int* finishedTrains) {

	wchar_t buf[21]; //amount of symbols in the string for drawing

	wsprintfW(buf, L"Trains finished: %ld", *finishedTrains);

	SetWindowTextW(*hLaFinishedTrains, buf);
}

void DrawingLabelMoney(HWND* hLaMoney, HDC* hdc, const int* Money) {

	wchar_t buf[20]; //amount of symbols in the string for drawing

	wsprintfW(buf, L"Money: %ld $", *Money);

	SetWindowTextW(*hLaMoney, buf);
}