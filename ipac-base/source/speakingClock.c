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
				printf("\nsay The Time Is\n");
			break;
			case 1:
				sayClockNumber(parsedHours);
				printf("say hour\n");
			break;
			case 2:
				sayClockNumber(parsedMinutes);
				printf("say minutes\n");
			break;
			case 3:
				sayClockHoursTextAmPm(parsedHours);
				printf("say AM or PM");
			break;
			default:
			break;
		}
		++sequence;
	}
	return 0;
}

///// array van links naar de server, sequentiueel ophalen en aflaten spelen

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
	char *displayString = malloc(strlen("sayClockNumber "+3));
	sprintf(displayString, sprintf("sayClockNumber %d", number));
    LcdWriteString(displayString, strlen("sayClockNumber ")+3);
    free(displayString);
	//TODO: add speak method "number"
	return 0;
}

int sayClockHoursTextAmPm( int hour ){
    if (hour >= 12){
    	LcdWriteString("It is PM", strlen("It is PM")+1);
	//TODO: add speak method "PM"
    } else {
    	LcdWriteString("It is AM", strlen("It is AM")+1);
	//TODO: add speak method "AM"
    }
	return 0;
} 
