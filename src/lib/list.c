#include "../lib/list.h"
#include "../cpu/memory.h"

void ll_add(LinkedList* list, void* data) {
    struct LinkedListNode* node = kmalloc(sizeof(struct LinkedListNode));
    node->data = data;
    node->next = NULL;

    if (list->front == NULL) {
        list->front = node;
    }

    if (list->back != NULL) {
        list->back->next = node;
    }

    list->back = node;

    list->size++;
}

void* ll_remove(LinkedList* list, int idx) {
    struct LinkedListNode* temp = list->front;
    for (int i = 0; i < idx - 1; i++) {
        temp = temp->next;
    }

    struct LinkedListNode* node_to_remove = temp->next;
    temp->next = temp->next->next;

    void* data = node_to_remove->data;
    kfree(node_to_remove);

    list->size--;
    return data;
}

void* ll_get(LinkedList* list, int idx) {
    struct LinkedListNode* temp = list->front;
    for (int i = 0; i < idx; i++) {
        temp = temp->next;
    }

    return temp->data;
}