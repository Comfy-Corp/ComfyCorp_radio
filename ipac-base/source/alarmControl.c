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
	testTime.tm_min += 1;
	testTime.tm_min %= 60;
	struct _alarm testAlarm;
	testAlarm.alarmText = "TestAlarm";
	testAlarm.streamName = "Kiss FM";
	testAlarm.alarmType = 0; //Primary
	testAlarm.alarmTime = testTime;
	AlarmControlCreateDaylyAlarm(testAlarm); //Set alarm to 5 seconds.
}

//Compares Sec, Min & Hour default alarm slot 0
void AlarmControlCreateDaylyAlarm(struct _alarm alarm){
	AlarmControlActivePrimaryAlarm = alarm;
	 X12RtcSetAlarm(0, &alarm.alarmTime, 0x06); //Checks on HH:MM
}

void AlarmControlCreateYearlyAlarm(struct _alarm alarm){
	AlarmControlActiveSecondaryAlarm = alarm;	
	X12RtcSetAlarm(1, &alarm.alarmTime, 0x1E); //Checks on Month + Day of Month + HH:MM. Requires alarmTime to contain these.
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
	if(result == RTC_STATUS_AL0){ //Primary alarm (Dayly)
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
