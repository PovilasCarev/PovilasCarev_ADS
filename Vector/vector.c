#include <stdio.h>
#include <stdlib.h>
#include "vector.h"

// Vector Methods
void push_back(Vector *vector, customVariable item){
    if (vector->size >= vector->capacity) {
        vector->capacity *= 2;
        vector->array = (customVariable *)realloc(vector->array, vector->capacity * sizeof(customVariable));
    }

    vector->array[vector->size++] = item;
}

void pop_back(Vector *vector){
    if(vector->size > 0) vector->size--;
}

void insert(Vector *vector, int index, customVariable item){
    if(index < 0 || index > vector->size){
        printf("Invalid index! \n");
        return;
    }

    if(vector->size >= vector->capacity){
        vector->capacity *= 2;
        vector->array = (customVariable *)realloc(vector->array, vector->capacity * sizeof(customVariable));
    }

    for(int i = vector->size; i > index; i--){
        vector->array[i] = vector->array[i-1];
    }

    vector->array[index] = item;
    vector->size++;
}

void print(Vector *vector){
    for (int i = 0; i < vector->size; i++) {
        printf("%d ", vector->array[i]);
    }
}

void initVector(Vector *vector, int capacity){
    vector->array = (customVariable *)malloc(capacity * sizeof(customVariable));
    vector->size = 0;
    vector->capacity = capacity;

    vector->push_back = push_back;
    vector->pop_back = pop_back;
    vector->insert = insert;
    vector->print = print;
}
