#include <stdio.h>


FILE* stream;
int* metaInterval;
int* ignoredData;
extern int ethInitInet(void);
extern int connectToStream(void);
extern int ethGetNTPTime(void);
extern void GetStreamURL(void);
extern void GetSettingsHTTP(void);
extern void GetAlarmsHTTP(void);
extern int playStream(void);
extern FILE* GetHTTPRawStream(char*);
extern void GetHTTPRawStreamWithAddress(char*, FILE*);
extern void fillStringWithStreamName(char requestName[]);
extern void freeBeforeNewSettingsFile(void);
/* -- Stream names and meta data -- */
char* streamName;
int streamNameSize;
int streamNameLocLCD;
