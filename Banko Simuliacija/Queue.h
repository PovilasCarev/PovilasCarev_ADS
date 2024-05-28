#ifndef QUEUE_H
#define QUEUE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

// Čia galima pasikeisti reikšmę.
#define DATA_TYPE int

typedef struct Node {
    DATA_TYPE value;
    struct Node * next;
} Node;

typedef struct Queue {
    Node * head;
    Node * tail;
    int size;
} Queue;

Queue * createQueue();
bool isEmpty(Queue * q);
bool isFull(Queue * q);
// Grąžina false, jei nepavyko įterpti elemento
bool enqueue(Queue * q, DATA_TYPE value);
int dequeue(Queue * q);
int peek(Queue * q);
int getSize(Queue * q);
void destroyQueue(Queue * q);

#endif