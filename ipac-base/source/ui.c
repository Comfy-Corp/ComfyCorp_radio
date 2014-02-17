#include <stdio.h>
#include "lcd.h"


 #define STATE_SHOWTIME 0
 #define STATE_SHOWMENU 1
 #define STATE_SHOWSETUP 2
 #define STATE_SHOWRESET 3
 #define STATE_SHOWALARM 4
 #define STATE_SHOWSYNCING 5

char STATE;
char timeBuffer[9];
char screenStateChar = STATE_SHOWTIME;

int UIshow()
{
	switch (screenStateChar)
        {
            case STATE_SHOWTIME:
            	LcdClear();
            	fillStringWithTime(timeBuffer);
            	LcdWriteString(timeBuffer, sizeof(timeBuffer));
                break;
            case STATE_SHOWSYNCING:
                break;
            case STATE_SHOWALARM:
                break;
            case STATE_SHOWRESET:
                break;
            case STATE_SHOWSETUP:
                break;
            case STATE_SHOWMENU:
                break;
        }
	return STATE;
}

void UIchangeState(char newState)
{
	STATE = newState;
}