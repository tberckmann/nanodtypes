
#ifndef NANODTYPES_H
#define NANODTYPES_H

#include <stddef.h>

enum nErrorType {
    nCodeSuccess = 0,
    nCodeNoSpace,
    nCodeBadInput,
    nCodeEmpty,
    nCodeFull
};

enum nBool {
    nFalse = 0,
    nTrue = 1
};

/*** nanoStack types ***/

struct nStack {
	char *stackData;
	unsigned int numElems;
	short managed;
	size_t elemSize;
	size_t maxElem;
};

/*** nanoStack functions ***/

enum nErrorType nStackInitM(struct nStack *s, size_t maxElem, size_t elemSize);
enum nErrorType nStackInit(struct nStack *s, void *stackData, size_t maxElem, size_t elemSize);
void nStackDestroy(struct nStack *s);
enum nErrorType nStackPush(struct nStack *s, void *dataIn);
enum nErrorType nStackPop(struct nStack *s, void *dataOut);
enum nErrorType nStackPeek(struct nStack *s, void *dataOut);
enum nBool nStackEmpty(struct nStack *s);
enum nBool nStackFull(struct nStack *s);
size_t nStackSize(struct nStack *s);


/*** nanoList types ***/

struct nListNode;

struct nList {
    struct nListNode *head;
    unsigned int numElems;
	size_t elemSize;
};

typedef enum nBool (*nListIterFunc) (void *);

/*** nanoList functions ***/

void nListInit(struct nList *l, size_t elemSize);
void nListDestroy(struct nList *l);
enum nErrorType nListInsertHead(struct nList *l, void *dataIn);
enum nErrorType nListInsertTail(struct nList *l, void *dataIn);
enum nErrorType nListRemoveHead(struct nList *l, void *dataOut);
enum nErrorType nListRemoveTail(struct nList *l, void *dataOut);
void nListForEach(struct nList *l, nListIterFunc func);
enum nBool nListEmpty(struct nList *l);
size_t nListSize(struct nList *l);

#endif