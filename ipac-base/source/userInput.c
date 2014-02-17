#include <stdio.h>
#include <string.h>

#include "display.h" 
#include "keyboard.h"
#include "userInput.h"





void buttonPressed(u_char keyPressed){
	if (keyPressed == KEY_ESC || keyPressed == KEY_OK || keyPressed == KEY_ALT || keyPressed == KEY_POWER ||
	 keyPressed == KEY_01 || keyPressed == KEY_02 || keyPressed == KEY_03 || keyPressed == KEY_04 ||
	 keyPressed == KEY_05 || keyPressed == KEY_UP || keyPressed == KEY_DOWN || keyPressed == KEY_LEFT ||
	 keyPressed == KEY_RIGHT){
	 	LcdClear();
		switch(keyPressed){
			//special keys
			case KEY_ESC:
				LcdWriteString("Esc pressed", sizeof("Esc pressed"));
			break;
			case KEY_OK:
				LcdWriteString("Ok pressed", sizeof("Ok pressed"));
			break;
			case KEY_ALT:
				LcdWriteString("Alt pressed", sizeof("Alt pressed"));
			break;
			case KEY_POWER:
				LcdWriteString("Power pressed", sizeof("Power pressed"));
			break;
			// number keys
			case KEY_01:
				LcdWriteString("01 pressed", sizeof("01 pressed"));
			break;
			case KEY_02:
				LcdWriteString("02 pressed", sizeof("02 pressed"));
			break;
			case KEY_03:
				LcdWriteString("03 pressed", sizeof("03 pressed"));
			break;
			case KEY_04:
				LcdWriteString("04 pressed", sizeof("04 pressed"));
			break;
			case KEY_05:
				LcdWriteString("05 pressed", sizeof("05 pressed"));
			break;
			//direction keys
			case KEY_UP:
				LcdWriteString("Up pressed", sizeof("Up pressed"));
			break;
			case KEY_DOWN:
				LcdWriteString("Down pressed", sizeof("Down pressed"));
			break;
			case KEY_RIGHT:
				LcdWriteString("Right pressed", sizeof("Right pressed"));
			break;
			case KEY_LEFT:
				LcdWriteString("Left pressed", sizeof("Left pressed"));
			break;
		}
	}
}

