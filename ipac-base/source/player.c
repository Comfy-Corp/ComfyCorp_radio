
///
#define LOG_MODULE  LOG_PLAYER_MODULE

#include <sys/heap.h>
#include <sys/bankmem.h>
#include <sys/thread.h>
#include <sys/timer.h>

#include "player.h"
#include "vs10xx.h"


#define OK			1
#define NOK			0

THREAD(StreamPlayer, arg);

int initPlayer(void)
{
	return OK;
}

int play(FILE *stream)
{
	NutThreadCreate("Bg", StreamPlayer, stream, 512);
	printf("Play thread created. Device is playing stream now !\n");

	
	return OK;
}

THREAD(StreamPlayer, arg)
{
	FILE *stream = (FILE *) arg;
	size_t rbytes = 0;
	char *mp3buf;
	int result = NOK;
	int nrBytesRead = 0;
	unsigned char iflag;
	
	//
	// Init MP3 buffer. NutSegBuf is een globale, systeem buffer
	//
	if( 0 != NutSegBufInit(8192) )
	{
		printf("%s\n", "opening buffer");
		// Reset global buffer
		iflag = VsPlayerInterrupts(0);
		NutSegBufReset();
		VsPlayerInterrupts(iflag);
		
		result = OK;
	}
	
	// Init the Vs1003b hardware
	if( OK == result )
	{
		if( -1 == VsPlayerInit() )
		{
			if( -1 == VsPlayerReset(0) )
			{
				result = NOK;
			}
		}
	}
	printf("%s\n", "mp3 decoder initialized");
	
	for(;;)
	{
		printf("%s\n", "querying some bytes");
		/*
		 * Query number of byte available in MP3 buffer.
		 */
        iflag = VsPlayerInterrupts(0);
        printf("%s\n", "if flag set");
        mp3buf = NutSegBufWriteRequest(&rbytes);
        printf("%s\n", "filled mp3buf");
        VsPlayerInterrupts(iflag);
        printf("%s\n", "VS_PLAYER_INTERRUPT set");
		
		// Bij de eerste keer: als player niet draait maak player wakker (kickit)
		if( VS_STATUS_RUNNING != VsGetStatus() )
		{
			if( rbytes < 1024 )
			{
				printf("VsPlayerKick()\n");
				VsPlayerKick();
			}
		}
		else
		{
			printf("%s\n", "Player is awake");
		}
		
		while( rbytes )
		{
			printf("I am in ze while\n");
			// Copy rbytes (van 1 byte) van stream naar mp3buf.
			nrBytesRead = fread(mp3buf,1,5,stream);
			printf("LOL\n");
			
			if( nrBytesRead > 0 )
			{
				printf("nrBytesRead thingy entered");
				iflag = VsPlayerInterrupts(0);
				printf("nrBytesRead  first thingy\n");
				mp3buf = NutSegBufWriteCommit(nrBytesRead);
				printf("nrBytesRead  second thingy\n");
				VsPlayerInterrupts(iflag);
				printf("nrBytesRead  third thingy\n");
				if( nrBytesRead < rbytes && nrBytesRead < 512 )
				{
					printf("nrBytesRead if thingy\n");
					NutSleep(250);
				}

			}
			else
			{
				printf("leaving while because bytes <0\n");
				break;
			}
			rbytes -= nrBytesRead;
			
			if( nrBytesRead <= 0 )
			{
				printf("Leaving while because bytes <=0\n");
				break;
			}				
		}
	}
}

