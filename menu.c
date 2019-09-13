#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#define MENU_ITEM_SIZE 32

typedef struct Node {
    struct Node* next;
    struct Node* prev;
    char data[MENU_ITEM_SIZE];
} Node;

typedef struct {
	int subMenus;
	Node* head;
	Node** subMenuHeads;
} Menu;

void insert(Node** head, const char* item) {
    if (*head == NULL) {
        *head = (Node*)malloc(sizeof(Node));
        strcpy((*head)->data, item);
        (*head)->next = NULL;
        (*head)->prev = *head;
    }
    else {
        (*head)->prev->next = (Node*)malloc(sizeof(Node)); 
        strcpy((*head)->prev->next->data, item);
        (*head)->prev->next->prev = (*head)->prev;
        (*head)->prev->next->next = NULL;
        (*head)->prev = (*head)->prev->next;
    }
}

void initMenu(Menu* menu, char menuHeadNames[][MENU_ITEM_SIZE], int subMenus) {
	int i;
	menu->head = NULL;
	for(i = 0; i < subMenus; i++) {
		insert(&menu->head, menuHeadNames[i]);
	}
	menu->subMenus = subMenus;
	menu->subMenuHeads = (Node**)calloc((size_t)subMenus, sizeof(Node));
	for(i = 0; i < subMenus; i++) {
		*(menu->subMenuHeads + i) = NULL;
	}
}

void insertSubMenuItem(Menu* menu, int subMenu, const char* text) {
	if (subMenu >= menu->subMenus) return; //illegal memory access!
	insert(menu->subMenuHeads + subMenu, text);
}

void displayMenu(Menu* menu, Node* item, Node* subItem) {
	int i = 0;
	int allNull = 0;
	Node* current = menu->head;
	system("clear || cls");
	while(current != NULL) {
		if (item == current) printf(">> %s\t\t", current->data);
		else printf("%s\t\t", current->data);
		current = current->next;
	}
	printf("\n\n");
	
	Node** currentHeads = (Node**)calloc((size_t)menu->subMenus, sizeof(Node));
	for(i = 0; i < menu->subMenus; i++) {
		*(currentHeads + i) = *(menu->subMenuHeads + i);
	}
	while(allNull == 0) {
		for(i = 0; i < menu->subMenus; i++) {
			if (*(currentHeads + i) != NULL) {
				if (subItem == *(currentHeads + i)) printf("> %s\t\t", (*(currentHeads + i))->data);
				else printf("%s\t\t", (*(currentHeads + i))->data);
				*(currentHeads + i) = (*(currentHeads + i))->next;
			}
		}
		for(i = 0; i < menu->subMenus; i++) {
			if (*(currentHeads + i) == NULL) allNull++;
		}
		if (allNull == menu->subMenus) break;
		else allNull = 0;
		printf("\n");
	}
	
	printf("\n\n");
	free(currentHeads);
}

/* reads from keypress, doesn't echo */ int getch(void) { struct termios oldattr, newattr; int ch; tcgetattr( STDIN_FILENO, &oldattr ); newattr = oldattr; newattr.c_lflag &= ~( ICANON | ECHO ); tcsetattr( STDIN_FILENO, TCSANOW, &newattr ); ch = getchar(); tcsetattr( STDIN_FILENO, TCSANOW, &oldattr ); return ch; }

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
		//scanf("%c", &input);
		input = (char)getch();
		switch(input) {
			case 'd':
			case 'D':
				if (currentMenu->next == NULL) {
					currentMenu = menu.head;
				}
				else currentMenu = currentMenu->next;
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
				if (currentSubMenu->next == NULL) {
					currentSubMenu = *(menu.subMenuHeads + i);
				}
				else currentSubMenu = currentSubMenu->next;
				break;
			case 'x':
			case 'X':
				printf("\nExiting...\n");
		}
	} while (input != 'X' && input != 'x');
    
    return 0;
}
