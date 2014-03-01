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
	tm testAlarm;
	X12RtcGetClock(&testAlarm);
	testAlarm.tm_min += 5;
	testAlarm.tm_min %= 60;
	AlarmControlCreatePrimaryAlarm(testAlarm); //Set alarm to 5 seconds.
}

//Compares Sec, Min & Hour default alarm slot 0
void AlarmControlCreatePrimaryAlarm(tm AlarmTime){
	 X12RtcSetAlarm(0, &AlarmTime, 0x06);
}

void AlarmControlCreateSecondaryAlarm(tm AlarmTime){
	X12RtcSetAlarm(1, &AlarmTime, 0x06);	
}

void AlarmControlPrintActiveAlarm(){
	 tm AlarmTime;
	 int AlarmFlags;
	 X12RtcGetAlarm(0, &AlarmTime, &AlarmFlags);
	 printf("AlarmA: %02d:%02d:%02d \n", AlarmTime.tm_hour, AlarmTime.tm_min, AlarmTime.tm_sec);
	 printf("FlagsA: %x \n", AlarmFlags);
}

u_long AlarmControlCheck(){
	u_long result;
	X12RtcGetStatus(&result);
	if(result == RTC_STATUS_AL0){
		AlarmControlAlarmEvent();
		X12RtcClearStatus(0x30);
	}
	return  result; //Possibly not neccessary
}

//params: no idea what to do here either
//ideas 
void AlarmControlAlarmEvent(){
	UIchangeState(UISTATE_ALARMEVENT);
	printf("Alarm!\n");
}