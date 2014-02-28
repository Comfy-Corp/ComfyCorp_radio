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


#define NOK 1
#define OK 0
FILE *stream;
int* ignoredData = 0;
int* metaInterval = 0;

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
	
	NutSleep(1000);
	
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
   //  TCPSOCKET* sock;
   //  printf("connecting to %s\n", ip);
   //  sock = NutTcpCreateSocket();
   //  if (NutTcpConnect(sock, inet_addr(ip), 8000)) {
   //      /* Error: Cannot connect server. */
   //      printf("%s\n", "server connection failed");
   //  }
   //  else
   //  {
   //      FILE *stream;
 		// printf("%s\n", "server connection ok");
   //      stream = _fdopen((int) sock, "r+b");
   //      fwrite("GET / HTTP/1.0\r\n\r\n", 1, 26, stream);
   //      fflush(stream);
   //      return stream;
   //  }

	int result = OK;
	char *data;
	
	TCPSOCKET *sock;
	
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
	
	TCPSOCKET *sock;
	
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
	int result = OK;
	char *data;
	

    TCPSOCKET* sock;
    sock = NutTcpCreateSocket();
	char* ip = malloc(23*sizeof(char));
	strncpy(ip, netaddress, 22);
	char nullTerm=0;
	int slashLoc=0;
	int colonLoc=0;
	int i;
	int port = 80;

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
    char* address = malloc(64*sizeof(char));
    printf("connecting to ip %s\n", ip);
    if( NutTcpConnect(	sock,
						inet_addr(ip), 
						port) )
	{
		printf("Error: >> NutTcpConnect()");
	}
    else
    {
        FILE *stream;
        if (nullTerm>0)
        {
	 		for (i=slashLoc;i < 80; ++i)
	        {
	            address[i-slashLoc] = netaddress[i];
	        }
    	}
    	else
    	{
    		address = "";
    	}	
    	//printf("opening %s%s\n", ip, address);
        stream = _fdopen((int) sock, "r+b");
        printf("Address is: %s\n", address);
        fprintf(stream, "GET %s HTTP/1.0\r\n", address);
		fprintf(stream, "Host: %s\r\n", "62.212.132.54");
		fprintf(stream, "User-Agent: Ethernut\r\n");
		fprintf(stream, "Accept: */*\r\n");
		// fprintf(stream, "Icy-MetaData: 1\r\n");
		fprintf(stream, "Connection: close\r\n\r\n");
		fflush(stream);

		
		// Server stuurt nu HTTP header terug, catch in buffer
		data = (char *) malloc(512 * sizeof(char));
		
		while( fgets(data, 512, stream) )
		{
			char* stringData = strstr(data, "icy-metaint:");
			char* stringStreamNameLoc = strstr(data, "icy-name:");
			int* streamNameSizeTemp = &streamNameSize;
			int* streamNameLocLCDTemp = &streamNameLocLCD;
			char* streamNameTemp = malloc(sizeof(char)*16);
			if (stringStreamNameLoc != NULL)
			{
				streamNameTemp = strstr(stringStreamNameLoc, ":")+1;
				if (streamNameTemp)
				{
					for (*streamNameSizeTemp = 0; *streamNameSizeTemp < 16; ++*streamNameSizeTemp)
					{
						if (streamNameTemp[*streamNameSizeTemp] == 0)
						{
							*streamNameSizeTemp-=1;
							break;
						}
					}
					*streamNameLocLCDTemp = ( 8-(*streamNameSizeTemp/2));
					LcdWriteStringAtLoc(streamNameTemp, *streamNameSizeTemp, *streamNameLocLCDTemp);
					stringStreamNameLoc = NULL;
					// Dit alles crasht :S
					// TODO FIX
					// strcpy(streamName, streamNameTemp);
					// free(streamNameSizeTemp);
					// free(streamNameLocLCDTemp);
					// free(streamNameTemp);
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
				ignoredData = sizeof(EOT);
				//printf("%s\n", EOT);
				break;
			}
			if( 0 == *data )
				break;
		}
		

		free(data);
        return stream;
    }
}


