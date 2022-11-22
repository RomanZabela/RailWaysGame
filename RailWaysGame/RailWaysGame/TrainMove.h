#pragma once

void DrawTrains(HWND*, HWND*, HDC*, PAINTSTRUCT*, RECT*, const int*, Train*, Road[CLIENT_AREA_X][CLIENT_AREA_Y], City*, int*);

//BYTE DirectMoving(int*, byte, Train[]);
//void TurningTrainLeftBottom(int*, Train[]);
//void TurningTrainBottomLeft(int*, Train[]);
//void TurningTrainTopRight(int*, Train[]);
//void TurningTrainRightTop(int*, Train[]);
//void TurningTrainLeftTop(int*, Train[]);
//void TurningTrainTopLeft(int*, Train[]);
//void TurningTrainRightBottom(int*, Train[]);
//void TurningTrainBottomRight(int*, Train[]);

void FinishTrain(int*, int*, Train[]);
BYTE IsAnyTrainInTheBlock(Train[], POINT, const int*, int*);