#include "Structures.h"
#include "MousePosition.h"

int MousePositionBlock(POINT mousePosition, POINT Block) {
	int result = -1;

	if ((mousePosition.x - (Block.x * 100) > 5) && (mousePosition.x - (Block.x * 100) < 35) &&
		(mousePosition.y - (Block.y * 100) > 40) && (mousePosition.y - (Block.y * 100) < 60)) {
		result = 1;
	};

	if ((mousePosition.x - (Block.x * 100) > 40) && (mousePosition.x - (Block.x * 100) < 60) &&
		(mousePosition.y - (Block.y * 100) > 65) && (mousePosition.y - (Block.y * 100) < 95)) {
		result = 2;
	};

	if ((mousePosition.x - (Block.x * 100) > 65) && (mousePosition.x - (Block.x * 100) < 95) &&
		(mousePosition.y - (Block.y * 100) > 40) && (mousePosition.y - (Block.y * 100) < 60)) {
		result = 3;
	};

	if ((mousePosition.x - (Block.x * 100) > 40) && (mousePosition.x - (Block.x * 100) < 60) &&
		(mousePosition.y - (Block.y * 100) > 5) && (mousePosition.y - (Block.y * 100) < 35)) {
		result = 4;
	}

	return result;
}