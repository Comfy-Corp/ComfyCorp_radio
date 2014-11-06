/*! \mainpage SIR firmware documentation
 *
 *  \section intro Introduction
 *  A collection of HTML-files has been generated using the documentation in the sourcefiles to
 *  allow the developer to browse through the technical documentation of this project.
 *  \par
 *  \note these HTML files are automatically generated (using DoxyGen) and all modifications in the
 *  documentation should be done via the sourcefiles.
 */

/*! \file
 *  COPYRIGHT (C) STREAMIT BV 2010
 *  \date 19 december 2003
 */




#define LOG_MODULE  LOG_MAIN_MODULE

/*--------------------------------------------------------------------------*/
/*  Include files :)                                                        */
/*--------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>

#include "thread.h"
#include <sys/timer.h>
#include <sys/version.h>
#include <sys/osdebug.h>

#include "system.h"
#include "portio.h"
#include "lcd.h"
#include "alarmControl.h"
#include "remcon.h"
#include "keyboard.h"
#include "led.h"
#include "log.h"
#include "uart0driver.h"
#include "mmc.h"
#include "watchdog.h"
//#include "flash.h"
#include "spidrv.h"
#include "dispatcher.h"

#include <time.h>
#include "rtc.h"
#include "ui.h"

#include "storage.h"

#include "inet.h"
#include "player.h"

#include "ethernet.h"

/*-------------------------------------------------------------------------*/
/* global variable definitions                                             */
/*-------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------*/
/* local variable definitions                                              */
/*-------------------------------------------------------------------------*/
u_char isLoading = 0;


/*-------------------------------------------------------------------------*/
/* local routines (prototyping)                                            */
/*-------------------------------------------------------------------------*/
void changeMenuItem(char);

/*-------------------------------------------------------------------------*/
/* Stack check variables placed in .noinit section                         */
/*-------------------------------------------------------------------------*/

/*!
 * \addtogroup System
 */

/*@{*/


/*-------------------------------------------------------------------------*/
/*                         start of code                                   */
/*-------------------------------------------------------------------------*/





/* ÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃ */
/*!
 * \brief Initialise Digital IO
 *  init inputs to '0', outputs to '1' (DDRxn='0' or '1')
 *
 *  Pull-ups are enabled when the pin is set to input (DDRxn='0') and then a '1'
 *  is written to the pin (PORTxn='1')
 */
/* ÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃ */
void SysInitIO(void)
{
    /*
     *  Port B:     VS1011, MMC CS/WP, SPI
     *  output:     all, except b3 (SPI Master In)
     *  input:      SPI Master In
     *  pull-up:    none
     */
    outp(0xF7, DDRB);

    /*
     *  Port C:     Address bus
     */

    /*
     *  Port D:     LCD_data, Keypad Col 2 & Col 3, SDA & SCL (TWI)
     *  output:     Keyboard colums 2 & 3
     *  input:      LCD_data, SDA, SCL (TWI)
     *  pull-up:    LCD_data, SDA & SCL
     */
    outp(0x0C, DDRD);
    outp((inp(PORTD) & 0x0C) | 0xF3, PORTD);

    /*
     *  Port E:     CS Flash, VS1011 (DREQ), RTL8019, LCD BL/Enable, IR, USB Rx/Tx
     *  output:     CS Flash, LCD BL/Enable, USB Tx
     *  input:      VS1011 (DREQ), RTL8019, IR
     *  pull-up:    USB Rx
     */
    outp(0x8E, DDRE);
    outp((inp(PORTE) & 0x8E) | 0x01, PORTE);

    /*
     *  Port F:     Keyboard_Rows, JTAG-connector, LED, LCD RS/RW, MCC-detect
     *  output:     LCD RS/RW, LED
     *  input:      Keyboard_Rows, MCC-detect
     *  pull-up:    Keyboard_Rows, MCC-detect
     *  note:       Key row 0 & 1 are shared with JTAG TCK/TMS. Cannot be used concurrent
     */
#ifndef USE_JTAG
    sbi(JTAG_REG, JTD); // disable JTAG interface to be able to use all key-rows
    sbi(JTAG_REG, JTD); // do it 2 times - according to requirements ATMEGA128 datasheet: see page 256
#endif //USE_JTAG

    outp(0x0E, DDRF);
    outp((inp(PORTF) & 0x0E) | 0xF1, PORTF);

    /*
     *  Port G:     Keyboard_cols, Bus_control
     *  output:     Keyboard_cols
     *  input:      Bus Control (internal control)
     *  pull-up:    none
     */
    outp(0x18, DDRG);
}

/*!
 * \brief Thread listening for serial communication
 *
 * this thread has a for(;;) loop in which it listens for incoming communication
 * from the Uart interface.
 *
 * \return \b never returns
 */

THREAD(SettingsPolling, arg)
{
    for(;;)
    {
        
        if(alarmEventFlag){
            printf("Starting alarm request\n");
            NutSleep(5000);
            GetStreamURL();
            size_t item = NutThreadStackAvailable("Uart");
            printf("%d mems\n", item);
            alarmEventFlag = 0;
            alarmPlayingFlag = 1;
        } 
        else if(alarmPlayingFlag) 
        {
            printf("Poll thread sleep\n");
            NutSleep(1500);
        }

        else if (!isPlaying())
        {
            NutSleep(500);
            GetSettingsHTTP();
            
        }
        else
        {
            NutSleep(1000);
            GetSettingsHTTP();
        }

    }
}

THREAD(UartListener, arg)
{
    FILE *stream = Uart0DriverGetStream();
    for (;;)
    {
        while(isLoading){
            LcdLoading(0,1);
        }

        //fputs("henk", stream);
        char *str = malloc(81 * sizeof(char));
        fgets(str, 81 * sizeof(char), stream);
        printf("%s\n", str);
        str[strlen(str) - 1] = 0;
        if (strcmp("reset", str) == 0)
        {
            WatchDogEnable();
            WatchDogStart(30);
            for (;;) {};
        }
        else if (strncmp("s", str, 1) == 0)
        {
            char *strIP = malloc(16 * sizeof(char));
            printf("%s\n", str);
            int i;
            for (i=-1;i < 17; ++i)
            {
                strIP[i-1] = str[i];
            }
            printf("%s s\n",strIP );
            FILE* stream = GetHTTPRawStream(strIP);
            initPlayer();
            //int playResult = 
            play(stream);
        }
        else if (strncmp("x", str, 1) == 0)
        {
            char *strIP = malloc(80 * sizeof(char));
            printf("%s x\n", str);
            int i;
            for (i=-1;i < 81; ++i)
            {
                strIP[i-1] = str[i];
                /*if (str[i] == 0)
                {
                    break;
                }*/
            }
            if (isPlaying())
            {
                setPlaying(0);
                NutSleep(1500);
            }
            printf("%s test\n",strIP );

            FILE* stream = NULL;
            GetHTTPRawStreamWithAddress(strIP, stream);
            size_t item = NutThreadStackAvailable("Uart");
            printf("%d mems\n", item);
            initPlayer();
            puts((char *)stream);
            //int playResult = 
            play(stream);
        }
        else if (strncmp("q", str, 1) == 0)
        {

            //puts(stream);
            
        }
        else if (strncmp("t", str, 1) == 0)
        {
            FILE* strout = Uart0DriverGetStream();
            NutDumpThreadList(strout);
        }
        free(str);
    }
}

/* ÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃ */
/*!
 * \brief Main entry of the SIR firmware
 *
 * All the initialisations before entering the for(;;) loop are done BEFORE
 * the first key is ever pressed. So when entering the Setup (POWER + VOLMIN) some
 * initialisatons need to be done again when leaving the Setup because new values
 * might be current now
 *
 * \return \b never returns
 */
/* ÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃÃ */
int main(void)
{
    /*
     *  First disable the watchdog
     */
    WatchDogDisable();

    NutDelay(100);

    NutTimerInit();

    SysInitIO();

    SPIinit();

    LedInit();

    LcdLowLevelInit();
    LcdClear();

    Uart0DriverInit();
    Uart0DriverStart();
    LogInit();
    LogMsg_P(LOG_INFO, PSTR("Hello World and Jim\n"));

    CardInit();

    /*
     * Kroeske: sources in rtc.c en rtc.h
     */
    X12Init();

    RcInit();

    KbInit(); 

    SysControlMainBeat(ON);             // enable 4.4 msecs hartbeat interrupt

    /*
     * Increase our priority so we can feed the watchdog.
     */
    NutThreadSetPriority(1);
    WatchDogDisable();
    isLoading = 1;
    NutThreadCreate("Uart", UartListener, NULL, 512);
    /* Enable global interrupts */
    sei();
    //LcdChar(32);
    LcdClear();
    LcdSetCursor(0x01);
    LcdWriteString("Connecting", strlen("Connecting")+1);
    int ethWork = ethInitInet();
    if (!ethWork)
        ethGetNTPTime();
    else
        printf("%s\n", "network setup failed");

    _StorableSetting timezone;
    StorageLoadConfig(&timezone);
    printf("Current timezone: %d\n", timezone.timezone); 
    //PRINT TIME ON DISPLAY
    char timeBuffer[9];
    //char *timeBuffer = malloc(sizeof(char) * 20);

    X12FillStringWithTime(timeBuffer);
    //LcdWriteString(timeBuffer,sizeof(timeBuffer));
    isLoading = 0; 
    u_char kb_start_up = KbGetKey();
    if(kb_start_up == KEY_ESC)
    {
        UIchangeState(UISTATE_SHOWRESET);
        while(getStateChar() == UISTATE_SHOWRESET || getStateChar() == UISTATE_SHOWSETUP)
        {
            int kb_error = KbWaitForKeyEvent(50/3);
            LcdRefreshCallback();
            UIHandleInput(kb_error);
        }
    }
        LcdClear();
        FILE* webstream = NULL;
        GetHTTPRawStreamWithAddress("37.46.136.205:80/startup.mp3", webstream);
        initPlayer();
        //int playResult = 
        play(webstream);
        LcdHelloAnimation();
        setPlaying(0);
        UIchangeState(UISTATE_SHOWTIME);
    //AlarmControlInit();

    /*
     *Cycle time for the main loop. Divide over all blocking/waiting methods.
     *Preferably there could be an array of functions so that we can determine 
     *the amount of actions dynamically and execute each wait variably to match
     *the total time for a cycle. 
     *e.g. actionsCount = 2 -> method waits: cycleTime/actionsCount
    */

    int cycleTime = 100;

    NutThreadCreate("Polling", SettingsPolling, NULL, 512);
    for (;;)
    {
        LcdRefreshCallback();
        NutSleep(cycleTime/3);
        int kb_error = KbWaitForKeyEvent(cycleTime/3);
        UIHandleInput(kb_error);
        AlarmControlCheck();
        NutSleep(cycleTime/3);
        if(AlarmControlActivePrimaryAlarm == NULL)
            GetAlarmsHTTP();
            //GetAlarmsHTTP("37.46.136.205/alarms"); 
    }
    
    return (0);     // never reached, but 'main()' returns a non-void, so.....
}
/* ---------- end of module ------------------------------------------------ */

/*@}*/
