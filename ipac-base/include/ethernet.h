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