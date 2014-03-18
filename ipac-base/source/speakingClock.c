#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lcd.h"
#include "player.h"
#include "ethernet.h"

int doSpeakingClockSequence(char stringToParse[]){
	int sequence = 0;

	int parsedHours;
	int parsedMinutes;

	parseTimeClock(stringToParse, &parsedHours, &parsedMinutes);

	doSpeakingClockSample();

	while(sequence <= 3){
		switch(sequence){
			case 0:
				sayTheTimeIs();
				printf("\nsay The Time Is\n");
			break;
			case 1:
				sayClockNumber(parsedHours);
				printf("say %d\n", parsedHours);
			break;
			case 2:
				sayClockNumber(parsedMinutes);
				printf("say %d\n", parsedMinutes);
			break;
			case 3:
				sayClockHoursTextAmPm(parsedHours);
				printf("say AM or PM\n");
			break;
			default:
			break;
		}
		++sequence;
	}
	return 0;
}

void doSpeakingClockSample(){
	printf("Proof of concept\n");
	if (isPlaying()){
		setPlaying(0);
	}
	FILE *stream = GetHTTPRawStreamWithAddress("37.46.136.205:80/audiofiles/proofOfConceptSpeakingClock.mp3");
	initPlayer();
	int PlayerResult = play(stream);
	LcdWriteString("Proof of concept", strlen("Proof of concept")+1);
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

void sayTheTimeIs(){
	//TODO: add speak method "The time is "
	LcdWriteString("sayTheTimeIs", strlen("sayTheTimeIs")+1);
}

void sayClockNumber( int number ){
    LcdWriteString("sayClockNumber %d", number, strlen("sayClockNumber ")+3);
	//TODO: add speak method "number"
}

void sayClockHoursTextAmPm( int hour ){
    if (hour >= 12){
    	LcdWriteString("It is PM", strlen("It is PM")+1);
	//TODO: add speak method "PM"
    } else {
    	LcdWriteString("It is AM", strlen("It is AM")+1);
	//TODO: add speak method "AM"
    }
} 
