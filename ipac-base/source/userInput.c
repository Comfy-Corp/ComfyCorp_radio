#include <stdio.h>
#include <string.h>

#include "display.h" 
#include "keyboard.h"
#include "userInput.h"



void buttonPressed(u_char keyPressed, u_char menuItem){
	if (keyPressed == KEY_ESC || keyPressed == KEY_OK || keyPressed == KEY_ALT || keyPressed == KEY_POWER ||
	 keyPressed == KEY_01 || keyPressed == KEY_02 || keyPressed == KEY_03 || keyPressed == KEY_04 ||
	 keyPressed == KEY_05 || keyPressed == KEY_UP || keyPressed == KEY_DOWN || keyPressed == KEY_LEFT ||
	 keyPressed == KEY_RIGHT){
	 	char str [16];
	 	LcdClear();
		switch(keyPressed){
			//special keys
			case KEY_ESC:
				sprintf(str, "Esc pressed at %c", menuItem);
				LcdWriteString(str, (int)sizeof(str));
			break;
			case KEY_OK:
				sprintf(str, "Ok pressed at %c", menuItem);
				LcdWriteString(str, 16 * sizeof(char));
			break;
			case KEY_ALT:
				sprintf(str, "Alt pressed at %c", menuItem);
				LcdWriteString(str, (int)sizeof(str));
			break;
			case KEY_POWER:
				sprintf(str, "Power pressed at %c", menuItem);
				LcdWriteString(str, (int)sizeof(str));
			break;
			// number keys
			case KEY_01:
				sprintf(str, "01 pressed at %c", menuItem);
				LcdWriteString(str, (int)sizeof(str));
			break;
			case KEY_02:
				sprintf(str, "02 pressed at %c", menuItem);
				LcdWriteString(str, (int)sizeof(str));
			break;
			case KEY_03:
				sprintf(str, "03 pressed at %c", menuItem);
				LcdWriteString(str, (int)sizeof(str));
			break;
			case KEY_04:
				sprintf(str, "04 pressed at %c", menuItem);
				LcdWriteString(str, (int)sizeof(str));
			break;
			case KEY_05:
				sprintf(str, "05 pressed at %c", menuItem);
				LcdWriteString(str, (int)sizeof(str));
			break;
			//direction keys
			case KEY_UP:
				sprintf(str, "Up pressed at %c", menuItem);
				LcdWriteString(str, (int)sizeof(str));
			break;
			case KEY_DOWN:
				sprintf(str, "Down pressed at %c", menuItem);
				LcdWriteString(str, (int)sizeof(str));
			break;
			case KEY_RIGHT:
				sprintf(str, "Right pressed at %c", menuItem);
				LcdWriteString(str, (int)sizeof(str));
			break;
			case KEY_LEFT:
				sprintf(str, "Left pressed at %c", menuItem);
				LcdWriteString(str, (int)sizeof(str));
			break;
		}
	}
}



