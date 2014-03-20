
typedef struct _StorableSetting {
	int timezone;
	unsigned char len;
} _StorableSetting;

extern void StorageSaveConfig(_StorableSetting*);

extern void StorageLoadConfig(_StorableSetting*);

//extern void StorageSavePrimaryAlarm(_alarm*);

//extern void StorageLoadPrimaryAlarm(_alarm*);
