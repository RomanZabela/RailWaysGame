# RailWaysGame
Simple game for MASA

Author: Zabela Roman

Email: cplCold@gmail.com

video from the game: https://drive.google.com/file/d/1kZ5A9mrfUTQ6nb8evb4_cdFTPRS-rt-G/view?usp=share_link

The Train game created for learning C while studying on the Full Stack program from MASA and Zionet

Version 1.0.0:

- In the beginng there are two cities on the area. From time to time new city appers.

- The colorfull trains start randomly from one of the cities. The trains should arriv to destination city of the same color.

- User can build the railways, switch any arrows and stop the trains.


Version 1.0.1:

1. Changed the colors for better visualisation
2. Separated function from the file HelpForMain.c and removed the file
3. Rebuilt Main RailWaysGame.c file:
	a. Moved all procedures to specific files:
		Create, Move and draw trains -> CreatNewTrainAndMove.c
		Create and draw City -> CreateCityAndDrawing.c
		Drawing Railways -> DrawingRails.c
	b. Fixed all warnings
	c. Kept in place the procedures responsible for main logics and connected with system
3. Isolated all Moving procedures into CreatNewTrainAndMove.c: Turning left and right, Moving up and down
4. Moved client area down to 30px
