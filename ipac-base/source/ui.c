#include <stdio.h>
#include "lcd.h"
#include "ui.h"

char screenStateChar = UISTATE_SHOWTIME;

int UIshow()
{
	LcdClear();
	switch (screenStateChar)
        {
            case UISTATE_SHOWTIME:
                break;
            case UISTATE_SHOWSYNCING:
                break;
            case UISTATE_SHOWALARM:
                break;
            case UISTATE_SHOWRESET:
                break;
            case UISTATE_SHOWSETUP:
                break;
            case UISTATE_SHOWMENU:
                break;
        }
	return screenStateChar;
}

void UIchangeState(char newState)
{
	screenStateChar = newState;
	UIshow();
}

/*struct menuItem[] mainMenu = {
	{
		"Dit is een mens"
	}
};*/
