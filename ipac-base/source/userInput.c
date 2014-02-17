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
	 	LcdClear();
		switch(keyPressed){
			//special keys
			case KEY_ESC:
				LcdWriteString("Esc pressed " + menuItem, sizeof("Esc pressed " + menuItem));
			break;
			case KEY_OK:
				LcdWriteString("Ok pressed " + menuItem, sizeof("Ok pressed " + menuItem));
			break;
			case KEY_ALT:
				LcdWriteString("Alt pressed " + menuItem, sizeof("Alt pressed " + menuItem));
			break;
			case KEY_POWER:
				LcdWriteString("Power pressed " + menuItem, sizeof("Power pressed " + menuItem));
			break;
			// number keys
			case KEY_01:
				LcdWriteString("01 pressed " + menuItem, sizeof("01 pressed " + menuItem));
			break;
			case KEY_02:
				LcdWriteString("02 pressed " + menuItem, sizeof("02 pressed " + menuItem));
			break;
			case KEY_03:
				LcdWriteString("03 pressed " + menuItem, sizeof("03 pressed " + menuItem));
			break;
			case KEY_04:
				LcdWriteString("04 pressed " + menuItem, sizeof("04 pressed " + menuItem));
			break;
			case KEY_05:
				LcdWriteString("05 pressed " + menuItem, sizeof("05 pressed " + menuItem));
			break;
			//direction keys
			case KEY_UP:
				LcdWriteString("Up pressed " + menuItem, sizeof("Up pressed " + menuItem));
			break;
			case KEY_DOWN:
				LcdWriteString("Down pressed " + menuItem, sizeof("Down pressed " + menuItem));
			break;
			case KEY_RIGHT:
				LcdWriteString("Right pressed " + menuItem, sizeof("Right pressed " + menuItem));
			break;
			case KEY_LEFT:
				LcdWriteString("Left pressed " + menuItem, sizeof("Left pressed " + menuItem));
			break;
		}
	}
}

