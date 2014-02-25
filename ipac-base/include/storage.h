
typedef struct _StorableSetting {
	int timezone;
	unsigned char len;
} _StorableSetting;

//extern void StorageTestProcedure(void);

extern void StorageSaveConfig(_StorableSetting*);

extern void StorageLoadConfig(_StorableSetting*);
//extern void StorageSaveConfig(char);
