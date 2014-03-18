typedef struct _alarm{
	char *alarmText;
	char *alarmStreamName;
	u_char alarmType; //0 : primary  1 : secondary
	struct tm *alarmTime;
}_alarm;

extern void AlarmControlSleep(void);
extern void AlarmControlCreateDaylyAlarm(struct _alarm);
extern void AlarmControlCreateYearlyAlarm(struct _alarm);
extern u_long AlarmControlCheck();
extern void AlarmControlInit(void);
extern void AlarmControlRemoveDaylyAlarm(void);

struct _alarm *AlarmControlActivePrimaryAlarm;
struct _alarm *AlarmControlActiveSecondaryAlarm; //Not used yet
