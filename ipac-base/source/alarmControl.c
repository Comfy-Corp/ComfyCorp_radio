/*******************************************************
 *
 * Alarm control center
 * Use to manipulate alarms and keep tabs on active alarms.
 *
 *******************************************************/
#include <time.h>
#include "rtc.h"
#include "ui.h"
#include <string.h>
#include "alarmControl.h"
#include <io.h>
#include <stdlib.h>
#include <stdio.h>
#include "player.h"

char* baseUrl = "37.46.136.205/cgi-bin/api.php?q=getstreamurl&id=";
char* constExtens = "&time=";

void AlarmControlInit(void){
	X12RtcClearStatus(0x30);
	tm *testAlarm = malloc(sizeof(tm));
	testAlarm -> tm_min = 99; //Primary
	testAlarm -> tm_hour = 99;
	X12RtcSetAlarm(0, testAlarm, 0x06); //Checks on HH:MM
	//Warning, you didn't free :(
	free(testAlarm);
	_alarm *nonVolAlarm = malloc(sizeof(_alarm));
	StorageLoadPrimaryAlarm(&nonVolAlarm);
	AlarmControlCreateDailyAlarm(nonVolAlarm);
	free(nonVolAlarm);
}

void AlarmControlSleep(void){
	tm *sleepTime = malloc(sizeof(tm));
	X12RtcGetClock(sleepTime);
	sleepTime->tm_min += 2;
	if(sleepTime->tm_min>59){
		sleepTime->tm_min %= 60;
		sleepTime->tm_hour++;
		if(sleepTime->tm_hour>23)
			sleepTime->tm_hour = 0;
	}	
	struct _alarm *sleepAlarm = malloc(sizeof(_alarm));
	sleepAlarm->alarmText = "Wake up!";
	sleepAlarm->alarmStreamName = "19";
	sleepAlarm->alarmType = 0; //Primary
	sleepAlarm->alarmTime = sleepTime;
	AlarmControlCreateDailyAlarm(sleepAlarm); //Set alarm to 5 seconds.
}

void AlarmControlSnoozePrimary(){
	tm *now = malloc(sizeof(tm));
	X12RtcGetClock(now);
	now->tm_min += 5;
	if(now->tm_min>59){
		now->tm_min %= 60;
		now->tm_hour++;
	}
	struct _alarm *snoozeAlarm = malloc(sizeof(_alarm));
	snoozeAlarm->alarmText = "Snooze";
	snoozeAlarm->alarmStreamName;
	snoozeAlarm->alarmType = 0; //Primary
	snoozeAlarm->alarmTime = now;
	AlarmControlCreateDailyAlarm(snoozeAlarm); //Set alarm to 5 seconds.
}

//Compares Sec, Min & Hour default alarm slot 0
void AlarmControlCreateDailyAlarm(struct _alarm *alarm){
	if(AlarmControlActivePrimaryAlarm != NULL)
		free(AlarmControlActivePrimaryAlarm);
	AlarmControlActivePrimaryAlarm = malloc(sizeof(_alarm));
	AlarmControlActivePrimaryAlarm->alarmText = alarm->alarmText;	
	AlarmControlActivePrimaryAlarm->alarmStreamName = alarm -> alarmStreamName;
	AlarmControlActivePrimaryAlarm->alarmTime = alarm->alarmTime; //Fix here
	memcpy(AlarmControlActivePrimaryAlarm, alarm, sizeof(_alarm));
	AlarmControlPrintActiveAlarm();
	X12RtcSetAlarm(0, alarm->alarmTime, 0x06); //Checks on HH:MM
}

void AlarmControlRemoveDailyAlarm(){
	AlarmControlPrintActiveAlarm();
	AlarmControlActivePrimaryAlarm = NULL;
	tm beyondTime;
	beyondTime.tm_min = 99;
	beyondTime.tm_hour = 99;
	X12RtcSetAlarm(0, &beyondTime, 0x00);
}

void AlarmControlCreateYearlyAlarm(struct _alarm alarm){
	if(AlarmControlActiveSecondaryAlarm != NULL)
		free(AlarmControlActiveSecondaryAlarm);
	AlarmControlActiveSecondaryAlarm = malloc(sizeof(_alarm));
	*AlarmControlActiveSecondaryAlarm = alarm;	
	X12RtcSetAlarm(1, alarm.alarmTime, 0x1E); //Checks on Month + Day of Month + HH:MM. Requires alarmTime to contain these.
}

void AlarmControlPrintActiveAlarm(){
	 tm *AlarmTime = AlarmControlActivePrimaryAlarm -> alarmTime;
	 printf("AlarmA: \"%s\" %02d:%02d:%02d \n",AlarmControlActivePrimaryAlarm->alarmText, AlarmTime -> tm_hour, AlarmTime -> tm_min, AlarmTime -> tm_sec);
}

u_long AlarmControlCheck(){
	u_long result;
	X12RtcGetStatus(&result);
	if(result == RTC_STATUS_AL0){ //Primary alarm (Dayly)
		X12RtcClearStatus(0x30);
		AlarmControlAlarmEvent();
	}
	return  result; //Possibly not neccessary
}

//params: no idea what to do here either
//ideas 
void AlarmControlAlarmEvent(){
	size_t urlLength = strlen(baseUrl) + 32;
	char* streamID = AlarmControlActivePrimaryAlarm->alarmStreamName;
    char* netaddress = malloc(urlLength);
    memset (netaddress, 0, urlLength); 
    strcpy (netaddress, baseUrl);
    netaddress = strcat(netaddress, streamID);
    tm *timeNow = malloc(sizeof(tm));
   	X12RtcGetClock(timeNow);
    char* tijd = malloc(6);
    sprintf(tijd, "%d:%d\0", timeNow->tm_hour, timeNow->tm_min);
    free(timeNow);
    printf("tijd:%s\n", tijd);
    netaddress = strcat(netaddress, constExtens);
    //netaddress = strcat(netaddress, tijd);
	char *streamMe = GetStreamURL(netaddress);
	printf("Prepping:%s\n",streamMe);
    if (isPlaying())
    {
        setPlaying(0);
        NutSleep(1500);
    }
    FILE* stream = GetHTTPRawStreamWithAddress(streamMe);
	printf("Alarm!\n");
   	initPlayer();
    //puts(stream);
    int playResult = play(stream);
    UIchangeState(UISTATE_ALARMEVENT);
	free(netaddress);
	free(tijd);
}
