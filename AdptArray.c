#include "AdptArray.h"
#include <stdlib.h>
#include <stdio.h>

#define INIT_CAPACITY 10

struct AdptArray_ {
    PElement* elements;
    int size;
    int capacity;
    COPY_FUNC copyFunc;
    DEL_FUNC delFunc;
    PRINT_FUNC printFunc;
    Result last_error;
};

PAdptArray CreateAdptArray(COPY_FUNC copyFunc, DEL_FUNC delFunc, PRINT_FUNC printFunc) {
    PAdptArray arr = malloc(sizeof(struct AdptArray_));
    if (arr == NULL) {
        return NULL;
    }
    arr->elements = malloc(sizeof(PElement) * INIT_CAPACITY);
    if (arr->elements == NULL) {
        free(arr);
        return NULL;
    }
    arr->size = 0;
    arr->capacity = INIT_CAPACITY;
    arr->copyFunc = copyFunc;
    arr->delFunc = delFunc;
    arr->printFunc = printFunc;
    arr->last_error = SUCCESS;
    return arr;
}

void DeleteAdptArray(PAdptArray arr) {
    if (arr == NULL) {
        return;
    }
    for (int i = 0; i < arr->size; i++) {
        if(arr->elements[i] != NULL)
           arr->delFunc(arr->elements[i]);
    }
    free(arr->elements);
    free(arr);
}

Result SetAdptArrayAt(PAdptArray arr, int index, PElement element) {
    if (arr == NULL || index < 0) {
        return FAIL;
    }
    if (index >= arr->capacity) {
        int newCapacity = index + 1;
        PElement* newElements = realloc(arr->elements, sizeof(PElement) * newCapacity);
        if (newElements == NULL) {
            arr->last_error = FAIL;
            return FAIL;
        }
        arr->elements = newElements;
        arr->capacity = newCapacity;
    }
    if (index >= arr->size) {
        arr->size = index + 1;
    }
    PElement copy = arr->copyFunc(element);
    if (copy == NULL) {
        arr->last_error = FAIL;
        return FAIL;
    }
    arr->elements[index] = copy;
    arr->last_error = SUCCESS;
    return SUCCESS;
}

PElement GetAdptArrayAt(PAdptArray arr, int index) {
    if (arr == NULL || index < 0 || index >= arr->size || arr->elements[index] == NULL) {
        return NULL;
    }
    arr->last_error = SUCCESS;
    return arr->copyFunc(arr->elements[index]);
}

int GetAdptArraySize(PAdptArray arr) {
    if (arr == NULL) {
        return -1;
    }
    return arr->size;
}

void PrintDB(PAdptArray arr) {
    if (arr == NULL) {
        return;
    }
    for (int i = 0; i < arr->size; i++) {
        if(arr->elements[i] != NULL)
            arr->printFunc(arr->elements[i]);
    }
}