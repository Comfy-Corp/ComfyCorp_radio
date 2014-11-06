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
	sleepAlarm->alarmStreamName = "119";
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
		if(now->tm_hour>23)
			now->tm_hour = 0;
	}
	struct _alarm *snoozeAlarm = malloc(sizeof(_alarm));
	snoozeAlarm->alarmText = "Snooze";
	snoozeAlarm->alarmStreamName = AlarmControlActivePrimaryAlarm->alarmStreamName;
	snoozeAlarm->alarmType = 0; //Primary
	snoozeAlarm->alarmTime = now;
	AlarmControlCreateDailyAlarm(snoozeAlarm); //Set alarm to 5 seconds.
}

//Compares Sec, Min & Hour default alarm slot 0
void AlarmControlCreateDailyAlarm(struct _alarm *alarm){
	if(AlarmControlActivePrimaryAlarm != NULL){
		free(AlarmControlActivePrimaryAlarm);
		AlarmControlActivePrimaryAlarm = NULL;		
	}
	AlarmControlActivePrimaryAlarm = calloc(1,sizeof(_alarm));
	AlarmControlActivePrimaryAlarm->alarmText = alarm->alarmText;	
	AlarmControlActivePrimaryAlarm->alarmStreamName = alarm -> alarmStreamName;
	AlarmControlActivePrimaryAlarm->alarmTime = alarm->alarmTime; //Fix here
	//memcpy(AlarmControlActivePrimaryAlarm, alarm, sizeof(_alarm));
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
	 printf("AlarmA T:\"%s\"S:\"%s\" %02d:%02d:%02d \n",AlarmControlActivePrimaryAlarm->alarmText, AlarmControlActivePrimaryAlarm->alarmStreamName, AlarmTime -> tm_hour, AlarmTime -> tm_min, AlarmTime -> tm_sec);
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
	alarmPlayingFlag = 0;
	alarmEventFlag = 1; //Raise ethernet's alarm flag
	UIchangeState(UISTATE_ALARMEVENT);
    printf("Alarm!\n");
}
