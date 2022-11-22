#include "Structures.h"
#include "Timer.h"

void RestartTimer(HWND* hwnd, const int* TimerID) {
	SetTimer(*hwnd, *TimerID, 60, NULL);
}