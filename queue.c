#include "queue.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"

#define min(a, b) a < b ? a : b

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));
    if (!q) {
        return NULL;
    }
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    if (!q) {
        return;
    }
    /* Free the list elements and the strings */
    list_ele_t *current = q->head;
    while (current) {
        q->head = current->next;
        free(current->value);
        current->value = NULL;
        current->next = NULL;
        free(current);
        current = q->head;
    }
    /* Free queue structure */
    free(q);
}

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    if (!q) {
        return false;
    }

    list_ele_t *newh = NULL;
    newh = malloc(sizeof(list_ele_t));
    if (!newh) {
        return false;
    }

    /* allocate space and copy the string into it */
    newh->value = malloc(strlen(s) + 1);
    if (!newh->value) {
        free(newh);
        return false;
    }
    memcpy(newh->value, s, strlen(s) + 1);

    newh->next = q->head;
    q->head = newh;
    if (!q->size) {
        q->tail = newh;
    }
    q->size++;
    return true;
}

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    if (!q) {
        return false;
    }

    list_ele_t *newt = NULL;
    newt = malloc(sizeof(list_ele_t));
    if (!newt) {
        return false;
    }

    /* allocate space and copy the string into it */
    newt->value = malloc(strlen(s) + 1);
    if (!newt->value) {
        free(newt);
        return false;
    }
    memcpy(newt->value, s, strlen(s) + 1);

    newt->next = NULL;
    if (!q->size) {
        q->head = newt;
    }
    q->tail = newt;
    q->size++;
    return true;
}

/*
 * Attempt to remove element from head of queue.
 * Return true if successful.
 * Return false if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 * The space used by the list element and the string should be freed.
 */
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    if (!q || !q->size) {
        return false;
    }

    list_ele_t *oldh = q->head;
    q->head = q->head->next;

    if (sp) {
        size_t length = min(strlen(oldh->value), bufsize - 1);
        memcpy(sp, oldh->value, length);
        sp[length + 1] = '\0';
    }

    oldh->next = NULL;
    free(oldh->value);
    free(oldh);
    q->size--;
    if (!q->size) {
        q->tail = NULL;
    }
    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    return !q ? 0 : q->size;
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    if (!q || !q->size) {
        return;
    }

    list_ele_t *cursor = q->head, *prev = NULL;
    while (*cursor) {
        list_ele_t *next = cursor->next;
        cursor->next = prev;
        prev = cursor;
        cursor = next;
    }

    q->tail = q->head;
    q->head = prev;
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
void q_sort(queue_t *q)
{
    /* TODO: You need to write the code for this function */
    /* TODO: Remove the above comment when you are about to implement. */
}
