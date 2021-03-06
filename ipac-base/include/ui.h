#define UISTATE_SHOWTIME 0
#define UISTATE_SHOWALARM 1
#define UISTATE_SHOWSYNCING 2
#define UISTATE_SHOWSETUP 3
#define UISTATE_SHOWRESET 4
#define UISTATE_ALARMEVENT 5

typedef struct menuItem {
	char* displayableText;
	struct menuItem* next;
	struct menuItem* prev;
} menutItem;

typedef struct menuItemContainer {
	struct menuItem* firstMenuItem;
	struct menuItem* lastMenuItem;
} menuItemContainer;

void UIchangeState(char);
int UIshow(void);
int UIGetState(void);
int UIScreenUp(void);
int UIScreenDown(void);
int UIScreenLeft(void);
int UIScreenRight(void);
int UIScreenOK(void);
int UIScreenEsc(void);
int UIHandleReset(void);
int UIHandleInput(int);
int UIGetUserSetTimezone(void);
int getStateChar(void);

