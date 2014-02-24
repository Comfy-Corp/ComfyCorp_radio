#include <stdio.h>
#include <time.h>
#include "lcd.h"
#include "ui.h"
#include "userinput.h"
#include "keyboard.h"
#include "rtc.h"

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

extern tm timeZone;

int UIshow()
{
	LcdClear();
    char timeBuffer[9];
	switch (screenStateChar)
        {
            case UISTATE_SHOWTIME:
            fillStringWithTime(timeBuffer);
            LcdWriteString(timeBuffer, sizeof(timeBuffer));
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
            UIDisplayTimeZoneSelection(timeBuffer);
            LcdWriteString(timeBuffer, sizeof(timeBuffer));
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

        //TODO PROBLEM HOW CAN I CONTINUE FROM THE TIMEZONE WITHOUT THE ENTER KEY TO SAVE IT?
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
    if (screenStateChar == UISTATE_SHOWSETUP){
        //TODO executecode to make the timezone go to -
        --timeZone.tm_hour;
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
    if(screenStateChar == UISTATE_SHOWSETUP)
    {
        +timeZone.tm_hour;
    }
    return 1;
}

int UIScreenEsc()
{
    screenStateChar = 0;
    UIshow();
    return 1;
}

int UIHandleInput(int kb_error)
{
    if (kb_error != KB_ERROR)
    {
        userInputKeyPress();
    }
    
    return 1;
}


int UIDisplayTimeZone(char *timeZoneBufferToFill)
{
    getTimeZone(timeZone, timeZoneBufferToFill);
    return 1;
}


