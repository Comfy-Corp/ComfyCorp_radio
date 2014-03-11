/*******************************************************
 *
 * Alarm control center
 * Use to manipulate alarms and keep tabs on active alarms.
 *
 *******************************************************/
#include <time.h>
#include "rtc.h"
#include "ui.h"
#include "alarmControl.h"

void AlarmControlTestProcedure(void){
	X12RtcClearStatus(0x30);
	tm testTime;
	X12RtcGetClock(&testTime);
	testTime.tm_min += 2;
	testTime.tm_min %= 60;
	struct _alarm testAlarm;
	testAlarm.alarmText = "Wake up!";
	testAlarm.alarmStreamName = "Kiss FM";
	testAlarm.alarmType = 0; //Primary
	testAlarm.alarmTime = &testTime;
	AlarmControlCreateDaylyAlarm(testAlarm); //Set alarm to 5 seconds.
}

void AlarmControlSnoozePrimary(){
	tm now;
	X12RtcGetClock(&now);
	now.tm_min += 5;
	now.tm_min %= 60;
	struct _alarm snoozeAlarm;
	snoozeAlarm.alarmText = "Snooze";
	snoozeAlarm.alarmStreamName = "Kiss FM";
	snoozeAlarm.alarmType = 0; //Primary
	snoozeAlarm.alarmTime = &now;
	AlarmControlCreateDaylyAlarm(snoozeAlarm); //Set alarm to 5 seconds.
}

//Compares Sec, Min & Hour default alarm slot 0
void AlarmControlCreateDaylyAlarm(struct _alarm alarm){
	if(AlarmControlActivePrimaryAlarm != NULL)
		free(AlarmControlActivePrimaryAlarm);
	AlarmControlActivePrimaryAlarm = malloc(sizeof(_alarm));
	*AlarmControlActivePrimaryAlarm = alarm;
	AlarmControlPrintActiveAlarm();
	X12RtcSetAlarm(0, alarm.alarmTime, 0x06); //Checks on HH:MM
}

void AlarmControlRemoveDaylyAlarm(){
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
	 printf("AlarmA: %02d:%02d:%02d \n", AlarmTime -> tm_hour, AlarmTime -> tm_min, AlarmTime -> tm_sec);
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
	UIchangeState(UISTATE_ALARMEVENT);
	printf("Alarm!\n");
}
