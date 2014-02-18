#include <stdio.h>
#include "ui.h"
#include "userinput.h"
#include "keyboard.h"

void userInputKeyPress()
{
	u_char keyPressed = KbGetKey();
	switch(keyPressed)
	{
		case KEY_UP:
			UIScreenUp();
			break;
		case KEY_DOWN:
			UIScreenDown();
			break;
		case KEY_LEFT:
			UIScreenLeft();
			break;
		case KEY_RIGHT:
			UIScreenRight();
			break;
		case KEY_OK:
			UIScreenOK();
			break;
		case KEY_ESC:
			UIScreenEsc();
			break;

	}
}

/*
#define KEY_SPEC       0
#define KEY_01         1
#define KEY_02         2
#define KEY_03         3
#define KEY_04         4
#define KEY_05         5
#define KEY_ALT        6

#define KEY_ESC        7
#define KEY_UP         8
#define KEY_OK         9
#define KEY_LEFT       10
#define KEY_DOWN       11
#define KEY_RIGHT      12

#define KEY_POWER      13
#define KEY_SETUP      14
#define KEY_LCD        15 
*/