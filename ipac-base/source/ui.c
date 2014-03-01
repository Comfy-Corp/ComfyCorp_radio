#include <stdio.h>
#include <time.h>
#include "lcd.h"
#include "ui.h"
#include "userinput.h"
#include "keyboard.h"
#include "storage.h"
#include "rtc.h"

char screenStateChar = UISTATE_SHOWTIME;
int tempTimezoneHours=0;
char *previousTime;

int UIshow()
{
	LcdClear();
    char *timeBuffer = malloc(sizeof(char) * 8);
    char *timeBuffer2 = malloc(sizeof(char) * 8);
	switch (screenStateChar)
        {
            case UISTATE_SHOWTIME:           
                X12FillStringWithTime(timeBuffer);
                previousTime = timeBuffer;
                LcdSetCursor(0x44);
                LcdWriteString(timeBuffer, strlen(timeBuffer)+1);
                free(timeBuffer);
                break;
            case UISTATE_SHOWSYNCING:
            LcdWriteString("SYNCING",8);
                break;
            case UISTATE_SHOWALARM:
            LcdWriteString("ALARMZZ",8);
                break;
            case UISTATE_SHOWRESET:
            LcdWriteString("FACTORY RESET?",15);
                break;
            case UISTATE_SHOWSETUP:
            printf("UISTATE_SHOWSETUP\n");
                X12GetTimeZoneString(timeBuffer2, 0);
                LcdSetCursor(0x00);
                LcdWriteString("Hoe laat is het?", strlen("Hoe laat is het?")+1);
                LcdSetCursor(0x44);
                LcdWriteString(timeBuffer2, strlen(timeBuffer2)+1);
                free(timeBuffer2);
                break;
            case UISTATE_ALARMEVENT:
                LcdClear();
                LcdSetCursor(0x00);
                LcdWriteString("Beep beep.", strlen("Beep beep."));
                LcdSetCursor(0x40);
                LcdWriteString("Faggot.", strlen("Faggot."));
                LcdBackLightBriefOn(200);
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
        return 1;
        //TODO PROBLEM HOW CAN I CONTINUE FROM THE TIMEZONE WITHOUT THE ENTER KEY TO SAVE IT?
    }
    if (screenStateChar == UISTATE_SHOWRESET)
    {
        _StorableSetting zero = {0, sizeof(zero)};
        StorageSaveConfig(&zero);
        screenStateChar = UISTATE_SHOWSETUP;
        UIshow();
        return 1;
    }

    if(screenStateChar == UISTATE_SHOWSETUP)
    {
        _StorableSetting timeZoneHour = {tempTimezoneHours, sizeof(timeZoneHour)};
        StorageSaveConfig(&timeZoneHour);
        printf("saved: %d", timeZoneHour);
        screenStateChar = UISTATE_SHOWTIME;
        LcdClear();
        return 1;
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
        LcdSetCursor(0x00);
        LcdWriteString("Hoe laat is het?", strlen("Hoe laat is het?")+1);
        LcdSetCursor(0x44);
        X12GetTimeZoneString(timeBuffer, tempTimezoneHours);
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
        LcdSetCursor(0x00);
        LcdWriteString("Hoe laat is het?", strlen("Hoe laat is het?")+1);
        LcdSetCursor(0x44);
        X12GetTimeZoneString(timeBuffer, tempTimezoneHours);
        LcdWriteString(timeBuffer, sizeof(timeBuffer));
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
        if ((kb_error != KB_ERROR))
        {
            LcdBackLightBriefOn(100);
            userInputKeyPress();
        }
        return 1;
}

int UIRefreshScreen(){
    if(screenStateChar == UISTATE_SHOWTIME){
        char *timeBuffer = malloc(sizeof(char) * 8);
        X12FillStringWithTime(timeBuffer);
        LcdSetCursor(0x44);
        LcdWriteString(timeBuffer, strlen(timeBuffer)+1);
        free(timeBuffer);
    }    
    return 1;
}

int UIGetUserSetTimezone(void)
{
    return tempTimezoneHours;
}

