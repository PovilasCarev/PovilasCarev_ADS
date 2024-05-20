#ifndef VECTOR_H
#define VECTOR_H

// Structures
typedef int customVariable;
typedef struct{
    customVariable *array;
    int size;
    int capacity;

    void (*push_back)(struct Vector *, customVariable);
    void (*pop_back)(struct Vector *);
    void (*insert)(struct Vector *, int, customVariable);
    void (*print)(struct Vector *);
    void (*initVector)(struct Vector *, int);

}Vector;

// Functions
void push_back(Vector *vector, customVariable item);
void pop_back(Vector *vector);
void insert(Vector *vector, int index, customVariable item);
void print(Vector *vector);
void initVector(Vector *vector, int capacity);

#endif
