#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lcd.h"

int doSpeakingClockSequence(char stringToParse[]){
	int sequence = 0;

	int parsedHours;
	int parsedMinutes;

	parseTimeClock(stringToParse, &parsedHours, &parsedMinutes);

	while(sequence > 3){
		switch(sequence){
			case 0:
				sayTheTimeIs();
			break;
			case 1:
				sayClockNumber();
			break;
			case 2:
				sayClockNumber();
			break;
			case 3:
				sayClockHoursTextAmPm();
			break;
			default:
			break;
		}
		++sequence;
	}
}


int parseTimeClock(char stringToParse[], int *hours, int *minutes){
	if (strlen(stringToParse) < 4)
		return -1;
	char parsedHoursChar[2];
	char parsedMinutesChar[2];

	parsedHoursChar[0] = stringToParse[0];
	parsedHoursChar[1] = stringToParse[1];

	parsedMinutesChar[0] = stringToParse[3];
	parsedMinutesChar[1] = stringToParse[4];

	hours = atoi(parsedHoursChar);
	minutes = atoi(parsedHoursChar);

	return 0;
}

int sayTheTimeIs( void ){
	//TODO: add speak method "The time is "
}

int sayClockNumber( int number ){
    LcdWriteString("sayClockNumber %d", number, strlen("sayClockNumber %d", number)+1);

	//TODO: add speak method "number"
	return 0;
}

int sayClockHoursTextAmPm( int hour ){
    
    int hourOfTheDay = atio(hour);

    if (hour >= 12){
    	LcdWriteString("It is PM", strlen("It is PM")+1);
	//TODO: add speak method "PM"
    } else {
    	LcdWriteString("It is AM", strlen("It is AM")+1);
	//TODO: add speak method "AM"
    }
	return 0;
} 
