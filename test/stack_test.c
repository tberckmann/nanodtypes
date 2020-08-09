#include "nanodtypes.h"
#include "test.h"

/* Tests on a stack that only holds a single element */

static struct nStack            simpleStack;    /* Stack that can fit one
                                                 * element */
static char                     simplePushData = 'a';   /* Data pushed in simple
                                                         * push */
static char                     simplePopData;  /* Data popped from simple
                                                 * pop */

static enum nBool
simpleCreate()
{
    if (!nStackInitM(&simpleStack, 1, 1))
        return nTrue;
    else
        return nFalse;
}

/* Depends on simpleCreate */
static enum nBool
simpleEmptyCheck()
{
    return nStackEmpty(&simpleStack);
}

/* Depends on simpleCreate */
static enum nBool
simpleEmptySizeCheck()
{
    return nStackSize(&simpleStack) == 0;
}

/* Depends on simpleCreate */
static enum nBool
simplePush()
{
    if (!nStackPush(&simpleStack, &simplePushData))
        return nTrue;
    else
        return nFalse;
}

/* Depends on simplePush */
static enum nBool
simpleFullCheck()
{
    return nStackFull(&simpleStack);
}

/* Depends on simplePush */
static enum nBool
simpleFullSizeCheck()
{
    return nStackSize(&simpleStack) == 1;
}

/* Depends on simplePush */
static enum nBool
simplePushExtra()
{
    char                dummyData = 'x';
    if (nStackPush(&simpleStack, &dummyData))
        return nTrue;
    else
        return nFalse;
}

/* Depends on simplePush */
static enum nBool
simplePeek()
{
    char                dataOut;
    if (nStackPeek(&simpleStack, &dataOut))
        return nFalse;
    if (dataOut != simplePushData)
        return nFalse;
    return nTrue;
}

/* Depends on simplePush */
static enum nBool
simplePop()
{
    if (!nStackPop(&simpleStack, &simplePopData))
        return nTrue;
    else
        return nFalse;
}

/* Depends on simplePop */
static enum nBool
simplePopCheck()
{
    if (simplePopData == simplePushData)
        return nTrue;
    else
        return nFalse;
}

/* Depends on simplePop */
static enum nBool
simplePopExtra()
{
    char                dummyData;
    if (nCodeEmpty != nStackPop(&simpleStack, &dummyData))
        return nFalse;
    if (nCodeEmpty != nStackPeek(&simpleStack, &dummyData))
        return nFalse;
    return nTrue;
}

static enum nBool
simpleDestroy()
{
    nStackDestroy(&simpleStack);
    nStackDestroy(&simpleStack);/* Confirm no double free */
    if (!nStackFull(&simpleStack))
        return nFalse;
    if (!nStackEmpty(&simpleStack))
        return nFalse;
    return nTrue;
}

/* Tests on a complex stack with many elements */

#define CMPLX_ELEM_SIZE 3
#define CMPLX_NUM_ELEM 5
#define CMPLX_BUF_SIZE_BYTES (CMPLX_ELEM_SIZE * CMPLX_NUM_ELEM)
#define PAD_CHAR 'N'
#define FILL_CHAR 'X'

static char                     cmplxDataBuf[CMPLX_BUF_SIZE_BYTES + 2];
static struct nStack            complexStack;

static enum nBool
complexCreate()
{
    cmplxDataBuf[0] = PAD_CHAR;
    cmplxDataBuf[CMPLX_BUF_SIZE_BYTES + 1] = PAD_CHAR;
    if (nStackInit(&complexStack, cmplxDataBuf + 1, CMPLX_NUM_ELEM, CMPLX_ELEM_SIZE))
        return nFalse;
    return nTrue;
}

static enum nBool
complexAdd()
{
    int                 modVal = 17;
    int                 i;
    char                inDataBuf[CMPLX_ELEM_SIZE];
    for (i = 0; i < CMPLX_ELEM_SIZE; i++) {
        inDataBuf[i] = FILL_CHAR;
    }
    for (i = 0; i < CMPLX_NUM_ELEM + 1; i++) {
        inDataBuf[0] = i % modVal;
        nStackPush(&complexStack, inDataBuf);
    }
    for (i = CMPLX_NUM_ELEM - 1; i >= 0; i--) {
        nStackPop(&complexStack, inDataBuf);
        if (inDataBuf[0] != i % modVal)
            return nFalse;
    }
    return nTrue;
}

static enum nBool
complexCheckPad()
{
    if (cmplxDataBuf[0] != PAD_CHAR || cmplxDataBuf[CMPLX_BUF_SIZE_BYTES + 1] != PAD_CHAR)
        return nFalse;
    return nTrue;
}

/* Tests on a numbered stack */

static struct nStack            numberStack;

static enum nBool
numberPushPop()
{
    short               in = 15, out = 0;
    if (nStackInitM(&numberStack, 10, sizeof(short)))
        return nFalse;
    nStackPush(&numberStack, &in);
    nStackPop(&numberStack, &out);
    nStackPop(&numberStack, &out);      /* Second pop doesn't overwrite */
    if (out != in)
        return nFalse;
    return nTrue;
}

/* Test list */

struct testInfo                 stackTests[] = {

    /* Simple stack */
    {simpleCreate, "Creation of simple stack succeeds"},
    {simpleEmptyCheck, "Simple stack reads initially empty"},
    {simpleEmptySizeCheck, "Simple stack size reads initially zero"},
    {simplePush, "Push to simple stack succeeds"},
    {simpleFullCheck, "Simple stack reads full"},
    {simpleFullSizeCheck, "Simple stack size reads one"},
    {simplePushExtra, "Pushing too many elements fails"},
    {simplePeek, "Data from peeking top is correct"},
    {simplePop, "Pop from simple stack succeeds"},
    {simplePopCheck, "Data from pop was correct"},
    {simplePopExtra, "Popping too many elements fails"},
    {simpleEmptyCheck, "Simple stack reads empty again"},
    {simpleEmptySizeCheck, "Simple stack size reads zero again"},
    {simpleDestroy, "Destroyed simple stack is empty and full"},

    /* Complex stack */
    {complexCreate, "Creation of complex stack succeeds"},
    {complexAdd, "Adding and popping many elements succeeds"},
    {complexCheckPad, "Complex padding untouched"},

    /* Numbered stack */
    {numberPushPop, "Example with short member works"},

    {NULL, ""}

};
