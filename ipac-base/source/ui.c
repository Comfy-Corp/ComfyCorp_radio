#include <stdio.h>
#include <time.h>
#include "lcd.h"
#include "ui.h"
#include "userinput.h"
#include "keyboard.h"
#include "rtc.h"

char screenStateChar = UISTATE_SHOWTIME;
int tempTimezoneHours=0;


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
            printf("UISTATE_SHOWSETUP\n");
            getTimeZoneString(timeBuffer2, 0);
            LcdWriteString(timeBuffer2, sizeof(timeBuffer2));
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
    if (screenStateChar == UISTATE_SHOWSETUP)
    {
        char timeBuffer[6];
        --tempTimezoneHours;
        if (tempTimezoneHours < -12)
        {
            tempTimezoneHours = 11;
        }
        getTimeZoneString(timeBuffer, tempTimezoneHours);
        LcdWriteString(timeBuffer, sizeof(timeBuffer));
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
        char timeBuffer[6];
        ++tempTimezoneHours;
        if (tempTimezoneHours >= 12)
        {
            tempTimezoneHours = -12;
        }
        getTimeZoneString(timeBuffer, tempTimezoneHours);
        LcdWriteString(timeBuffer, sizeof(timeBuffer));
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
    return 1;
}

int UIGetUserSetTimezone(void)
{
    return tempTimezoneHours;
}

