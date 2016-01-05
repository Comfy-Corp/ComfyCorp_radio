#include <stdio.h>
#include <time.h>
#include "lcd.h"
#include "ui.h"
#include "userInput.h"
#include "keyboard.h"
#include "storage.h"
#include "rtc.h"
#include "watchdog.h"
#include "ethernet.h"
#include "alarmControl.h"

int tempTimezoneHours=0;
char *previousTime;

char screenStateChar = 0;

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
                if(streamName != NULL)
                {
                    if (streamNameSize < 16)
                    {
                        LcdWriteStringAtLoc(streamName, streamNameSize, streamNameLocLCD);
                    }
                    else
                    {
                        LcdWriteStringAtLoc(streamName, streamNameSize+1, streamNameLocLCD);
                    }
                }
                free(timeBuffer);
                break;
            case UISTATE_SHOWSYNCING:
            LcdWriteString("SYNCING",8);
                break;
            case UISTATE_SHOWALARM:
            if(AlarmControlActivePrimaryAlarm == NULL)
                LcdWriteString("Sleep 2 min",strlen("Sleep 2 min")+1);
            else
                LcdWriteString("Remove alarm",strlen("Remove alarm")+1);
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
                LcdWriteString( AlarmControlActivePrimaryAlarm -> alarmText,
                                strlen(AlarmControlActivePrimaryAlarm -> alarmText)+1);
                LcdSetCursor(0x40);
                LcdWriteString("OK:Snooze", strlen("OK:Snooze")+1);
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
    if (screenStateChar == UISTATE_ALARMEVENT)
    {
        return 0;
    }
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
    if (screenStateChar == UISTATE_ALARMEVENT)
    {
        return 0;
    }
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
        //printf("%s\n","I would like to sync, but I can not do that yet :(" );
        //char* url2= GetAlarmsHTTP("188.166.22.194/alarms");  
        AlarmControlAlarmEvent();
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

    if (screenStateChar == UISTATE_SHOWALARM)
    {
        if(AlarmControlActivePrimaryAlarm == NULL)
            AlarmControlSleep();
        else
            AlarmControlRemoveDailyAlarm();
        /*
        char* streamID = "118";
        char* netaddress = malloc(strlen(baseUrl)+6); 
        memset (netaddress, 0, urlLength);
        memcpy (netaddress, baseUrl, strlen(baseUrl));
        netaddress = strcat(netaddress, streamID);
        GetStreamURL(netaddress);
        free(netaddress);  */
        screenStateChar = UISTATE_SHOWTIME;
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
    if(screenStateChar == UISTATE_ALARMEVENT)
    {
        screenStateChar = UISTATE_SHOWTIME;
        alarmPlayingFlag = 0;       
        UIshow();        
        AlarmControlSnoozePrimary();
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
    if(screenStateChar == UISTATE_ALARMEVENT){
        alarmPlayingFlag = 0;
        AlarmControlRemoveDailyAlarm();
    }
    if(screenStateChar == UISTATE_SHOWRESET)
        return 1;
    screenStateChar = UISTATE_SHOWTIME;
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
        if(AlarmControlActivePrimaryAlarm != NULL){
            LcdSetCursor(0x40);
            LcdAlarmIcon(0x4F);
        }
    }
    return 1;
}

int UIHandleReset(){
    WatchDogEnable();
    WatchDogStart(30);
    for (;;) {};
}

int UIGetUserSetTimezone(void)
{
    return tempTimezoneHours;
}

int getStateChar(void)
{
    return screenStateChar;
}
