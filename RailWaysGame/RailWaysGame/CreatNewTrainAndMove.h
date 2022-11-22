#pragma once

void DrawTrains(HWND*, HWND*, HDC*, PAINTSTRUCT*, RECT*, const int*, Train*, Road[CLIENT_AREA_X][CLIENT_AREA_Y], City*, int*);
void FinishTrain(int*, int*, Train[]);
BYTE IsAnyTrainInTheBlock(Train[], POINT, const int*, int*);
void AddNewTrain(int*, int*, City*, Train*);