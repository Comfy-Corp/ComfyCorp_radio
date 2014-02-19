#include <stdio.h>
#include "lcd.h"
#include "ui.h"
#include "userinput.h"
#include "keyboard.h"

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
    char *timeBuffer = malloc(sizeof(char) * 20);
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
            LcdBackLightBriefOn(100);
            userInputKeyPress();
        }
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
