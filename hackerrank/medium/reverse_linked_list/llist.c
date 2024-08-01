#include <stdio.h>
#include <stdlib.h>

struct LinkedListNode
{
    int data;
    struct LinkedListNode* next;
};

struct LinkedListNode* reverse(struct LinkedListNode** head) {
    struct LinkedListNode* curr = *head;
    struct LinkedListNode* prev = NULL;
    struct LinkedListNode* next;

    while (curr != NULL)
    {
        next = curr->next;
        curr->next = prev;
        prev = curr;
        curr =  next;
    }

    *head = prev;
    
    return *head;
}

void printLinkedList(struct LinkedListNode* head) {
    struct LinkedListNode* current = head;
    while (current != NULL)
    {
        printf("%d -> ", current->data);
        current = current->next;
    }
    printf("NULL\n");
}

void freeLinkedList(struct LinkedListNode* head) {
    struct LinkedListNode* current = head;
    struct LinkedListNode* next;
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
}

int main(int argc, char const *argv[])
{
    /* code */
    struct LinkedListNode* head = NULL;
    struct LinkedListNode* first = (struct LinkedListNode*) malloc(sizeof(struct LinkedListNode));
    first->data = 1;
    first->next = *(&head);
    *(&head) = first;
    struct LinkedListNode* current = head;

    for (int i = 2; i < 4; i++)
    {
        struct LinkedListNode* node = (struct LinkedListNode*) malloc(sizeof(struct LinkedListNode));
        node->data = i;
        node->next = NULL;
        current->next = *(&node);
        current = current->next;
    }

    printf("Before reverse: ");
    printLinkedList(head);

    printf("After reverse: ");
    head = reverse(&head);
    printLinkedList(head);

    freeLinkedList(head);
    
    return 0;
}

