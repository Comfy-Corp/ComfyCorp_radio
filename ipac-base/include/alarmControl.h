typedef struct _alarm{
	char *alarmText;
	char *alarmStreamName;
	unsigned char alarmType; //0 : primary  1 : secondary
	struct tm *alarmTime;
}_alarm;

extern void AlarmControlSleep(void);
extern void AlarmControlCreateDailyAlarm(struct _alarm*);
extern void AlarmControlCreateYearlyAlarm(struct _alarm);
extern unsigned long AlarmControlCheck(void);
extern void AlarmControlInit(void);
extern void AlarmControlRemoveDailyAlarm(void);

struct _alarm *AlarmControlActivePrimaryAlarm;
struct _alarm *AlarmControlActiveSecondaryAlarm; //Not used yet
