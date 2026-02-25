#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define THREAD_COUNT 8
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

/**
 * Linked list node.
 */
struct node {
    int value;
    struct node *next;
};

/**
 * Insert at head of a linked list.
 */
void llist_insert(struct node **head, int value)
{
    struct node *n = malloc(sizeof *n);
    n->value = value;
    usleep(1); // IGNORE all usleeps! Leave them in place.
    n->next = *head;
    usleep(1);
    *head = n;
}

/**
 * Delete head of a linked list.
 */
void llist_delete(struct node **head)
{
    if (*head == NULL) {
        return;
    }

    struct node *n = *head;
    usleep(1);
    struct node *next = (*head)->next;
    usleep(1);
    *head = next;

    free(n);
}

/**
 * Append to end of linked list.
 */
void llist_append(struct node **head, int value)
{
    struct node *n = malloc(sizeof *n);
    n->value = value;
    n->next = NULL;
    if (*head == NULL) {
        *head = n;
        return;
    }

    struct node *p = *head;

    while (p->next != NULL) {
        usleep(1);
        p = p->next;
    }

    usleep(1);
    p->next = n;
}

/**
 * Traverse a linked list, calling function f() on each node in turn.
 */
void llist_traverse(struct node *head, void(*f)(struct node *))
{
    for (struct node *p = head; p != NULL; p = p->next)
        f(p);
}

/**
 * Print a single linked list node.
 */
void print_node(struct node *n)
{
    printf("[%d]%s", n->value, n->next? "->": "->NULL");
}

/**
 * Print an entire list.
 */
void llist_print(struct node *head)
{
    if (head == NULL) {
        puts("[EMPTY]");
        return;
    }

    llist_traverse(head, print_node);
    putchar('\n');
}

/**
 * Thread run function to test appends.
 */
void *run_append(void *arg)
{
    struct node **head = arg;

    pthread_mutex_lock(&lock);

    for (int i = 0; i < 10; i++) {
        llist_append(head, i);
    }

    pthread_mutex_unlock(&lock);

    return NULL;
}

/**
 * Thread run function to test inserts.
 */
void *run_insert(void *arg)
{
    struct node **head = arg;

    pthread_mutex_lock(&lock);

    for (int i = 0; i < 10; i++) {
        llist_insert(head, i);
    }

    pthread_mutex_unlock(&lock);

    return NULL;
}

/**
 * Thread run function to test inserts and deletes.
 */
void *run_insert_delete(void *arg)
{
    struct node **head = arg;

    pthread_mutex_lock(&lock);

    for (int i = 0; i < 10; i++) {
        llist_insert(head, i);
    }

    for (int i = 0; i < 10; i++) {
        llist_delete(head);
    }

    pthread_mutex_unlock(&lock);

    return NULL;
}

/**
 * Main.
 */
int main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "usage: %s 0|1|2\n", argv[0]);
        return 1;
    }

    pthread_t threads[THREAD_COUNT];

    struct node *head = NULL;

    for (int i = 0; i < THREAD_COUNT; i++) {
        switch (argv[1][0]) {
        case '0':
            pthread_create(threads + i, NULL, run_append, &head);
            break;
        case '1':
            pthread_create(threads + i, NULL, run_insert, &head);
            break;
        case '2':
            pthread_create(threads + i, NULL, run_insert_delete, &head);
            break;
        }
    }

    for (int i = 0; i < THREAD_COUNT; i++) {
        pthread_join(threads[i], NULL);
    }

    llist_print(head);
}