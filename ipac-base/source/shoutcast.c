
///
#define LOG_MODULE  LOG_SHOUTCAST_MODULE

#include "thread.h"
#include <sys/timer.h>

#include <dev/nicrtl.h>
#include <arpa/inet.h>
#include <sys/confnet.h>
#include <pro/dhcp.h>


#include "log.h"
#include "shoutcast.h"

#include <sys/socket.h>

#define ETH0_BASE	0xC300
#define ETH0_IRQ	5

#define OK			1
#define NOK			0

static char eth0IfName[9] = "eth0";
FILE *stream;

int initInet(void)
{
	uint8_t mac_addr[6] = { 0x00, 0x06, 0x98, 0x30, 0x02, 0x76 };
	
	int result = OK;

	// Registreer NIC device (located in nictrl.h)
	if( NutRegisterDevice(&DEV_ETHER, ETH0_BASE, ETH0_IRQ) )
	{
		LogMsg_P(LOG_ERR, PSTR("Error: >> NutRegisterDevice()"));
		result = NOK;
	}
	
	if( OK == result )
	{
		if( NutDhcpIfConfig(eth0IfName, mac_addr, 0) )
		{
			LogMsg_P(LOG_ERR, PSTR("Error: >> NutDhcpIfConfig()"));
			result = NOK;
		}
	}
	
	if( OK == result )
	{
		LogMsg_P(LOG_INFO, PSTR("Networking setup OK, new settings are:\n") );	

		LogMsg_P(LOG_INFO, PSTR("if_name: %s"), confnet.cd_name);	
		LogMsg_P(LOG_INFO, PSTR("ip-addr: %s"), inet_ntoa(confnet.cdn_ip_addr) );
		LogMsg_P(LOG_INFO, PSTR("ip-mask: %s"), inet_ntoa(confnet.cdn_ip_mask) );
		LogMsg_P(LOG_INFO, PSTR("gw     : %s"), inet_ntoa(confnet.cdn_gateway) );
	}
	
	NutSleep(1000);
	
	return result;
}

int connectToStream(void)
{
	int result = OK;
	char *data;
	
	TCPSOCKET *sock;
	
	sock = NutTcpCreateSocket();
	if( NutTcpConnect(	sock,
						inet_addr("localhost"), 
						8080) )
	{
		LogMsg_P(LOG_ERR, PSTR("Error: >> NutTcpConnect()"));
		exit(1);
	}
	stream = _fdopen( (int) sock, "r+b" );
	
	fprintf(stream, "GET %s HTTP/1.0\r\n", "");
	fprintf(stream, "Host: %s\r\n", "localhost");
	fprintf(stream, "User-Agent: Ethernut\r\n");
	fprintf(stream, "Accept: */*\r\n");
	fprintf(stream, "Icy-MetaData: 1\r\n");
	fprintf(stream, "Connection: close\r\n\r\n");
	fflush(stream);

	// Connection stream debug
	LogMsg_P(LOG_ERR, PSTR("%s"), stream);
	
	// Server stuurt nu HTTP header terug, catch in buffer
	data = (char *) malloc(2048 * sizeof(char));
	
	while( fgets(data, 2048, stream) )
	{
		if( 0 == *data )
			break;
	}
	LogMsg_P(LOG_ERR, PSTR("%s"), data);
	free(data);
	
	return result;
}

int playStream(void)
{
	int *mp3buf;
	int rxbytes;
	int idx = 0;
	
	mp3buf = (int *)malloc(sizeof(int) * 2048);
	
	rxbytes = fread(mp3buf, 1, 2048, stream );

	for( idx = 0; idx < 2048; idx++ )
	{
		printf("%2X ", mp3buf[idx]);
		if((idx%32)==0)
			printf("\n");
	}
	
	return OK;
}

int stopStream(void)
{
	fclose(stream);	
	
	return OK;
}

