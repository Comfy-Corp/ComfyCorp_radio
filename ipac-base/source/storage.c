/************************************************************************
 *Storage abstraction class. Allows quick volatile storage of           
 *any short amount of information.                                                                 
 *                                                                    
 ************************************************************************/
#include "storage.h"
#include "alarmControl.h"
#include <time.h>
#include <dev/board.h>
#include <stdio.h>


//On reboot increment, print amount of reboots.
void StorageTestProcedure(void){
	_StorableSetting setting;
	StorageLoadConfig(&setting);
	if (setting.len != sizeof(setting))
	{
		setting.timezone = 0;
		printf("no valid config found\n");
	} else {
		printf("amount of boots: %d\n", setting.timezone);
	}
	setting.len = sizeof(setting);
	setting.timezone++;
	StorageSaveConfig(&setting);
}

void StorageSaveConfig(_StorableSetting* setting){
	NutNvMemSave(512, setting, sizeof(setting));
}

void StorageLoadConfig(_StorableSetting* setting){
	NutNvMemLoad(512, setting, sizeof(setting));
}

void StorageSavePrimaryAlarm(_alarm* alarm){
	NutNvMemSave(1024, alarm, sizeof(alarm));
}

void StorageLoadPrimaryAlarm(_alarm* alarm){
	NutNvMemLoad(1024, alarm, sizeof(alarm));
}




