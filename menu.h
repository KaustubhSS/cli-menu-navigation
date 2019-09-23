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
	Node* temp;
    if (*head == NULL) {
        *head = (Node*)malloc(sizeof(Node));
        strcpy((*head)->data, item);
        (*head)->next = *head;
        (*head)->prev = *head;
    }
    else {
		temp = (Node*)malloc(sizeof(Node));
		temp->next = *head;
		temp->prev = (*head)->prev;
		strcpy(temp->data, item);
		(*head)->prev->next = temp;
		(*head)->prev = temp;
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
	int allDone = 0;
	Node* current = menu->head;
	system("clear || cls");
	while(current->next != menu->head) {
		if (item == current) printf(">> %s\t\t", current->data);
		else printf("%s\t\t", current->data);
		current = current->next;
	}
	//once more for last element
	if (item == current) printf(">> %s\t\t", current->data);
	else printf("%s\t\t", current->data);
	printf("\n\n");
	
	Node** currentHeads = (Node**)calloc((size_t)menu->subMenus, sizeof(Node));
	int* headDonePrinting = (int*)calloc((size_t)menu->subMenus, sizeof(int));
	for(i = 0; i < menu->subMenus; i++) {
		*(currentHeads + i) = *(menu->subMenuHeads + i);
	}
	while(allDone == 0) {
		for(i = 0; i < menu->subMenus; i++) {
			if ((*(currentHeads + i))->next != *(menu->subMenuHeads + i)) {
				if (subItem == *(currentHeads + i)) printf("> %s\t\t", (*(currentHeads + i))->data);
				else printf("%s\t\t", (*(currentHeads + i))->data);
				*(currentHeads + i) = (*(currentHeads + i))->next;
			}
			else {
				allDone++;
				if (*(headDonePrinting + i) == 0) {
					//printing last element
					if (subItem == *(currentHeads + i)) printf("> %s\t\t", (*(currentHeads + i))->data);
					else printf("%s\t\t", (*(currentHeads + i))->data);
					*(headDonePrinting + i) = 1;
				}
			}
		}
		printf("\n");
		if (allDone == menu->subMenus) {
			break;
		}
		else allDone = 0;
	}
	
	printf("\n\n");
	free(currentHeads);
	free(headDonePrinting);
}
