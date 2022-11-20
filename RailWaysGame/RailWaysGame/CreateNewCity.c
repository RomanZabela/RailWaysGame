#include "Structures.h"
#include "CreateNewCity.h"

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

	if (*randNumber < 7) {
		for (int i = 0; i < *numberCities; i++) {
			if (cities[i].block.y == *randNumber && cities[i].block.x == 0) {
				result = TRUE;
			}
		}
	}
	else if (*randNumber >= 7) {
		for (int i = 0; i <= *numberCities; i++) {
			if (cities[i].block.y == (*randNumber) - 7 && cities[i].block.x == 13) {
				result = TRUE;
			}
		}
	}

	return result;
}

void NewCity(const int numberCities, City cities[CITIES_ON_THE_AREA], Road map[CLIENT_AREA_X][CLIENT_AREA_Y], const COLORREF* BankOfColors) {

	srand((unsigned int)time(NULL));
	
	int randomNumber = rand() % 14;

	while (FindNotUsingPosition(&randomNumber, &numberCities, cities)) {
		randomNumber = rand() % 14;
	};

	//drawing in the left side
	if (randomNumber < 7) {
		cities[numberCities].block.y = randomNumber;
		cities[numberCities].block.x = 0;

		map[0][randomNumber].horizontal = 2;
		map[0][randomNumber].isRoad = TRUE;
	}
	//drawing in the right side
	else if (randomNumber >= 7) {
		cities[numberCities].block.y = randomNumber - 7;
		cities[numberCities].block.x = 13;

		map[13][randomNumber - 7].horizontal = 2;
		map[13][randomNumber - 7].isRoad = TRUE;
	};

	randomNumber = rand() % 14;

	while (FindColor(&randomNumber, &numberCities, &BankOfColors, cities)) {
		randomNumber = rand() % 14;
	}

	cities[numberCities].Color = BankOfColors[randomNumber];
}