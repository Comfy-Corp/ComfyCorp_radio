/* ========================================================================
 * [PROJECT]    SIR100
 * [MODULE]     Display
 * [TITLE]      lcd header file
 * [FILE]       lcd.h
 * [VSN]        1.0
 * [CREATED]    030414
 * [LASTCHNGD]  030414
 * [COPYRIGHT]  Copyright (C) STREAMIT BV 2010
 * [PURPOSE]    API and gobal defines for lcd module
 * ======================================================================== */

#ifndef _Display_H
#define _Display_H


/*-------------------------------------------------------------------------*/
/* global defines                                                          */
/*-------------------------------------------------------------------------*/
#define DISPLAY_SIZE                16
#define NROF_LINES                  2
#define MAX_SCREEN_CHARS            (NROF_LINES*DISPLAY_SIZE)

#define LINE_0                      0
#define LINE_1                      1

#define FIRSTPOS_LINE_0             0
#define FIRSTPOS_LINE_1             0x40


#define LCD_BACKLIGHT_ON            1
#define LCD_BACKLIGHT_OFF           0

#define ALL_ZERO                    0x00      // 0000 0000 B
#define WRITE_COMMAND               0x02      // 0000 0010 B
#define WRITE_DATA                  0x03      // 0000 0011 B
#define READ_COMMAND                0x04      // 0000 0100 B
#define READ_DATA                   0x06      // 0000 0110 B


/*-------------------------------------------------------------------------*/
/* typedefs & structs                                                      */
/*-------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
/*  Global variables                                                        */
/*--------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------*/
/* export global routines (interface)                                      */
/*-------------------------------------------------------------------------*/
extern void LcdChar(char);
extern void LcdWriteUchar(u_char, u_char);
extern void LcdBackLight(u_char);
extern void LcdSetCursor(u_char);
extern void LcdBackLightBriefOn(u_char);
extern void LcdCommand (u_char);
extern void LcdHelloAnimation(void);
extern void LcdLoading(u_char, u_char);
extern void LcdAlarmIcon(u_char);
extern void LcdWriteString(char[], int);
extern void LcdWriteStringAtLoc(char[], int, int);
extern void LcdDefinePattern(const u_char[], u_char);
extern void LcdClear(void);
extern void LcdRefreshCallback(void);
extern void LcdLowLevelInit(void);

#endif /* _Display_H */
/*  様様  End Of File  様様様様 様様様様様様様様様様様様様様様様様様様様様様 */
