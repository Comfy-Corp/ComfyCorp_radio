/*******************************************************
 *
 * Alarm control center
 * Use to manipulate alarms and keep tabs on active alarms.
 *
 *******************************************************/
#include "rtc.h"

void AlarmControlTestProcedure(){
	int params;
	AlarmControlCreateAlarm(&params); //Set alarm to 5 seconds.
	while(AlarmControlCheck() != RTC_STATUS_AL0){
		NutDelay(10);
	}	
}

//params: no idea what to do yet, placeholder
//Ideas: Time of alarm, alarm type
void AlarmControlCreateAlarm(void *params){
	 
	 X12RtcSetAlarm(0, CONST struct _tm *tm, int aflgs)
}

//
void AlarmControlGetActiveAlarm(){

}

u_long AlarmControlCheck(){
	u_long result;
	X12RtcGetStatus(&result);
	if(result == RTC_STATUS_AL0)
		AlarmControlAlarmEvent();
	return  result; //Possibly not neccessary
}

//params: no idea what to do here either
//ideas 
void AlarmControlAlarmEvent(){
	printf("Alarm!");
}