#include "thread.h"
#include <sys/timer.h>
#include <dev/nicrtl.h>
#include <arpa/inet.h>
#include <sys/confnet.h>
#include <pro/dhcp.h>
#include <time.h>
#include "ethernet.h"
#include <sys/socket.h>
#include <dev/board.h>
#include <net/route.h>
#include <pro/sntp.h>
#include <stdlib.h>
#include <stdio.h>
#include <io.h>
#include <string.h>
#include <dev/vs1001k.h>
#include <sys/version.h>
#include "heap.h"
#include "bankmem.h"
#include <netinet/tcp.h>
#include "lcd.h"
#include "ethernet.h"
#include <errno.h>
#include "player.h"
#include "led.h"
#include "alarmControl.h"

#define NOK 1
#define OK 0

TCPSOCKET *sock;
char* streamURLCurrent;
	
int ethInitInet(void)
{
	//uint8_t mac_addr[6] = { 0xC0, 0x01, 0x1E, 0x01, 0x02, 0x03 };
	uint8_t mac_addr[6] = { 0x00, 0x0f, 0xFE, 0x74, 0x13, 0x8D };

	
	int result = OK;

	// Registreer NIC device (located in nictrl.h)
	if( NutRegisterDevice(&DEV_ETHER, 0x8300, 5) )
	{ 
		printf("%s\n", "Error: >> NutRegisterDevice()");
		result = NOK;
	}
	
	if( OK == result )
	{
		if( NutDhcpIfConfig(DEV_ETHER_NAME, mac_addr, 0) )
		{
			printf("%s\n", "Error: >> NutDhcpIfConfig()");
			result = NOK;
		}
	}
	if( OK == result )
	{
		printf("%s\nif_name: %s\nip-addr: %s\nip-mask: %s\ngw     : %s\n",
			"Networking setup OK, new settings are:",
			confnet.cd_name,
			inet_ntoa(confnet.cdn_ip_addr),
			inet_ntoa(confnet.cdn_ip_mask),
			inet_ntoa(confnet.cdn_gateway));
	}
	//NutSleep(1000);
	streamURLCurrent = calloc(1,100);
	return result;
}

int ethGetNTPTime()
{
	//Tijdelijke lokale tijdzone, LET OP, GMT +1 wordt in de code -1!!
	_timezone = -1 *60*60;
	time_t ntp_time = 0;
	tm *ntp_datetime;
	uint32_t timeserver = 0;
	/* Retrieve time from the "pool.ntp.org" server. This is a free NTP server. */
    puts("Retrieving time from 0.nl.pool.ntp.org...");
 
    timeserver = inet_addr("37.139.24.95");
 
    for (;;) {
        if (NutSNTPGetTime(&timeserver, &ntp_time) == 0) {
            break;
        } else {
            NutSleep(1000);
            puts("Failed to retrieve time. Retrying...");
        }
    }
    puts("Done.\n");
 
    ntp_datetime = localtime(&ntp_time);
 	
    printf("NTP time is: %02d:%02d:%02d\n", ntp_datetime->tm_hour, ntp_datetime->tm_min, ntp_datetime->tm_sec);
    X12RtcSetClock(ntp_datetime);
    return 0;
}

FILE* GetHTTPRawStream(char* ip)
{

	int result = OK;
	char *data;
	
	if (sock != NULL)
	{
		NutTcpCloseSocket(sock);
	}
	sock = NutTcpCreateSocket();
	if( NutTcpConnect(	sock,
						inet_addr(ip), 
						8000) )
	{
		printf("Error: >> NutTcpConnect()");
		exit(1);
	}
	stream = _fdopen( (int) sock, "r+b" );

	fprintf(stream, "GET %s HTTP/1.0\r\n", "/");
	fprintf(stream, "Host: %s\r\n", "62.212.132.54");
	fprintf(stream, "User-Agent: Ethernut\r\n");
	fprintf(stream, "Accept: */*\r\n");
	fprintf(stream, "Icy-MetaData: 1\r\n");
	fprintf(stream, "Connection: close\r\n\r\n");
	fflush(stream);

	
	// Server stuurt nu HTTP header terug, catch in buffer
	data = (char *) malloc(512 * sizeof(char));
	
	while( fgets(data, 512, stream) )
	{
		if( 0 == *data )
			break;
	}
	puts(data);
	
	free(data);

	return stream;
}

int connectToStream(void)
{
	int result = OK;
	char *data;
	
	if (sock != NULL)
	{
		NutTcpCloseSocket(sock);
	}
	
	sock = NutTcpCreateSocket();
	if( NutTcpConnect(	sock,
						inet_addr("192.168.1.142"), 
						8000) )
	{
		printf("Error: >> NutTcpConnect()");
		exit(1);
	}
	stream = _fdopen( (int) sock, "r+b" );
	//http://icecast.omroep.nl/radio4-bb-mp3 of http://icecast.omroep.nl/radio1-bb-mp3
	fprintf(stream, "GET %s HTTP/1.0\r\n", "/");
	fprintf(stream, "Host: %s\r\n", "62.212.132.54");
	fprintf(stream, "User-Agent: Ethernut\r\n");
	fprintf(stream, "Accept: */*\r\n");
	fprintf(stream, "Connection: close\r\n\r\n");
	fflush(stream);

	
	// Server stuurt nu HTTP header terug, catch in buffer
	data = (char *) malloc(512 * sizeof(char));
	
	while( fgets(data, 512, stream) )
	{
		if( 0 == *data )
			break;
	}
	
	free(data);

	return result;
}

int playStream(void)
{
	play(stream);
	
	return OK;
}

FILE* GetHTTPRawStreamWithAddress(char* netaddress)
{ 
	LcdBackLightBriefOn(100);
	int result = OK;
	char *data;
	if (sock != NULL)
	{
		NutTcpCloseSocket(sock);
	}
    sock = NutTcpCreateSocket();
	char* ip = malloc(23*sizeof(char));
	strncpy(ip, netaddress, 22);
	char nullTerm=0;
	int slashLoc=0;
	int colonLoc=0;
	int i;
	int port = 80;
	char* stringData;
	char* stringStreamNameLoc;
	streamName = "_";

	/*-- Finding the forward slash and port colon --*/
	for(i = 0;i<=23;++i)
	{
		if(ip[i]=='/')
		{
			ip[i]=0;
			nullTerm = 1;
			slashLoc = i;
			printf("slashLoc set to %d\n", slashLoc);
			break;
		}

		if(ip[i]==':')
		{
			colonLoc = i;
			printf("colonLoc set to %d\n", colonLoc);
		}

		else if (ip[i] ==0)
		{
			nullTerm=-1;
			break;
		}
	} 
	if (colonLoc)
	{
		ip[colonLoc] = 0;
		char* Sport = malloc((slashLoc - colonLoc)+2*sizeof(char));
		for (i=colonLoc+1;i<slashLoc;++i)
		{
			Sport[i-colonLoc-1] = netaddress[i];
		}
		Sport[slashLoc - colonLoc-1] = 0;
		
		port = atoi(Sport);
		free(Sport);
	}

	if (!nullTerm)
	{
		ip[17] = 0;
	}   
    //str[strlen(str) - 1] = 0;
    char* address = malloc(80*sizeof(char));
    memset(address, 0, sizeof(address));
    printf("connecting to ip %s\n", ip);
    if( NutTcpConnect(	sock,
						inet_addr(ip), 
						port) )
	{
		printf("Error: >> NutTcpConnect()");
	}
    else
    {
    	//free(ip);
        FILE *stream;
        if (nullTerm>0)
        {
	 		for (i=slashLoc;i < 79; ++i)
	        {
	            address[i-slashLoc] = netaddress[i];
	        }
    	}
    	else
    	{
    		address[0] = 0;
    	}	
    	//free(netaddress);
    	//printf("opening %s%s\n", ip, address);
        stream = _fdopen((int) sock, "r+b");
        if (stream == NULL)
        {
        	printf("%s\n", "STREAM IS NULL ABORT!");
        	printf("%d\n",errno );
        	exit(1);
        }
        printf("Address is: %s\n", address);
        fprintf(stream, "GET %s HTTP/1.0\r\n", address);
		fprintf(stream, "Host: %s\r\n", "62.212.132.54");
		fprintf(stream, "User-Agent: Ethernut\r\n");
		fprintf(stream, "Accept: */*\r\n");
		fprintf(stream, "Connection: close\r\n\r\n");
		fflush(stream);
		data = (char *) malloc(512 * sizeof(char));
		
		LcdClear();
		strncpy(streamName, "", 1);
		while( fgets(data, 512, stream) )
		{
			stringData = strstr(data, "icy-metaint:");
			stringStreamNameLoc = strstr(data, "icy-name:");
			if (strncmp(stringStreamNameLoc,"icy-name:",strlen("icy-name:")) == 0)
			{
				streamName = "";
				strcpy(streamName,strstr(stringStreamNameLoc, ":")+1);
				printf("%s\n",streamName );
				for (streamNameSize = 0; streamNameSize < 16; ++streamNameSize)
				{
					if (streamName[streamNameSize] == 0)
					{
						streamNameSize-=1;
						break;
					}
				}
				streamNameLocLCD = ( 8-(streamNameSize/2));
				LcdClear();
				if (streamNameSize < 16)
				{
					LcdWriteStringAtLoc(streamName, streamNameSize, streamNameLocLCD);
				}
				else
				{
					LcdWriteStringAtLoc(streamName, streamNameSize+1, streamNameLocLCD);
				}
			}

			if (stringData != NULL)
			{
				printf("Hoera, gevonden! %s\n", stringData );
				metaInterval = atoi(strstr(stringData,":")+1);
				printf("MetaInt = %d\n", metaInterval);
			}
			char* EOT = strstr(data, "\r\n\r\n");
			if (EOT != NULL)
			{
				ignoredData = sizeof(EOT); // fout.
				printf("%s\n", EOT);
				break;
			}
			if( 0 == *data )
				break;
		}
		
		printf("Stream name: %s\n",streamName);
		if (strcmp(streamName,"_") == 0)
		{

			LcdClear();
			strcpy(streamName,strstr(address, "/")+1);
			printf("Address name: %s\n",streamName );
			
			for (streamNameSize = 0; streamNameSize < 16; ++streamNameSize)
			{
				if (streamName[streamNameSize] == 0)
				{
					streamNameSize+=1;
					break;
				}
			}
			streamNameLocLCD = ( 8-(streamNameSize/2));
			LcdClear();
			if (streamNameSize < 16)
			{
				LcdWriteStringAtLoc(streamName, streamNameSize, streamNameLocLCD);
			}
			else
			{
				LcdWriteStringAtLoc(streamName, streamNameSize+1, streamNameLocLCD);
			}
		}
		free(address);
		free(ip);
		free(data);
        return stream;
    }
}

void GetSettingsHTTP(void) 				   //NEW
{ 
	FILE *streampie; //NEW - MOVED FROM GLOBAL TO INSTRUCTION VARIABLE
	LedControl(LED_TOGGLE); //EYE-CANDY
	char* stringDataType; //CHECKED - NO MALLOC/CALLOC
	char* stringStreamAddr; //CHECKED - NO MALLOC/CALLOC
	char* settingsType; //FREEED
	char* streamAddrStripped; //NOT FREEED BUT MALLOCED!!! WARNING WARNING WARNING COULD BE THE CAUSE!!!

	char *data; //FREEED!! COULD STILL BE CAUSE OF CRASHES!!!!!
	
	TCPSOCKET *sockie; //NEW - MOVED FROM GLOBAL TO INSTRUCTION VARIABLE
	sockie = NutTcpCreateSocket();
    uint32_t socketTimeout = 1000;

    int errorCodeNutTcpSetSockOpt = 0;
    errorCodeNutTcpSetSockOpt = NutTcpSetSockOpt(sockie, SO_RCVTIMEO, &socketTimeout,sizeof(socketTimeout));
    printf("NutTcpSetSockOpt: %d\n", errorCodeNutTcpSetSockOpt);
    if (errorCodeNutTcpSetSockOpt)
    {
    	streampie = NULL;
		stringDataType = NULL;
		stringStreamAddr = NULL;
		settingsType = NULL;
		streamAddrStripped = NULL;
		data = NULL;
    	printf("%d\n", NutTcpError(sockie));
    	NutTcpCloseSocket(sockie);
		sockie = NULL;
    	return;
    }
	

    ///NEW
    if( NutTcpConnect(	sockie,
						inet_addr("37.46.136.205"), 
						80) )
	{
		streampie = NULL;
		stringDataType = NULL;
		stringStreamAddr = NULL;
		settingsType = NULL;
		streamAddrStripped = NULL;
		data = NULL;
		NutTcpCloseSocket(sockie);
		sockie = NULL;
		printf("Error: >> NutTcpConnect()");
		return;
	}
    ///END NEW

    else
    {

    	LedControl(LED_TOGGLE); //EYE-CANDY
        streampie = _fdopen((int) sockie, "r+b"); //CHECKED - CLOSES VERY TIME!
        printf("Address is: %s\n", "/settings");
        fprintf(streampie, "GET %s HTTP/1.0\r\n", "/settings");
		fprintf(streampie, "Host: %s\r\n", "62.212.132.54");
		fprintf(streampie, "User-Agent: Ethernut\r\n");
		fprintf(streampie, "Accept: */*\r\n");
		fprintf(streampie, "Connection: close\r\n\r\n");
		fflush(streampie);
		LedControl(LED_TOGGLE); //EYE-CANDY

		// Server stuurt nu HTTP header terug, catch in buffer
		data = (char *) malloc(512 * sizeof(char)); //(as told before) FREEED!! COULD STILL BE CAUSE OF CRASHES!!!!!
		
		settingsType = calloc(sizeof(char),16); //FREEED
		streamAddrStripped = calloc(sizeof(char),100); //NOT FREEED BUT MALLOCED!!! WARNING WARNING WARNING COULD BE THE CAUSE!!!

		//Nothing special going on in here: except for the strncopies!
		while( fgets(data, 512, streampie) )
		{
			LedControl(LED_TOGGLE);
			stringDataType = strstr(data, "Type:");
			stringStreamAddr = strstr(data, "StreamAddr:");
			
			if (strncmp(data, "Type:", strlen("Type:")) == 0)
			{
				strncpy(settingsType,strstr(stringDataType, ":")+1, 16); //FREED, BOTH OF THE POINTY POINTERS
			}
			if (strncmp(data, "StreamAddr:", strlen("StreamAddr:")) == 0)
			{
				strncpy(streamAddrStripped,strstr(stringStreamAddr, ":")+1, 100); //NOT FREEED BUT MALLOCED!!! WARNING WARNING WARNING COULD BE THE CAUSE!!! (talking about the dreaded streamAddrStripped)
				if (strncmp(streamAddrStripped, "STOP", strlen("STOP")) == 0)
				{
					setPlaying(0);
					// streamURLCurrent = ""; //STRANGE, AND NOT ACCEPTABLE
					char* pointerTemp = "";
					strcpy(streamURLCurrent,pointerTemp); 
					pointerTemp = NULL;
					LcdClear();
					// streamName = "_";
				}
				break;
			}
			memset(data,0,512); //Not sure if works
		}
		stringDataType = NULL;
		stringStreamAddr = NULL;
		printf("settingsType: %s\n", settingsType);
		free(data); //FREE, AT LAST! :D
		
		int i;
		for (i = 0; i < 100; ++i)
		{
			if (streamAddrStripped[i]==10) //lf
			{
				streamAddrStripped[i] = 0;
				break; //I WANT TO BREAK FREE!
			}
		}
		
		fclose(streampie); //CLOSED FILE STREAM
		streampie = NULL; //POINTER CLEAR WITHOUT ALLOCATION, I AM SO SMART
		printf("socket close exit code %d", NutTcpCloseSocket(sockie)); //CLOSE SOCKET!
		sockie = NULL;

		if (strcmp(streamURLCurrent,streamAddrStripped)!=0 
				&& strncmp(streamAddrStripped, "STOP", strlen("STOP")) != 0 
				&& strncmp(settingsType, "STREAMREQ", 7) == 0 
				&& strcmp(streamAddrStripped,"") != 0) // WORKS AND THIS IF IS A BEAUTY :)
		{
			printf("Hallo\n");
			if (isPlaying())
            {
                setPlaying(0);
                NutSleep(1500);
            }
	        strcpy(streamURLCurrent,streamAddrStripped);
            printf("streamURLCurrent: %s\n",streamURLCurrent );
            printf("streamAddrStripped: %s\n", streamAddrStripped);
	        FILE* webstream = GetHTTPRawStreamWithAddress(streamURLCurrent); //WORKS, WE THINK ;)
	        initPlayer(); //WORKS, WE KNOW
	        // int playResult = play(webstream); //WORKS, WE KNOW! :D
	        play(webstream);
	        // streamURLCurrent = streamAddrStripped; //WHAT THE FUCK IS GOING ON HERE? *BARF*
	        free(streamAddrStripped);
	        free(settingsType); //FREEEEE
			printf("streamURLCurrent: %s\n", streamURLCurrent);
			LedControl(LED_OFF); //BYE_BYE EYE-CANDY
			return;
			// return ""; //BYE_BYE getSettingsHTTP()
		}
		else
		{
			free(streamAddrStripped);
	        free(settingsType); // FRIET!
			printf("streamURLCurrent: %s\n", streamURLCurrent);
			LedControl(LED_OFF); //BYE_BYE EYE-CANDY
			return;
			// return ""; //BYE_BYE getSettingsHTTP()
		}
    }
}

void GetAlarmsHTTP(){
	FILE* streampie;

	char *data;
	
	TCPSOCKET *sockie; //NEW - MOVED FROM GLOBAL TO INSTRUCTION VARIABLE
	sockie = NutTcpCreateSocket();
    uint32_t socketTimeout = 1000;

    int errorCodeNutTcpSetSockOpt = 0;
    errorCodeNutTcpSetSockOpt = NutTcpSetSockOpt(sockie, SO_RCVTIMEO, &socketTimeout,sizeof(socketTimeout));
    printf("NutTcpSetSockOpt: %d\n", errorCodeNutTcpSetSockOpt);
    if (errorCodeNutTcpSetSockOpt)
    {
    	streampie = NULL;
		data = NULL;
    	printf("%d\n", NutTcpError(sockie));
    	NutTcpCloseSocket(sockie);
		sockie = NULL;
    	return;
    }
	

    ///NEW
    if( NutTcpConnect(	sockie,
						inet_addr("37.46.136.205"), 
						80) )
	{
		streampie = NULL;
		data = NULL;
		NutTcpCloseSocket(sockie);
		sockie = NULL;
		printf("Error: >> NutTcpConnect()");
		return;
	}
    ///END NEW
    else
    {
        streampie = _fdopen((int) sockie, "r+b");
        printf("Address is: %s\n", "/alarms");
        fprintf(streampie, "GET %s HTTP/1.0\r\n", "/alarms");
		fprintf(streampie, "Host: %s\r\n", "62.212.132.54");
		fprintf(streampie, "User-Agent: Ethernut\r\n");
		fprintf(streampie, "Accept: */*\r\n");
		fprintf(streampie, "Connection: close\r\n\r\n");
		fflush(streampie);
		
		// Server stuurt nu HTTP header terug, catch in buffer
		data = (char *) malloc(512 * sizeof(char));
		
		char *alarmText;
		char *alarmStreamName;
		char *alarmType; //0 : primary  1 : secondary
		char *alarmTimeText;

		char *alarmTextTag;
		char *alarmStreamNameTag;
		char *alarmTypeTag; //0 : primary  1 : secondary
		char *alarmTimeTextTag;

		alarmText = malloc (sizeof(char)*100);
		alarmStreamName = malloc(sizeof(char)*16);
		alarmType = malloc(sizeof(char) * 4);
		alarmTimeText = malloc(sizeof(char)*32);
		
		int i;
		while( fgets(data, 512, streampie) )
		{
			//Alarm parts
			alarmTextTag = strstr(data, "AlarmTextA:");
			alarmStreamNameTag = strstr(data, "AlarmStreamIDA:");
			alarmTypeTag = strstr(data, "AlarmTypeA:"); //0 : primary  1 : secondary
			alarmTimeTextTag = strstr(data, "AlarmTimeA:");

			if (strncmp(data, "AlarmTextA:", strlen("AlarmTextA:")) == 0)
			{
				strncpy(alarmText,strstr(alarmTextTag, ":")+1, 100);
				for (i = 0; i < 100; ++i)
				{
					if (alarmText[i]==10) //lf
					{
						alarmText[i] = 0;
						break;
					}
				}
			}
			if (strncmp(data, "AlarmStreamIDA:", strlen("AlarmStreamIDA:")) == 0)
			{
				strncpy(alarmStreamName,strstr(alarmStreamNameTag, ":")+1, 16);
				for (i = 0; i < 16; ++i)
				{
					if (alarmStreamName[i]==10) //lf
					{
						alarmStreamName[i] = 0;
						break;
					}
				}
			}
			if (strncmp(data, "AlarmTimeA:", strlen("AlarmTimeA:")) == 0)
			{
				strncpy(alarmTimeText,strstr(alarmTimeTextTag, ":")+1, 32);	
				for (i = 0; i < 32; ++i)
				{
					if (alarmTimeText[i]==10) //lf
					{
						alarmTimeText[i] = 0;
						break;
					}
				}		
			}
			if (data == "end")
				break;
		}
		printf("alarmText: %s\n", alarmText);
		printf("alarmStreamName %s\n",alarmStreamName);
		printf("alarmTimeText %s\n",alarmTimeText);
		_alarm *constructedAlarm = malloc(sizeof(_alarm));
		alarmText[strlen(alarmText)-1]=0;
		constructedAlarm->alarmText = alarmText;
		constructedAlarm->alarmStreamName = alarmStreamName;
		//constructedAlarm->alarmType = alarmType;
		constructedAlarm->alarmType = 0;
		tm *alarmTime = calloc(1,sizeof(tm));
		alarmTime -> tm_min  = atoi(strstr(alarmTimeText,":")+1);		
		alarmTimeText[2] = 0;
		alarmTime -> tm_hour = atoi(alarmTimeText);
		constructedAlarm->alarmTime = alarmTime;	
		tm *newTime = constructedAlarm->alarmTime;
		printf("Text:%s Stream:%s Type:%d Time:%d:%d\n",constructedAlarm->alarmText,
														constructedAlarm->alarmStreamName,
														constructedAlarm->alarmType,
														newTime->tm_hour,
														newTime->tm_min);
		AlarmControlCreateDailyAlarm(constructedAlarm);
		free(data);
		fclose(streampie);
		printf("socket close exit code %d\n", NutTcpCloseSocket(sockie));
		alarmTextTag = NULL;
		alarmStreamNameTag = NULL;
		alarmTypeTag = NULL;
		alarmTimeTextTag = NULL;
		free(alarmTimeText);
		streampie = NULL;

    }
}

void GetStreamURL(){
	FILE * streampie;
	char* data;
	char* baseUrl = "/cgi-bin/api.php?q=getstreamurl&id=";
	char* constExtens = "&time=";
	size_t urlLength = strlen(baseUrl) + 32;
	char* streamID = AlarmControlActivePrimaryAlarm->alarmStreamName;
    char* netaddress = malloc(urlLength);
    memset (netaddress, 0, urlLength); 
    strcpy (netaddress, baseUrl);
    netaddress = strcat(netaddress, streamID);
    tm *timeNow = malloc(sizeof(tm));
   	X12RtcGetClock(timeNow);
    char* timeStr = malloc(6);
    sprintf(timeStr, "%d:%d\0", timeNow->tm_hour, timeNow->tm_min);
    free(timeNow);
    printf("time:%s\n", timeStr);
    netaddress = strcat(netaddress, constExtens);
    //netaddress = strcat(netaddress, constExtens);
    netaddress = strcat(netaddress, timeStr);

	TCPSOCKET *sockie; //NEW - MOVED FROM GLOBAL TO INSTRUCTION VARIABLE
	sockie = NutTcpCreateSocket();
    uint32_t socketTimeout = 1000;

    int errorCodeNutTcpSetSockOpt = 0;
    errorCodeNutTcpSetSockOpt = NutTcpSetSockOpt(sockie, SO_RCVTIMEO, &socketTimeout,sizeof(socketTimeout));
    printf("NutTcpSetSockOpt: %d\n", errorCodeNutTcpSetSockOpt);
    if (errorCodeNutTcpSetSockOpt)
    {
    	streampie = NULL;
		data = NULL;
    	printf("%d\n", NutTcpError(sockie));
    	NutTcpCloseSocket(sockie);
		sockie = NULL;
    	return;
    }
	

    ///NEW
    if( NutTcpConnect(	sockie,
						inet_addr("37.46.136.205"), 
						80) )
	{
		streampie = NULL;
		data = NULL;
		NutTcpCloseSocket(sockie);
		sockie = NULL;
		printf("Error: >> NutTcpConnect()");
		return;
	}
    else
    {
        streampie = _fdopen((int) sockie, "r+b");
        printf("Address is: %s\n", netaddress);
        fprintf(streampie, "GET %s HTTP/1.0\r\n", netaddress);
		fprintf(streampie, "Host: %s\r\n", "62.212.132.54");
		fprintf(streampie, "User-Agent: Ethernut\r\n");
		fprintf(streampie, "Accept: */*\r\n");
		fprintf(streampie, "Connection: close\r\n\r\n");
		fflush(streampie);
		
		// Server stuurt nu HTTP header terug, catch in buffer
		data = (char *) malloc(512 * sizeof(char));
		
		char *streamURL;

		char *streamURLtag;

		streamURL = calloc(1, sizeof(char)*64);		
		int i;
		while( fgets(data, 512, streampie) )
		{
			printf("data[%s]\n", data);
			//Alarm parts
			streamURLtag = strstr(data, "StreamURL:");

			if (strncmp(data, "StreamURL:", strlen("StreamURL:")) == 0)
			{
				strncpy(streamURL,strstr(streamURLtag, ":")+1, 100);
				for (i = 0; i < 100; ++i)
				{
					if (streamURL[i]==10) //lf
					{
						streamURL[i] = 0;
						break;
					}
				}
				break;
			}
		}
		if (strcmp(streamURLCurrent,streamURL)!=0 
				&& strncmp(streamURL, "NONE", strlen("NONE")) != 0 
				&& strcmp(streamURL,"") != 0) // WORKS AND THIS IF IS A BEAUTY :)
		{
			if (isPlaying())
            {
                setPlaying(0);
                NutSleep(1500);
            }
	        FILE* webstream = GetHTTPRawStreamWithAddress(streamURLCurrent); //WORKS, WE THINK ;)
	        initPlayer(); 
	        play(webstream);
			printf("streamURL: %s\n", streamURL);
			LedControl(LED_OFF); //BYE_BYE EYE-CANDY
			return;
		}
		else
		{
			printf("streamURLCurrent: %s\n", streamURLCurrent);
			LedControl(LED_OFF); //BYE_BYE EYE-CANDY
			return;
		}
    }
}
