typedef struct _alarm{
	struct tm *alarmTime;
	char *alarmText;
	char *alarmStreamName;
	unsigned char alarmType; //0 : primary  1 : secondary
}_alarm;

u_char alarmEventFlag;
u_char alarmPlayingFlag; //I don't like doing this, but I guess it's neccesary

extern void AlarmControlSleep(void);
extern void AlarmControlCreateDailyAlarm(struct _alarm*);
extern void AlarmControlCreateYearlyAlarm(struct _alarm);
extern unsigned long AlarmControlCheck(void);
extern void AlarmControlInit(void);
extern void AlarmControlRemoveDailyAlarm(void);

struct _alarm *AlarmControlActivePrimaryAlarm;
struct _alarm *AlarmControlActiveSecondaryAlarm; //Not used yet
