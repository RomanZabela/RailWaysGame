#pragma once
#include <Windows.h>
#include "Structures.h"

BYTE FindColor(const int*, const int*, const COLORREF*, City[]);
BYTE FindNotUsingPosition(const int*, const int*, City[]);

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
			if (cities[i].block.y == *randNumber && cities[i].block.x == 13) {
				result = TRUE;
			}
		}
	}

	return result;
}