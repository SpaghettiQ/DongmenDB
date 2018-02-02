//
// Created by Sam on 2018/1/31.
//

#include "arraylist.h"

#define INITIAL_ARRAYLIST_SIZE 10

arraylist *arraylist_create()
{
    arraylist *list = malloc(sizeof(arraylist));
    if (list == NULL) {
        fprintf(stderr, "Insufficient memory to initialize arraylist.\n");
        return NULL;
    }
    list->capacity = INITIAL_ARRAYLIST_SIZE;
    list->size = 0;
    list->data = calloc(list->capacity, sizeof(void *));
    if (list->data == NULL) {
        free(list);
        return NULL;
    }

    return list;
}

void arraylist_destroy(arraylist **list)
{
    free((*list)->data);
    free(*list);
    *list = NULL;
}

int arraylist_add(arraylist *list, void *element)
{
    if (list->size >= list->capacity) {
        list->capacity *= 2;
        list->data = realloc(list->data, sizeof(void *) * list->capacity);
        if (list->data == NULL) {
            fprintf(stderr, "Insufficient memory to resize arraylist.\n");
            return 0;
        }
    }

    list->data[list->size++] = element;

    return 1;
}

int arraylist_remove_by_element(arraylist *list, void *element){
    int i = 0 ;
    for (; i <=list->size-1;i++){
        if (list->data[i] == element){
            break;
        }
    }
    arraylist_remove(list, i);

}

int arraylist_remove(arraylist *list, size_t index)
{
    size_t i;
    if (index >= list->size) return 0;

    for (i = index; i <list->size - 1; i++)
    {
        list->data[i] = list->data[i + 1];
    }

    list->size -= 1;

    return 1;
}

void *arraylist_get(arraylist *list, size_t index)
{
    if (index >= list->size) return NULL;

    return list->data[index];
}

int arraylist_set(arraylist *list, size_t index, void *value)
{
    if (index >= list->size) return 0;

    list->data[index] = value;
    return 1;
}

int arraylist_shrink(arraylist *list)
{
    if (list == NULL || list->data == NULL) return 0;

    list->capacity = list->size;
    list->data = realloc(list->data, sizeof(void *) * list->capacity);

    return 1;
}