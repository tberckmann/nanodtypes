#include <stdlib.h>             /* NULL */
#include <string.h>             /* memcpy */

#include "nanodtypes.h"

#include "list.h"

/* Helper functions */

/* Adding the first element to an empty list is special */
static void
insertFirstItem(struct nList *l, struct nListNode *newNode)
{
    newNode->next = newNode;
    newNode->prev = newNode;
    l->head = newNode;
}

static void
insertNodeBefore(struct nListNode *newNode, struct nListNode *curNode)
{
    curNode->prev->next = newNode;
    newNode->prev = curNode->prev;
    curNode->prev = newNode;
    newNode->next = curNode;
}

/* Allocate a new node and place the data into it; return NULL on failure */
static struct nListNode        *
allocNode(struct nList *l, void *dataIn)
{
    struct nListNode   *newNode;

    if (!(newNode = malloc(sizeof(struct nListNode)))) {
        return NULL;
    }
    if (!(newNode->data = malloc(l->elemSize))) {
        free(newNode);
        return NULL;
    }
    memcpy(newNode->data, dataIn, l->elemSize);

    return newNode;
}

static enum nErrorType
nListInsert(enum nBool atHead, struct nList *l, void *dataIn)
{
    struct nListNode   *newNode;

    if (!(newNode = allocNode(l, dataIn)))
        return nCodeNoSpace;

    if (l->head) {
        insertNodeBefore(newNode, l->head);
        if (atHead)
            l->head = newNode;
    } else {
        insertFirstItem(l, newNode);
    }

    return nCodeSuccess;
}

/* API functions */

void
nListInit(struct nList *l, size_t elemSize)
{
    l->head = NULL;
    l->numElems = 0;
    l->elemSize = elemSize;
}

enum nErrorType
nListInsertHead(struct nList *l, void *dataIn)
{
    return nListInsert(nTrue, l, dataIn);
}

enum nErrorType
nListInsertTail(struct nList *l, void *dataIn)
{
    return nListInsert(nFalse, l, dataIn);
}

enum nBool
nListEmpty(struct nList *l)
{
    if (nListSize(l) == 0)
        return nTrue;
    return nFalse;
}

size_t
nListSize(struct nList *l)
{
    return l->numElems;
}