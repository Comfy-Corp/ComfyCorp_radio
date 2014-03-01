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

#include "Lcd.h"
#include "customChars.h"

u_char loadingTicks = 0; //Could be replaced with a global tick system, so animations van be interrupted. For now: only used in loading.
 
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

void CustomCharsLoading(u_char pos){
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
	loadingTicks++;
	loadingTicks%=4;
}
