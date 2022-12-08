#include "Structures.h"
#include "CreateCityAndDrawing.h"

void CityDrawing(HDC* hdc, int* numberOfCities, City* cities) {

	LOGBRUSH brush = { 0 };
	DWORD pen_style = PS_SOLID | PS_GEOMETRIC | PS_JOIN_ROUND;

	brush.lbStyle = BS_SOLID;
	brush.lbColor = RGB(0, 0, 0);
	brush.lbHatch = 0;

	HPEN hPenBuild = ExtCreatePen(pen_style, 2, &brush, 0, NULL);

	brush.lbColor = RGB(252, 252, 25);

	HPEN hPenWindows = ExtCreatePen(pen_style, 1, &brush, 0, NULL);

	HPEN hOldPen = SelectObject(*hdc, hPenBuild);


	HBRUSH hBrushWindows = CreateSolidBrush(RGB(252, 252, 25));

	POINT triangle[3] = { 0 }, coordinates = { 0 };

	for (int i = 0; i <= *numberOfCities; i++) {

		HBRUSH hBrushBuild = CreateSolidBrush(cities[i].Color);

		SelectObject(*hdc, hPenBuild);
		HBRUSH hOldBrush = SelectObject(*hdc, hBrushBuild);

		coordinates.x = cities[i].block.x * 100;
		coordinates.y = cities[i].block.y * 100;

		triangle[0].x = 36 + coordinates.x;
		triangle[0].y = 100 + coordinates.y;

		triangle[1].x = 50 + coordinates.x;
		triangle[1].y = 90 + coordinates.y;

		triangle[2].x = 62 + coordinates.x;
		triangle[2].y = 100 + coordinates.y;

		Rectangle(*hdc, 15 + coordinates.x, 40 + coordinates.y, 45 + coordinates.x, 80 + coordinates.y);

		SelectObject(*hdc, hBrushWindows);
		SelectObject(*hdc, hPenWindows);

		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 2; j++) {
				Rectangle(*hdc, 20 + (15 * i) + coordinates.x, 45 + (20 * j) + coordinates.y,
					25 + (15 * i) + coordinates.x, 55 + (20 * j) + coordinates.y);
			}
		}

		SelectObject(*hdc, hBrushBuild);
		SelectObject(*hdc, hPenBuild);

		Rectangle(*hdc, 5 + coordinates.x, 70 + coordinates.y, 35 + coordinates.x, 110 + coordinates.y);
		Rectangle(*hdc, 40 + coordinates.x, 100 + coordinates.y, 60 + coordinates.x, 120 + coordinates.y);
		Rectangle(*hdc, 55 + coordinates.x, 45 + coordinates.y, 75 + coordinates.x, 65 + coordinates.y);

		Polygon(*hdc, triangle, 3);

		triangle[0].x = 51 + coordinates.x;
		triangle[0].y = 45 + coordinates.y;

		triangle[1].x = 65 + coordinates.x;
		triangle[1].y = 35 + coordinates.y;

		triangle[2].x = 77 + coordinates.x;
		triangle[2].y = 45 + coordinates.y;

		Polygon(*hdc, triangle, 3);

		SelectObject(*hdc, hPenWindows);

		SelectObject(*hdc, hBrushWindows);

		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 2; j++) {
				Rectangle(*hdc, 10 + (15 * i) + coordinates.x, 75 + (20 * j) + coordinates.y,
					15 + (15 * i) + coordinates.x, 85 + (20 * j) + coordinates.y);
			}
		}

		Rectangle(*hdc, 45 + coordinates.x, 105 + coordinates.y, 50 + coordinates.x, 110 + coordinates.y);
		Rectangle(*hdc, 60 + coordinates.x, 50 + coordinates.y, 65 + coordinates.x, 55 + coordinates.y);

		SelectObject(*hdc, hOldBrush);
		DeleteObject(hBrushBuild);
	}


	SelectObject(*hdc, hOldPen);

	DeleteObject(hPenWindows);
	DeleteObject(hBrushWindows);
	DeleteObject(hPenBuild);
}

BYTE FindColor(const int* ColorNumber, const int* numberCities, const COLORREF* BankOfColors, City cities[]) {

	BYTE result = FALSE;

	for (int i = 0; i < *numberCities; i++) {
		if (BankOfColors[*ColorNumber] == cities[i].Color) {
			result = TRUE;
		}
	}

	return result;
}

BYTE FindNotUsingPosition(const int* randNumber, const int* numberCities, City cities[]) {

	BYTE result = FALSE;

	if (*randNumber < MAX_CITIES_ON_THE_AREA / 2) {
		for (int i = 0; i < *numberCities; i++) {
			if (cities[i].block.y == *randNumber && cities[i].block.x == 0) {
				result = TRUE;
			}
		}
	}
	else if (*randNumber >= MAX_CITIES_ON_THE_AREA / 2) {
		for (int i = 0; i <= *numberCities; i++) {
			if (cities[i].block.y == (*randNumber) - (MAX_CITIES_ON_THE_AREA / 2) && cities[i].block.x == 13) {
				result = TRUE;
			}
		}
	}

	return result;
}

void NewCity(const int numberCities, City cities[MAX_CITIES_ON_THE_AREA], Road map[CLIENT_AREA_X][CLIENT_AREA_Y], const COLORREF* BankOfColors) {

	srand((unsigned int)time(NULL));
	
	int randomNumber = rand() % MAX_CITIES_ON_THE_AREA;

	while (FindNotUsingPosition(&randomNumber, &numberCities, cities)) {
		randomNumber = rand() % MAX_CITIES_ON_THE_AREA;
	};

	//drawing in the left side
	if (randomNumber < (MAX_CITIES_ON_THE_AREA / 2)) {
		cities[numberCities].block.y = randomNumber;
		cities[numberCities].block.x = 0;

		map[0][randomNumber].horizontal = 2;
		map[0][randomNumber].isRoad = TRUE;
	}
	//drawing in the right side
	else if (randomNumber >= (MAX_CITIES_ON_THE_AREA / 2)) {
		cities[numberCities].block.y = randomNumber - (MAX_CITIES_ON_THE_AREA / 2);
		cities[numberCities].block.x = 13;

		map[13][randomNumber - (MAX_CITIES_ON_THE_AREA / 2)].horizontal = 2;
		map[13][randomNumber - (MAX_CITIES_ON_THE_AREA / 2)].isRoad = TRUE;
	};

	randomNumber = rand() % MAX_CITIES_ON_THE_AREA;

	while (FindColor(&randomNumber, &numberCities, BankOfColors, cities)) {
		randomNumber = rand() % MAX_CITIES_ON_THE_AREA;
	}

	cities[numberCities].Color = BankOfColors[randomNumber];
}