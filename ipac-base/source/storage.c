/************************************************************************
 *Storage abstraction class. Allows quick volatile storage of           
 *any short amount of information. Interfaces mostly with 'flash' 
 *module.                                                                   
 *
 * Page mapping:
 * 0 - Timezone mapping
 * 1 - XXX                                                                      
 ************************************************************************/
#include "storage.h"
#include "flash.h"
#include <time.h>


void StorageInit(){
	At45dbInit();
}

void StorageTestProcedure(void){
	storageSaveConfig(3);
	printf("Stored.\n");
	NutSleep(5000);
	char testResult;
	printf("Loading...");
	storageLoadConfig(&testResult);
	printf("Retval: %d \n", testResult);
	if(testResult == 3)
		printf("Operation success\n");

}

void StorageSaveConfig(char timeZone){
	At45dbPageErase(1);
	int result = At45dbPageWrite(1, timeZone, sizeof(timeZone));
	printf(result);
	if(!result)
		printf("FUCK\n");
}

void StorageLoadConfig(char *timeZone){
	int result = At45dbPageRead(1, &timeZone, sizeof(timeZone));
	printf(result);
	if(!result)
		printf("FUCK\n");
}
