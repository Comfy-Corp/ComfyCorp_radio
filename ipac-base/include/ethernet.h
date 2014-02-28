#include <stdio.h>


FILE* stream;
int* metaInterval;
int* ignoredData;
extern int ethInitInet(void);
int connectToStream(void);
extern int ethGetNTPTime(void);
int playStream(void);
extern FILE* GetHTTPRawStream(char*);
FILE* GetHTTPRawStreamWithAddress(char* netaddress);
void fillStringWithStreamName(char requestName[]);
/* -- Stream names and meta data -- */
char* streamName;
int* streamNameSize;
int* streamNameLocLCD;