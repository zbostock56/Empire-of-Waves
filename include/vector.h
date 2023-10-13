/*
    Dynamic array implemented in C from "https://eddmann.com/posts/implementing-a-dynamic-vector-array-in-c/"
*/
#include <stdio.h>
#include <stdlib.h>
#define VECTOR_INIT_CAPACITY 4
typedef struct vector {
    void **items;
    int capacity;
    int total;
} vector;

void vector_init(vector *);
int vector_total(vector *);
void vector_resize(vector *, int);
void vector_add(vector *, void *);
void vector_set(vector *, int, void *);
void *vector_get(vector *, int);
void vector_delete(vector *, int);
void vector_free(vector *);
