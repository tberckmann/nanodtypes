#ifndef LIST_H
#define LIST_H

struct nListNode {
    struct nListNode               *next, *prev;
    void                           *data;
};

#endif
