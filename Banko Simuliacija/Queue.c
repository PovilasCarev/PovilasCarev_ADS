#include "Queue.h"

Queue * createQueue() {
    Queue * q = malloc(sizeof(Queue));

    q->head = NULL;
    q->tail = NULL;
    q->size = 0;

    return q;
}

bool isEmpty(Queue * q) {
    return q->size == 0;
}

bool isFull(Queue * q) {
    if(enqueue(q, 0)) {
        dequeue(q);

        return false;
    }

    return true;
}

bool enqueue(Queue * q, int value) {
    Node * newNode = malloc(sizeof(Node));
    if(newNode == NULL) return false;
    
    newNode->value = value;
    newNode->next = NULL;

    if(q->head == NULL) {
        q->head = newNode;
    }

    if(q->tail != NULL) {
        q->tail->next = newNode;
    }
    q->tail = newNode;

    q->size++;

    return true;
}

int dequeue(Queue * q) {
    if(q->head == NULL)
        return INT_MIN;

    Node * first = q->head;

    int result = first->value;

    q->head = first->next;

    free(first);

    if(q->head == NULL)
        q->tail = NULL;

    q->size--;

    return result;
}

int peek(Queue * q) {
    if(q->head == NULL)
        return INT_MIN;

    return q->head->value;
}

int getSize(Queue * q) {
    return q->size;
}

void destroyQueue(Queue * q) {
    while(!isEmpty(q)) {
        dequeue(q);
    }

    free(q);
}