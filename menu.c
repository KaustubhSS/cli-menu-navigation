#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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
	menu->subMenuHeads = calloc(subMenus, sizeof(Node));
	for(i = 0; i < subMenus; i++) {
		*(menu->subMenuHeads + i) = NULL;
	}
}

void insertSubMenuItem(Menu* menu, int subMenu, const char* text) {
	if (subMenu >= menu->subMenus) return; //illegal memory access!
	insert(menu->subMenuHeads + subMenu, text);
}

void displayMenu(Menu* menu, int menuItem, int subMenuItem) {
	int i = 0;
	int j = -1;
	int allNull = 0;
	Node* current = menu->head;
	system("cls || clear");
	while(current != NULL) {
		if (menuItem == i && subMenuItem == j) printf(">> %s\t\t", current->data);
		else printf("%s\t\t", current->data);
		current = current->next;
		i++;
	}
	printf("\n\n");
	j++;
	
	Node** currentHeads = calloc(menu->subMenus, sizeof(Node));
	for(i = 0; i < menu->subMenus; i++) {
		*(currentHeads + i) = *(menu->subMenuHeads + i);
	}
	while(allNull == 0) {
		for(i = 0; i < menu->subMenus; i++) {
			if (*(currentHeads + i) != NULL) {
				if (menuItem == i && subMenuItem == j) printf(">> %s\t\t", (*(currentHeads + i))->data);
				else printf("%s\t\t", (*(currentHeads + i))->data);
				*(currentHeads + i) = (*(currentHeads + i))->next;
			}
		}
		for(i = 0; i < menu->subMenus; i++) {
			if (*(currentHeads + i) == NULL) allNull++;
		}
		if (allNull == menu->subMenus) break;
		else allNull = 0;
		j++;
		printf("\n");
	}
	
	printf("\n\n");
	free(currentHeads);
}

int main() {
    Menu menu;
    int i = 0, j = -1;
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
    
    do {
		displayMenu(&menu, i, j);
		printf("\n\nEnter W/A/S/D to navigate the menu (X to exit):\t");
		scanf("%c", &input);
		switch(input) {
			case 'D':
				if (i == menu.subMenus - 1) i = 0;
				else i++;
				break;
			case 'A':
				if (i == 0) i = menu.subMenus - 1;
				else i--;
				break;
			case 'W':
				if (j == -1) j = menu.subMenus - 1; //gotta replace this :/
				else j--;
				break;
			case 'S':
				if (j == menu.subMenus - 1) j = -1; //this too
				else j++;
				break;
			case 'X':
				printf("Exiting...");
		}
	} while (input != 'X');
    
    return 0;
}
