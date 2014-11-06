/*********************************************************
 *
 *Module for custom character animations and definitions.
 *
 *For use only in Lcd module.
 *
 *********************************************************/
#include <stdio.h>
#include <string.h>
#include <sys/timer.h>

#include "lcd.h"
#include "customChars.h"

u_char loadingTicks = 0; //Could be replaced with a global tick system, 
						 //so animations can be interrupted. For now: only used in loading.

const u_char forwardSlash[] = {
	0x10,
	0x08,
	0x04,
	0x02,
	0x01,
	0x00,
	0x00,
	0x00
};

const u_char alarmChar[] = { //Stolen from http://www.quinapalus.com/hd44780udg.html
	0x04,
	0x0e,
	0x0e,
	0x0e,
	0x1f,
	0x00,
	0x04,
	0x00
};

//Custom char definitions
const u_char animationA[] = {
	0x00,  // 00000
	0x00,  // 00000
	0x00,  // 00000
	0x00,  // 00000
	0x00,  // 00000
	0x00,  // 00000
	0x10,  // 10000
	0x00   // 00000
};

const u_char animationB[] = {
	0x00,
	0x00,
	0x00,
	0x00,
	0x00,
	0x10,
	0x08,
	0x00
};
	
const u_char animationC[] = {
	0x00,
	0x10,
	0x10,
	0x10,
	0x10,
	0x10,
	0x18,
	0x10
};
	
const u_char animationD[] = {
	0x00,
	0x18,
	0x18,
	0x18,
	0x08,
	0x08,
	0x18,
	0x08
};

const u_char animationE[] = {
	0x00,
	0x0C,
	0x0C,
	0x0C,
	0x04,
	0x0C,
	0x16,
	0x04
};
	
const u_char animationF[] = {
	0x00,
	0x0C,
	0x0C,
	0x0C,
	0x04,
	0x04,
	0x0C,
	0x04
};
	
const u_char animationG[] = {
	0x00,
	0x0C,
	0x0C,
	0x0C,
	0x04,
	0x04,
	0x04,
	0x04
};

const u_char animationH[] = {
	0x00,
	0x0E,
	0x0C,
	0x0E,
	0x04,
	0x0E,
	0x04,
	0x04	
};
	
const u_char animationI[] = { //Reusable stick figure frame, in case the animation looks as stupid as I think.
	0x00, // 00000
	0x0E, // 01110
	0x0A, // 01010
	0x0E, // 01110
	0x04, // 00100
	0x0E, // 01110
	0x15, // 10101
	0x04  // 00100
};

const u_char animationJ[] = {
	0x00,
	0x0E,
	0x0A,
	0x0E,
	0x05,
	0x0E,
	0x14,
	0x04
};
	//Repeat I

const u_char animationK[] = {
	0x00,
	0x0E,
	0x06,
	0x0E,
	0x04,
	0x0E,
	0x04,
	0x04
};
	

const u_char animationL[] = {
	0x00,
	0x06,
	0x06,
	0x06,
	0x04,
	0x04,
	0x04,
	0x04
};
	
const u_char animationM[] = {
	0x00,
	0x06,
	0x06,
	0x06,
	0x04,
	0x04,
	0x0C,
	0x04
};

const u_char animationN[] = {
	0x00,
	0x0C,
	0x0C,
	0x0C,
	0x08,
	0x18,
	0x0C,
	0x08
}; 

const u_char animationO[] = {
	0x00,
	0x18,
	0x18,
	0x18,
	0x10,
	0x18,
	0x14,
	0x10
};

const u_char animationP[] = {
	0x00,
	0x10,
	0x10,
	0x10,
	0x00,
	0x00,
	0x10,
	0x00
};

void CustomCharsAnimation(){
	unsigned int animationTiming = 200;  
	unsigned char waves = 1;			  //How many hearty hello's must mister Lcd bellow.
	int i;
	LcdCommand(0x01);
	LcdCommand(0xC0);
	LcdChar(0x00);
	LcdDefinePattern(animationA, 0);
	NutSleep(animationTiming);
	LcdDefinePattern(animationB, 0);
	NutSleep(animationTiming);
	LcdDefinePattern(animationC, 0);
	NutSleep(animationTiming);
	LcdDefinePattern(animationD, 0);
	NutSleep(animationTiming);
	LcdDefinePattern(animationE, 0);
	NutSleep(animationTiming);
	LcdDefinePattern(animationF, 0);
	NutSleep(animationTiming);
	LcdDefinePattern(animationG, 0);
	NutSleep(animationTiming);
	LcdDefinePattern(animationH, 0);
	NutSleep(animationTiming);
	for(i=0;i<waves;++i)
	{
		LcdDefinePattern(animationJ, 0);
		NutSleep(animationTiming);
		LcdDefinePattern(animationI, 0);
		LcdCommand(0x84);
		LcdChar('H');
		LcdChar('e');
		LcdChar('l');
		LcdChar('l');
		LcdChar('o');
		LcdChar('!');
		LcdDefinePattern(animationJ, 0);
		NutSleep(animationTiming *3);
		LcdCommand(0x01);
		LcdCommand(0xC0);
		LcdChar(0x00);
		LcdDefinePattern(animationI, 0);
		NutSleep(animationTiming *3);
	}
	LcdDefinePattern(animationK, 0);
	NutSleep(animationTiming);
	LcdDefinePattern(animationL, 0);
	NutSleep(animationTiming);
	LcdDefinePattern(animationM, 0);
	NutSleep(animationTiming);
	LcdDefinePattern(animationN, 0);
	NutSleep(animationTiming);
	LcdDefinePattern(animationO, 0);
	NutSleep(animationTiming);
	LcdDefinePattern(animationP, 0);
	NutSleep(animationTiming);
	LcdCommand(0x01);
	NutSleep(animationTiming);	
}

void CustomCharsAlarmIcon(u_char pos){
	LcdDefinePattern(alarmChar, 2);
    LcdSetCursor(pos);
    LcdChar(0x02);
}

void CustomCharsLoading(u_char pos, u_char isLast){
	LcdDefinePattern(forwardSlash, 1);
	LcdSetCursor(pos);
	if(loadingTicks == 0)
		LcdChar('/');
	else if(loadingTicks == 1)
		LcdChar('-');	
	else if(loadingTicks == 2) 
		LcdChar(0x01);	
	else if (loadingTicks == 3)		
		LcdChar('|');		
	NutSleep(100);
	if(isLast)
	{
		loadingTicks++;
		loadingTicks%=4;
	}	
}
