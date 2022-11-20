#pragma once

void DrawingRoads(HWND*, HDC*, PAINTSTRUCT*, Road, int*, int*, int*);
NewRoad ResetNewRoad(NewRoad);
void DrawingRailWays(HWND, HDC, PAINTSTRUCT, Road[CLIENT_AREA_X][CLIENT_AREA_Y], NewRoad);