#include <stdlib.h>
#include <string.h>

#include "nanodtypes.h"
#include "stack.h"

enum nErrorType
nStackInitM(struct nStack *s, size_t maxElem, size_t elemSize)
{

    enum nErrorType     ret;

    if (!(s->stackData = calloc(maxElem, elemSize)))
        return nCodeNoSpace;
    if ((ret = nStackInit(s, s->stackData, maxElem, elemSize))) {
        free(s->stackData);
        return ret;
    }
    s->managed = STACK_TRUE;
    return nCodeSuccess;
}

enum nErrorType
nStackInit(struct nStack *s, void *stackData, size_t maxElem, size_t elemSize)
{
    s->numElems = 0;
    s->stackData = stackData;   /* Check for NULL ptr? */
    s->elemSize = elemSize;     /* Check for limits */
    s->maxElem = maxElem;       /* Check for limits */
    s->managed = STACK_FALSE;
    return nCodeSuccess;
}

void
nStackDestroy(struct nStack *s)
{
    if (s->managed)
        free(s->stackData);
    s->stackData = NULL;
    s->managed = STACK_FALSE;
    s->numElems = 0;
    s->maxElem = 0;
    s->elemSize = 0;
}

enum nErrorType
nStackPush(struct nStack *s, void *dataIn)
{
    if (nStackFull(s)) {
        return nCodeFull;
    }
    memcpy(s->stackData, dataIn, s->elemSize);
    s->numElems++;
    s->stackData += s->elemSize;
    return nCodeSuccess;
}

enum nErrorType
nStackPop(struct nStack *s, void *dataOut)
{
    if (nStackEmpty(s)) {
        return nCodeEmpty;
    }
    s->stackData -= s->elemSize;
    memcpy(dataOut, s->stackData, s->elemSize);
    s->numElems--;
    return nCodeSuccess;
}

enum nErrorType
nStackPeek(struct nStack *s, void *dataOut)
{
    if (nStackEmpty(s)) {
        return nCodeEmpty;
    }
    memcpy(dataOut, s->stackData - s->elemSize, s->elemSize);
    return nCodeSuccess;
}

enum nBool
nStackEmpty(struct nStack *s)
{
    return s->numElems <= 0;
}

enum nBool
nStackFull(struct nStack *s)
{
    return s->numElems >= s->maxElem;
}

size_t
nStackSize(struct nStack *s)
{
    return s->numElems;
}
