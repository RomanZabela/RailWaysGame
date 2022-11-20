#pragma once

BYTE DirectMoving(int*, byte, Train[]);
void TurningTrainLeftBottom(int*, Train[]);
void TurningTrainBottomLeft(int*, Train[]);
void TurningTrainTopRight(int*, Train[]);
void TurningTrainRightTop(int*, Train[]);
void TurningTrainLeftTop(int*, Train[]);
void TurningTrainTopLeft(int*, Train[]);
void TurningTrainRightBottom(int*, Train[]);
void TurningTrainBottomRight(int*, Train[]);
void FinishTrain(int*, int*, Train[]);
BYTE TrainInTheBlock(Train[], POINT, const int*, int*);