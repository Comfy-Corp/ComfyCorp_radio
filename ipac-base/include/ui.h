#define UISTATE_SHOWTIME 0
#define UISTATE_SHOWMENU 1
#define UISTATE_SHOWSETUP 2
#define UISTATE_SHOWRESET 3
#define UISTATE_SHOWALARM 4
#define UISTATE_SHOWSYNCING 5

typedef struct menuItem {
	char* displayableText;
	struct menuItem* next;
	struct menuItem* prev;
} menutItem;

typedef struct menuItemContainer {
	struct menuItem* firstMenuItem;
	struct menuItem* lastMenuItem;
} menuItemContainer;

int UIshow();
void UIchangeState(char);