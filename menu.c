#include <stdio.h>
#include <string.h>
#define MENU_ITEM_SIZE 32

typedef struct node {
    struct node* next;
    struct node* prev;
    char data[MENU_ITEM_SIZE];
} node;

void insert(node** head, const char* item) {
    if (*head == NULL) {
        *head = (node*)malloc(sizeof(node));
        strcpy((*head)->data, item);
        (*head)->next = NULL;
        (*head)->prev = *head;
    }
    else {
        (*head)->prev->next = (node*)malloc(sizeof(node)); 
        strcpy((*head)->prev->next->data, item);
        (*head)->prev->next->prev = (*head)->prev;
        (*head)->prev->next->next = NULL;
        (*head)->prev = (*head)->prev->next;
    }
}

int main() {
    int menus = 3;
    node *mainMenu = NULL;
    node *fileMenu = NULL;
    node *editMenu = NULL;
    insert(&mainMenu, "File");
    insert(&mainMenu, "Edit");
    insert(&mainMenu, "Search");
}