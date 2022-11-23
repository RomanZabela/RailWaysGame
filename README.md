# RailWaysGame
Simple game for MASA

Author: Zabela Roman
Email: cplCold@gmail.com

The Train game created for learning C while studying on the Full Stack program from MASA and Zionet

Version 1.0.0:

- In the beginng there are two cities on the area. Time to time new city appered.

- The colorfull trains started randomly from one of the cities. The trains should arrived in destination city whith the same color.

- User can build railways, switch arrows and stop trains.


Version 1.0.1:

1. Change colors for better visualisation
2. Separated function from the file HelpForMain.c and removed the file
3. Rebuilt Main RailWaysGame.c file:
	a. Moved all procedures to specific files:
		Create, Move and draw trains -> CreatNewTrainAndMove.c
		Create and draw City -> CreateCityAndDrawing.c
		Drawing Railways -> DrawingRails.c
	b. Fixed all warnings
	c. Kept in place the procedures responsible for main logics and connected with system
3. Isolated all Moving procedures into CreatNewTrainAndMove.c: Turning left and right, Moving up and down
