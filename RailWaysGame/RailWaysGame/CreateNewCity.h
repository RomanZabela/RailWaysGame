#pragma once

BYTE FindColor(const int*, const int*, const COLORREF*, City[]);
BYTE FindNotUsingPosition(const int*, const int*, City[]);
void NewCity(const int, City[CITIES_ON_THE_AREA], Road[CLIENT_AREA_X][CLIENT_AREA_Y], const COLORREF*);