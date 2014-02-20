#include <sys/thread.h>
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
#include <sys/heap.h>
#include <sys/bankmem.h>
#include <netinet/tcp.h>


#define NOK 1
#define OK 0

int ethInitInet(void)
{
	//uint8_t mac_addr[6] = { 0xC0, 0x01, 0x1E, 0x01, 0x02, 0x03 };
	uint8_t mac_addr[6] = { 0x00, 0x0f, 0xFE, 0x74, 0x13, 0x8D };

	
	int result = OK;
	printf("%s\n", "makakakakakaka");

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
    TCPSOCKET* sock;
    printf("connection to %s\n", ip);
    sock = NutTcpCreateSocket();
    if (NutTcpConnect(sock, inet_addr(ip), 8000)) {
        /* Error: Cannot connect server. */
        printf("%s\n", "server connection failed");
    }
    else
    {
        FILE *stream;
        /* ... more code here ... */
 		printf("%s\n", "server connection ok");
        stream = _fdopen((int) sock, "r+b");
        fwrite("GET HTTP/1.0\r\n\r\n", 1, 18, stream);
        return stream;
    }
}

FILE* GetHTTPRawStreamWithAddress(char* netaddress, int port)
{
    TCPSOCKET* sock;
    sock = NutTcpCreateSocket();
	char* ip = malloc(17*sizeof(char));
	strncopy(ip, netaddress, 17);
	char nullTerm=0;
	int dashLoc=0;
	for(int i=0;i<=17;++i;)
	{
		if(ip[i]=='/')
		{
			ip[i]=0;
			nullterm = 1;
			dashLoc = i;
			break;
		}
		else if (ip[i] ==0)
		{
			nullterm=-1;
			break;
		}
	} 
	if (!nullTerm)
	{
		ip[17] = 0;
	}   
    //str[strlen(str) - 1] = 0;
    char* address = malloc(64*sizeof(char));
    printf("connecting to ip %s\n", ip);
    if (NutTcpConnect(sock, inet_addr(*ip), port)) {
        /* Error: Cannot connect server. */
    }
    else
    {
        FILE *stream;
        /* ... more code here ... */
        if (nullterm>0)
        {
	 		for (i=dashLoc;i < 81; ++i)
	        {
	            address[i-1] = netaddress[i];
	        }
    	}
    	else
    	{
    		address = "";
    	}	
    	printf("opening %s%s\n", ip, address);
        stream = _fdopen((int) sock, "r+b");
        fprintf(stream, "GET %s HTTP/1.1\r\n\r\n", address);
        return stream;
    }
}


