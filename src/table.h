#ifndef TABLE_H
#define TABLE_H

struct nTableNode {
    struct nTableNode              *l, *r;
    short                           bit;
    void                           *key, *value;
};

#endif
