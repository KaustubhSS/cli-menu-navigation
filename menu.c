#include "menu.h"

#ifdef _WIN32
	#include <conio.h>
#elif __linux__
	#include <termios.h>
	#include <unistd.h>
	int getch() {
		struct termios oldattr, newattr;
		int ch;
		tcgetattr(STDIN_FILENO, &oldattr);
		newattr = oldattr;
		newattr.c_lflag &= ~(ICANON | ECHO);
		tcsetattr(STDIN_FILENO, TCSANOW, &newattr);
		ch = getchar();
		tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);
		return ch;
	}
#endif

int main() {
    Menu menu;
    Node* currentMenu;
    Node* currentSubMenu;
    int i = 0;
    char input;
    char menuHeadNames[][MENU_ITEM_SIZE] = {"File", "Edit", "Search"};
    
    initMenu(&menu, menuHeadNames, 3);
    insertSubMenuItem(&menu, 0, "New");
    insertSubMenuItem(&menu, 0, "Open");
    insertSubMenuItem(&menu, 0, "Save");
    insertSubMenuItem(&menu, 0, "Save As");
    insertSubMenuItem(&menu, 1, "Cut");
    insertSubMenuItem(&menu, 1, "Copy");
    insertSubMenuItem(&menu, 1, "Paste");
    insertSubMenuItem(&menu, 1, "Delete");
    insertSubMenuItem(&menu, 2, "Find");
    insertSubMenuItem(&menu, 2, "Replace");
    
    currentMenu = menu.head;
    currentSubMenu = *menu.subMenuHeads;
    
    do {
		displayMenu(&menu, currentMenu, currentSubMenu);
		printf("\n\nEnter W/A/S/D to navigate the menu (X to exit)");
		input = (char)getch();
		switch(input) {
			case 'd':
			case 'D':
				currentMenu = currentMenu->next;
				if (i == menu.subMenus - 1) i = 0;
				else i++;
				currentSubMenu = *(menu.subMenuHeads + i);
				break;
			case 'a':
			case 'A':
				currentMenu = currentMenu->prev;
				if (i == 0) i = menu.subMenus - 1;
				else i--;
				currentSubMenu = *(menu.subMenuHeads + i);
				break;
			case 'w':
			case 'W':
				currentSubMenu = currentSubMenu->prev;
				break;
			case 's':
			case 'S':
				currentSubMenu = currentSubMenu->next;
				break;
			case 'x':
			case 'X':
				printf("\nExiting...\n");
		}
	} while (input != 'X' && input != 'x');
    
    return 0;
}
