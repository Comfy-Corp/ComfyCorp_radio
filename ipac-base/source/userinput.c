#include <stdio.h>
#include "ui.h"
#include "userInput.h"
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
		case KEY_POWER:
			UIHandleReset();
			break;
		case KEY_01:
			VsSetVolume(1,1);
		case KEY_02:
			VsSetVolume(2,2);
		case KEY_03:
			VsSetVolume(3,3);
		case KEY_04:
			VsSetVolume(4,4);
		case KEY_05:
			VsSetVolume(5,5);
		case KEY_ALT:
			printf("Current volume is: %d\n", VsGetVolume());
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