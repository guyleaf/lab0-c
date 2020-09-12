#include "queue.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"

#define min(a, b) (a < b ? a : b)
#define isNum(s) (s <= '9' && s >= '0')
#define IS_NULL_EMPTY(s) (!s)

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));
    if (IS_NULL_EMPTY(q))
        return NULL;

    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    if (IS_NULL_EMPTY(q))
        return;

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
    q->size = 0;
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
    if (IS_NULL_EMPTY(q))
        return false;

    list_ele_t *newh = NULL;
    newh = malloc(sizeof(list_ele_t));
    if (IS_NULL_EMPTY(newh))
        return false;

    /* allocate space and copy the string into it */
    newh->value = malloc(strlen(s) + 1);
    if (IS_NULL_EMPTY(newh->value)) {
        free(newh);
        return false;
    }
    memcpy(newh->value, s, strlen(s) + 1);
    newh->next = q->head;

    q->head = newh;
    if (IS_NULL_EMPTY(q->size))
        q->tail = newh;
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
    if (IS_NULL_EMPTY(q))
        return false;

    list_ele_t *newt = NULL;
    newt = malloc(sizeof(list_ele_t));
    if (IS_NULL_EMPTY(newt))
        return false;

    /* allocate space and copy the string into it */
    newt->value = malloc(strlen(s) + 1);
    if (IS_NULL_EMPTY(newt->value)) {
        free(newt);
        return false;
    }
    memcpy(newt->value, s, strlen(s) + 1);
    newt->next = NULL;

    if (IS_NULL_EMPTY(q->size))
        q->head = newt;
    else
        q->tail->next = newt;
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
    if (IS_NULL_EMPTY(q) || IS_NULL_EMPTY(q->size))
        return false;

    list_ele_t *oldh = q->head;
    q->head = q->head->next;

    if (!IS_NULL_EMPTY(sp)) {
        size_t length = min(strlen(oldh->value), bufsize - 1);
        memcpy(sp, oldh->value, length);
        sp[length] = '\0';
    }

    oldh->next = NULL;
    free(oldh->value);
    free(oldh);
    q->size--;

    if (IS_NULL_EMPTY(q->size))
        q->tail = NULL;
    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    return IS_NULL_EMPTY(q) ? 0 : q->size;
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
    if (IS_NULL_EMPTY(q) || IS_NULL_EMPTY(q->size))
        return;

    list_ele_t *cursor = q->head, *prev = NULL;
    while (cursor) {
        list_ele_t *next = cursor->next;
        cursor->next = prev;
        prev = cursor;
        cursor = next;
    }

    q->tail = q->head;
    q->head = prev;
}

/*
 * Merge two lists in ascending order
 * return head of list
 */
list_ele_t *merge(list_ele_t *a, list_ele_t *b)
{
    list_ele_t newh;
    list_ele_t *current, *head;
    newh.next = NULL;
    current = head = &newh;

    while (a && b) {
        int tmp = strcmp(a->value, b->value);
        if (tmp > 0) {
            current->next = b;
            current = current->next;
            b = b->next;
        } else {
            current->next = a;
            current = current->next;
            a = a->next;
        }
        current->next = NULL;
    }

    if (a)
        current->next = a;
    if (b)
        current->next = b;

    head = head->next;
    return head;
}

/*
 * Split list into two lists
 * return head of 2nd splited list
 */
list_ele_t *splitList(list_ele_t *head, size_t size)
{
    for (size_t halfSize = size / 2; halfSize > 1; halfSize--) {
        head = head->next;
    }

    list_ele_t *tmp = head;
    head = head->next;
    tmp->next = NULL;
    return head;
}

/*
 * Do merge sort algorithm
 * return head of sorted list
 */
list_ele_t *mergeSort(list_ele_t *head, size_t size)
{
    if (size <= 1)
        return head;

    list_ele_t *node = splitList(head, size);
    size_t halfSize = size / 2;
    return merge(mergeSort(head, halfSize), mergeSort(node, size - halfSize));
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
void q_sort(queue_t *q)
{
    if (IS_NULL_EMPTY(q) || IS_NULL_EMPTY(q->size))
        return;

    q->head = mergeSort(q->head, q->size);

    list_ele_t *current = q->head;
    while (current->next) {
        current = current->next;
    }
    q->tail = current;
}