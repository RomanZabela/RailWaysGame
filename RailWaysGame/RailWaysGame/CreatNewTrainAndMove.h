#pragma once

void DrawTrains(HWND*, HDC*, PAINTSTRUCT*, RECT*, int*, Train*, Road[CLIENT_AREA_X][CLIENT_AREA_Y], City*, int*, int*);
BYTE IsAnyTrainInTheBlock(Train[], POINT, const int*, int*);
void AddNewTrain(int*, int*, City*, Train*, RECT*);