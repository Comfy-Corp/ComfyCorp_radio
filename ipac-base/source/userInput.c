#include <stdio.h>
#include <string.h>

#include "display.h" 
#include "keyboard.h"





void buttonPressed(u_char keyPressed, u_char menuValue){
	u_char str[16];
	u_char *string[] = &str;
	switch(pressedButton){
		case KEY_ESC:
			string = "Esc Button ";
		break;
		case KEY_OK:
			string = "Ok Button ";
		break;
		case KEY_ALT:
			string = "Alt Button ";
		break;
		case KEY_ POWER:
			string = "Power Button ";
		break;
		// number keys
		case KEY_01:
			string = "01 Button ";
		break;
		case KEY_02:
			string = "02 Button ";
		break;
		case KEY_03:
			string = "03 Button ";
		break;
		case KEY_04:
			string = "04 Button ";
		break;
		case KEY_05:
			string = "05 Button ";
		break;
		case KEY_UP:
			string = "Up Button ";
		break;
		case KEY_DOWN:
			string = "Down Button ";
		break;
		case KEY_RIGHT:
			string = "Right Button ";
		break;
		case KEY_LEFT:
			string = "Left Button ";
		break;
	}
	strcat (str, menuValue);
	LcdWriteString(str, sizeof(str));
}
