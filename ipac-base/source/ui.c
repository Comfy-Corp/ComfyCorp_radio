#include <stdio.h>
#include "lcd.h"
#include "ui.h"
#include "userinput.h"
#include "keyboard.h"

char screenStateChar = UISTATE_SHOWTIME;

int UIshow()
{
	LcdClear();
	switch (screenStateChar)
        {
            case UISTATE_SHOWTIME:
            LcdWriteString("Ze Time",8);
                break;
            case UISTATE_SHOWSYNCING:
            LcdWriteString("SYNCING",8);
                break;
            case UISTATE_SHOWALARM:
            LcdWriteString("ALARMZZ",8);
                break;
            case UISTATE_SHOWRESET:
            LcdWriteString("RESET ME",9);
                break;
            case UISTATE_SHOWSETUP:
            LcdWriteString("SETUP SCREEN",13);
                break;
            default:
                break;
        }
	return screenStateChar;
}

void UIchangeState(char newState)
{
	screenStateChar = newState;
	UIshow();
}

int UIGetState()
{
    return screenStateChar;
}

int UIScreenUp()
{
    ++screenStateChar;
    if (screenStateChar > 2)
    {
        screenStateChar = 0;
    }
    UIshow();
    printf("%s\n", "Iraan houdt zich niet aan de afspraken over nucliare uranium verrijking!");
}

int UIScreenDown()
{
    --screenStateChar;
    if (screenStateChar < 0)
    {
        screenStateChar = 2;
    }
    UIshow();
}

int UIScreenOK()
{
    if (screenStateChar == UISTATE_SHOWSYNCING)
    {
        //TODO ADD ONLINE SETTINGS SYNCING METHODE
        printf("%s\n","I would like to sync, but I can not do that yet :(" );
    }
}

int UIScreenLeft()
{
    if (screenStateChar == UISTATE_SHOWALARM)
    {
        //TODO ADD SCROLLING ALARMS
        printf("%s\n","I would like to go left, but I can not do that yet :(" );
    }
}

int UIScreenRight()
{
    if (screenStateChar == UISTATE_SHOWALARM)
    {
        //TODO ADD SCROLLING ALARMS
        printf("%s\n","I would like to go right, but I can not do that yet :(" );
    }    
}

int UIScreenEsc()
{
    screenStateChar = 0;
    UIshow();
}

int UIHandleInput(int kb_error)
{
        if (kb_error != KB_ERROR)
        {
            userInputKeyPress();
        }
}

/*struct menuItem[] mainMenu = {
	{
		"Dit is een mens"
	}
};*/
