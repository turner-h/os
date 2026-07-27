#ifndef LIST_H
#define LIST_H

struct LinkedListNode {
    void* data;
    struct LinkedListNode* next;
};

typedef struct {
    int size;
    struct LinkedListNode* front;
    struct LinkedListNode* back;
} LinkedList;

void ll_add(LinkedList* list, void* data);
void* ll_remove(LinkedList* list, int idx);
void* ll_get(LinkedList* list, int idx);

#endif