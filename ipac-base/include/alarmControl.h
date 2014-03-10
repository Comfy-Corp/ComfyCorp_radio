typedef struct _alarm{
	char *alarmText;
	char *streamName;
	u_char alarmType; //0 : primary  1 : secondary
	tm alarmTime;
}_alarm;

extern void AlarmControlCreateDaylyAlarm(struct _alarm);
extern void AlarmControlCreateYearlyAlarm(struct _alarm);
extern u_long AlarmControlCheck();
extern void AlarmControlTestProcedure(void);

struct _alarm AlarmControlActivePrimaryAlarm;
struct _alarm AlarmControlActiveSecondaryAlarm; //Not used yet
