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
#include <sys/timer.h>
 
#include <arpa/inet.h>
#include <net/route.h>
#include <pro/dhcp.h>
#include <pro/sntp.h>
 
#include <stdlib.h>
#include <stdio.h>
#include <io.h>
#include <string.h>
#include <time.h>

#define NOK 1
#define OK 0

int ethInitInet(void)
{
	uint8_t mac_addr[6] = { 0xC0, 0x01, 0x1E, 0x01, 0x02, 0x03 };
	
	int result = OK;

	// Registreer NIC device (located in nictrl.h)
	if( NutRegisterDevice(&DEV_ETHER, 0, 0) )
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
	time_t ntp_time = 0;
	tm *ntp_datetime;
	uint32_t timeserver = 0;
	/* Retrieve time from the "pool.ntp.org" server. This is a free NTP server. */
    puts("Retrieving time from pool.ntp.org...");
 
    timeserver = inet_addr("213.203.238.86");
 
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
    return 0;
}
