#include "queue.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"

#define min(a, b) a < b ? a : b
#define isNum(s) (s <= '9' && s >= '0')

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

    q->tail->next = newt;
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
        sp[length] = '\0';
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
 * Compare two strings
 * Sorted by natural sort order
 * return positive number if 1st string > 2nd string
 * return 0 if 1st string == 2nd string
 * return negative number if 1st string < 2nd string
 */
int compare(char *word1, char *word2)
{
    while (*word1 != '\0' && *word2 != '\0') {
        if (isNum(*word1) && isNum(*word2)) {
            int a = atoi(word1), b = atoi(word2);
            if (a != b) {
                return a - b;
            }
        } else if (*word1 != *word2) {
            return *word1 - *word2;
        }
        word1++;
        word2++;
    }
    return *word1 - *word2;
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
        int tmp = compare(a->value, b->value);
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
    if (size <= 1) {
        return head;
    }

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
    if (!q || !q->size) {
        return;
    }

    q->head = mergeSort(q->head, q->size);

    list_ele_t *current = q->head;
    while (current->next) {
        current = current->next;
    }
    q->tail = current;
}