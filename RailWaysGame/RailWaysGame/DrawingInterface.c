#include "Structures.h"
#include "DrawingInterface.h"

void DrawingLabelFinishedTrains(HWND* hLaFinishedTrains, HDC* hdc, const int* finishedTrains) {

	wchar_t buf[21];

	wsprintfW(buf, L"Trains finished: %ld", *finishedTrains);

	SetWindowTextW(*hLaFinishedTrains, buf);
}