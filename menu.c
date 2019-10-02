#include "menu.h"

#ifdef _WIN32
	#include <conio.h>
#elif __linux__
	#include <termios.h>
	#include <unistd.h>
	int getch() {
		struct termios oldattr, newattr;
		int ch;
		tcgetattr(STDIN_FILENO, &oldattr); //getting old attributes of terminal
		newattr = oldattr;
		/*
		 * The following line modifies the local terminal modes.
		 * It disables ICANON and ECHO
		 * Disabling ICANON disables canonical mode/line editing mode.
		 * When ICANON is disabled, the input is made available to the program immediately.
		 * Disabling ECHO ensures that any input is not re-echoed as output.
		 * */
		newattr.c_lflag &= ~(ICANON | ECHO);
		tcsetattr(STDIN_FILENO, TCSANOW, &newattr); //setting new attributes to terminal
		ch = getchar(); //getting character
		tcsetattr(STDIN_FILENO, TCSANOW, &oldattr); //resetting old attributes of terminal
		return ch;
	}
#endif

void generateMenu(Menu* menu) {
	char menuHeadNames[][MENU_ITEM_SIZE] = {"File", "Edit", "Search", "Help"};
	initMenu(menu, menuHeadNames, 4);
	insertSubMenuItem(menu, 0, "New");
	insertSubMenuItem(menu, 0, "Open");
	insertSubMenuItem(menu, 0, "Save");
	insertSubMenuItem(menu, 0, "Save As");
	insertSubMenuItem(menu, 1, "Cut");
	insertSubMenuItem(menu, 1, "Copy");
	insertSubMenuItem(menu, 1, "Paste");
	insertSubMenuItem(menu, 1, "Delete");
	insertSubMenuItem(menu, 2, "Find");
	insertSubMenuItem(menu, 2, "Replace");
	insertSubMenuItem(menu, 3, "About");
}

void startProcess() {
	Menu menu;
	Node* currentMenu;
	Node* currentSubMenu;
	char input;
	generateMenu(&menu);
	currentMenu = menu.head;
	currentSubMenu = NULL;
	do {
		system("clear || cls"); //clear screen
		printf("Use arrow keys to navigate the menu (X to exit)\n"
			"You can also use the first letter of each menu to jump to it\n\n");
		displayMenu(&menu, currentMenu, currentSubMenu);
		
		if ((input = getch()) == 'X') {
			printf("\nExiting...\n");
		}
		else if (input == 'f' || input == 'F') {
			currentMenu = menu.head;
			currentSubMenu = *(menu.subMenuHeads + currentMenu->id);
		}
		else if (input == 'e' || input == 'E') {
			currentMenu = menu.head->next;
			currentSubMenu = *(menu.subMenuHeads + currentMenu->id);
		}
		else if (input == 's' || input == 'S') {
			currentMenu = menu.head->next->next;
			currentSubMenu = *(menu.subMenuHeads + currentMenu->id);
		}
		else if (input == 'h' || input == 'H') {
			currentMenu = menu.head->next->next->next;
			currentSubMenu = *(menu.subMenuHeads + currentMenu->id);
		}
		#ifdef _WIN32
		else switch(getch()) { //get arrow key
			case 72: //up
				if (currentSubMenu != NULL) currentSubMenu = currentSubMenu->prev;
				break;
			case 80: //down
				currentSubMenu = currentSubMenu == NULL ? *(menu.subMenuHeads + currentMenu->id) : currentSubMenu->next;
				break;
			case 77: //right
				currentMenu = currentMenu->next;
				currentSubMenu = NULL;
				break;
			case 75: //left
				currentMenu = currentMenu->prev;
				currentSubMenu = NULL;
				break;
		}
		#elif __linux__
		else if (input == '\033') { //control key
			getch(); //skip '['
			switch(getch()) { //get arrow key
				case 'A': //up
					if (currentSubMenu != NULL) currentSubMenu = currentSubMenu->prev;
					break;
				case 'B': //down
					currentSubMenu = currentSubMenu == NULL ? *(menu.subMenuHeads + currentMenu->id) : currentSubMenu->next;
					break;
				case 'C': //right
					currentMenu = currentMenu->next;
					currentSubMenu = NULL;
					break;
				case 'D': //left
					currentMenu = currentMenu->prev;
					currentSubMenu = NULL;
					break;
			}
		}
		#endif
	} while (input != 'X' && input != 'x');
}

int main() {
	startProcess();
	return 0;
}
