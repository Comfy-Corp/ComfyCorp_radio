
///
#define LOG_MODULE  LOG_SHOUTCAST_MODULE

#include "thread.h"
#include <sys/timer.h>

#include <dev/nicrtl.h>
#include <arpa/inet.h>
#include <sys/confnet.h>
#include <pro/dhcp.h>


#include "log.h"
#include "shoutcast_test.h"

#include <sys/socket.h>
//#include "player.h"


#define ETH0_BASE	0xC300
#define ETH0_IRQ	5

#define OK			1
#define NOK			0

static char eth0IfName[9] = "eth0";
FILE *stream;

int initInet(void)
{
	uint8_t mac_addr[6] = { 0x00, 0x0f, 0xFE, 0x74, 0x13, 0x8D };
	
	int result = OK;

	// Registreer NIC device (located in nictrl.h)
	if( NutRegisterDevice(&DEV_ETHER, 0x8300, 5) )
	{
		printf("Error: >> NutRegisterDevice()\n");
		result = NOK;
	}
	
	if( OK == result )
	{
		if( NutDhcpIfConfig(eth0IfName, mac_addr, 0) )
		{
			printf("Error: >> NutDhcpIfConfig()\n");
			result = NOK;
		}
	}
	
	if( OK == result )
	{
		printf("Networking setup OK, new settings are:\n");	

		printf("if_name: %s", confnet.cd_name);	
		printf("ip-addr: %s", inet_ntoa(confnet.cdn_ip_addr));
		printf("ip-mask: %s", inet_ntoa(confnet.cdn_ip_mask));
		printf("gw     : %s", inet_ntoa(confnet.cdn_gateway));
	}
	
	NutSleep(1000);
	
	return result;
}

/*int connectToStream(void)
{
	int result = OK;
	char *data;
	
	TCPSOCKET *sock;
	
	sock = NutTcpCreateSocket();
	if( NutTcpConnect(	sock,
						inet_addr("195.95.206.14"), 
						8000) )
	{
		// LogMsg_P(LOG_ERR, PSTR("Error: >> NutTcpConnect()"));
		exit(1);
	}
	stream = _fdopen( (int) sock, "r+b" );
	
	fprintf(stream, "GET /kiss HTTP/1.0\r\n", "/");
	fprintf(stream, "Host: %s\r\n", "195.95.206.14");
	fprintf(stream, "User-Agent: Ethernut\r\n");
	fprintf(stream, "Accept: *r\n");
	fprintf(stream, "Icy-MetaData: 1\r\n");
	fprintf(stream, "Connection: close\r\n\r\n");
	fflush(stream);

	
	// Server stuurt nu HTTP header terug, catch in buffer
	data = (char *) malloc(512 * sizeof(char));
	
	while( fgets(data, 512, stream) )
	{
		if( 0 == *data )
			break;

		printf("%s", data);
	}
	
	free(data);
	
	return result;
}*/

/*int playStream(void)
{
	// play(stream);
	
	return OK;
}

int stopStream(void)
{
	// fclose(stream);	
	
	return OK;
}*/

