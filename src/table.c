#include <string.h>
#include <stdlib.h>

#include "nanodtypes.h"
#include "table.h"

const unsigned short            bitsPerByte = 8;

/* Helper functions */

static struct nTableNode       *
allocNode(struct nTable *t, const void *keyIn, const void *valueIn, short bit)
{
    struct nTableNode  *newNode;

    if (!(newNode = malloc(sizeof(struct nTableNode))))
        goto err;
    if (!(newNode->key = malloc(t->keySize)))
        goto errN;
    if (!(newNode->value = malloc(t->valueSize)))
        goto errK;
    memcpy(newNode->key, keyIn, t->keySize);
    memcpy(newNode->value, valueIn, t->valueSize);
    newNode->bit = bit;
    return newNode;

errK:
    free(newNode->key);
errN:
    free(newNode);
err:
    return NULL;

}

static enum nBool
bitSet(unsigned short bitOff, const void *key)
{

    unsigned short      byteOffset = bitOff / bitsPerByte;
    unsigned short      innerBitOffset = bitOff % bitsPerByte;

    return (*((unsigned char *)key + byteOffset) >> (bitsPerByte - 1 - innerBitOffset)) & 1;

}

static short
findBitDiff(size_t keySize, const void *key1, const void *key2)
{
    unsigned char       curByte1, curByte2, *key1Byte, *key2Byte, curByteMask;
    size_t              byteOffset;
    unsigned short      innerBitOffset;
    key1Byte = (unsigned char *)key1;
    key2Byte = (unsigned char *)key2;

    for (byteOffset = 0; byteOffset < keySize; byteOffset++, key1Byte++, key2Byte++) {

        curByte1 = *key1Byte;
        if (key2)
            curByte2 = *key2Byte;
        else
            curByte2 = 0;
        curByteMask = 0x80;

        for (innerBitOffset = 0; innerBitOffset < bitsPerByte; innerBitOffset++) {
            if ((curByte1 & curByteMask) != (curByte2 & curByteMask))
                return (byteOffset * bitsPerByte) + innerBitOffset;
            curByteMask >>= 1;
        }
    }

    return keySize * bitsPerByte;
}

static void
lookupStep(struct nTableNode *node, const void *srchKey, struct nTableNode *parentNode,
           struct nTableNode **closestOut, struct nTableNode **parentOut)
{
    short               prevBit;

    if (parentNode) {
        prevBit = parentNode->bit;
    } else {
        prevBit = -1;
    }

    if (node->bit > prevBit) {
        if (bitSet(node->bit, srchKey)) {
            if (node->r) {
                lookupStep(node->r, srchKey, node, closestOut, parentOut);
                return;
            }
        } else {
            if (node->l) {
                lookupStep(node->l, srchKey, node, closestOut, parentOut);
                return;
            }
        }
    }
    *closestOut = node;
    *parentOut = parentNode;
    return;
}

static struct nTableNode       *
insert_step(struct nTable *t, struct nTableNode *node, short diffBit,
            const void *diffKey, const void *newKey, const void *newItem, short parentBit)
{
    struct nTableNode  *newLink;
    short               newBitDiff;

    if (node->bit > diffBit || node->bit <= parentBit) {
        newLink = allocNode(t, newKey, newItem, diffBit);
        if (bitSet(diffBit, newKey)) {
            newLink->r = newLink;
            newLink->l = node;
        } else {
            newLink->r = node;
            newLink->l = newLink;
        }
        return newLink;
    } else {
        if (bitSet(node->bit, newKey)) {
            node->r = insert_step(t, node->r, diffBit, diffKey, newKey, newItem, node->bit);
        } else {
            if (node->l) {
                node->l = insert_step(t, node->l, diffBit, diffKey, newKey, newItem, node->bit);
            } else {
                newBitDiff = findBitDiff(t->keySize, newKey, NULL);
                newLink = allocNode(t, newKey, newItem, newBitDiff);
                newLink->r = newLink;
                newLink->l = NULL;
                node->l = newLink;
            }
        }
        return node;
    }
}

/* API functions */

void
nTableInit(struct nTable *t, size_t keySize, size_t valueSize)
{
    t->head = NULL;
    t->numElems = 0;
    t->keySize = keySize;
    t->valueSize = valueSize;
}

enum nErrorType
nTableInsert(struct nTable *t, const void *key, const void *dataIn)
{
    struct nTableNode  *closestOut, *parentOut, *newNode;
    short               tgtBit;

    if (t->head) {

        lookupStep(t->head, key, NULL, &closestOut, &parentOut);
        if (!memcmp(closestOut->key, key, t->keySize)) {
            memcpy(closestOut->value, dataIn, t->valueSize);
            return nCodeSuccess;
        }
        tgtBit = findBitDiff(t->keySize, key, closestOut->key);
        t->head = insert_step(t, t->head, tgtBit, closestOut->key, key, dataIn, -1);

    } else {
        tgtBit = findBitDiff(t->keySize, key, NULL);
        newNode = allocNode(t, key, dataIn, tgtBit);
        newNode->r = newNode;
        newNode->l = NULL;
        t->head = newNode;
    }

    return nCodeSuccess;
}

enum nErrorType
nTablePeek(struct nTable *tab, const void *key, void *dataOut)
{
    struct nTableNode  *closestOut, *parentOut;

    if (!tab->head)
        return nCodeNotFound;

    lookupStep(tab->head, key, NULL, &closestOut, &parentOut);
    if (memcmp(closestOut->key, key, tab->keySize)) {
        return nCodeNotFound;
    } else {
        memcpy(dataOut, closestOut->value, tab->valueSize);
        return nCodeSuccess;
    }
}
