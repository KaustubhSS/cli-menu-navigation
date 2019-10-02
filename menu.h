#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MENU_ITEM_SIZE 32

typedef struct Node {
	struct Node* next;
	struct Node* prev;
	char data[MENU_ITEM_SIZE];
	int id;
} Node;

typedef struct {
	int subMenus;
	Node* head;
	Node** subMenuHeads;
} Menu;

void insert(Node** head, const char* item, int id) {
	Node* temp;
	if (*head == NULL) {
        	*head = (Node*)malloc(sizeof(Node));
        	strcpy((*head)->data, item);
        	(*head)->id = id;
        	(*head)->next = *head;
        	(*head)->prev = *head;
	}
	else {
		temp = (Node*)malloc(sizeof(Node));
		temp->next = *head;
		temp->prev = (*head)->prev;
		strcpy(temp->data, item);
		temp->id = id;
		(*head)->prev->next = temp;
		(*head)->prev = temp;
	}
}

void initMenu(Menu* menu, char menuHeadNames[][MENU_ITEM_SIZE], int subMenus) {
	int i;
	menu->head = NULL;
	for(i = 0; i < subMenus; i++) {
		insert(&menu->head, menuHeadNames[i], i);
	}
	menu->subMenus = subMenus;
	menu->subMenuHeads = (Node**)calloc((size_t)subMenus, sizeof(Node));
	for(i = 0; i < subMenus; i++) {
		*(menu->subMenuHeads + i) = NULL;
	}
}

void insertSubMenuItem(Menu* menu, int subMenu, const char* text) {
	if (subMenu >= menu->subMenus) return; //illegal memory access!
	insert(menu->subMenuHeads + subMenu, text, subMenu);
}

void displayMenu(Menu* menu, Node* item, Node* subItem) {
	int i = 0;
	Node* current = menu->head;
	//print main menu
	while(current->next != menu->head) {
		if (item == current) printf(">> %s\t\t", current->data);
		else printf("%s\t\t", current->data);
		current = current->next;
	}
	//once more for last element
	if (item == current) printf(">> %s\t\t", current->data);
	else printf("%s\t\t", current->data);
	printf("\n");
	//print submenu
	if (subItem != NULL) {
		current = *(menu->subMenuHeads + item->id);
		while(current->next != *(menu->subMenuHeads + item->id)) {
			for(i = 0; i < item->id; i++) printf("\t\t");
			if (subItem == current) printf("> %s\n", current->data);
			else printf("%s\n", current->data);
			current = current->next;
		}
		//once more for last element
		for(i = 0; i < item->id; i++) printf("\t\t");
		if (subItem == current) printf("> %s\n", current->data);
		else printf("%s\n", current->data);
	}
}
