#include <stdio.h>
#include "lcd.h"
#include "ui.h"
#include "userinput.h"
#include "keyboard.h"
#include "storage.h"

char screenStateChar = UISTATE_SHOWTIME;
/*
int UIshow();
void UIchangeState(char);
int UIGetState();
int UIScreenUp();
int UIScreenDown();
int UIScreenOK();
int UIScreenLeft();
int UIScreenEsc();
int UIHandleInput();
*/

int UIshow()
{
	LcdClear();
    char *timeBuffer = malloc(sizeof(char) * 16);
	switch (screenStateChar)
        {
            case UISTATE_SHOWTIME:                
                fillStringWithTime(timeBuffer);
                LcdSetCursor(0x00);
                LcdWriteString(timeBuffer, strlen(timeBuffer)+1);
                break;
            case UISTATE_SHOWSYNCING:
            LcdWriteString("SYNCING",8);
                break;
            case UISTATE_SHOWALARM:
            LcdWriteString("ALARMZZ",8);
                break;
            case UISTATE_SHOWRESET:
            LcdWriteString("FACTORY RESET?",16);
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
    return 1;
}

int UIScreenDown()
{
    --screenStateChar;
    if (screenStateChar < 0)
    {
        screenStateChar = 2;
    }
    UIshow();
    return 1;
}

int UIScreenOK()
{
    if (screenStateChar == UISTATE_SHOWSYNCING)
    {
        //TODO ADD ONLINE SETTINGS SYNCING METHODE
        printf("%s\n","I would like to sync, but I can not do that yet :(" );
    }
    if (screenStateChar == UISTATE_SHOWRESET)
    {
        _StorableSetting zero = {0, sizeof(zero)};
        StorageSaveConfig(&zero);
        screenStateChar = UISTATE_SHOWSETUP;
        UIshow();
    }
    return 1;
}

int UIScreenLeft()
{
    if (screenStateChar == UISTATE_SHOWALARM)
    {
        //TODO ADD SCROLLING ALARMS
        printf("%s\n","I would like to go left, but I can not do that yet :(" );
    }
    return 1;
}

int UIScreenRight()
{
    if (screenStateChar == UISTATE_SHOWALARM)
    {
        //TODO ADD SCROLLING ALARMS
        printf("%s\n","I would like to go right, but I can not do that yet :(" );
    }    
    return 1;
}

int UIScreenEsc()
{
    if(screenStateChar == UISTATE_SHOWRESET)
        return 1;
    screenStateChar = 0;
    UIshow();
    return 1;
}

int UIHandleInput(int kb_error)
{
        if (kb_error != KB_ERROR)
        {
            LcdBackLightBriefOn(100);
            userInputKeyPress();
        }
        return 1;
}

int UIRefreshScreen(){
    if(screenStateChar == UISTATE_SHOWTIME){
        char *timeBuffer = malloc(sizeof(char) * 20);
        fillStringWithTime(timeBuffer);
        LcdSetCursor(0x00);
        LcdWriteString(timeBuffer, strlen(timeBuffer)+1);
    }
}

/*struct menuItem[] mainMenu = {
	{
		"Dit is een mens"
	}
};*/
