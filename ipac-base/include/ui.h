#define UISTATE_SHOWTIME 0
#define UISTATE_SHOWALARM 1
#define UISTATE_SHOWSYNCING 2
#define UISTATE_SHOWSETUP 3
#define UISTATE_SHOWRESET 4

typedef struct menuItem {
	char* displayableText;
	struct menuItem* next;
	struct menuItem* prev;
} menutItem;

typedef struct menuItemContainer {
	struct menuItem* firstMenuItem;
	struct menuItem* lastMenuItem;
} menuItemContainer;

int UIshow(void);
void UIchangeState(char);
int UIGetState(void);
int UIScreenUp(void);
int UIScreenDown(void);
int UIScreenLeft(void);
int UIScreenRight(void);
int UIScreenOK(void);
int UIScreenEsc(void);
int UIHandleInput(int);
