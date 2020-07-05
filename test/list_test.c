#include "nanodtypes.h"
#include "test.h"

static struct nList             simpleList;

static enum nBool
nListSimpleEmptyCheck()
{
    nListInit(&simpleList, sizeof(char));
    return nListEmpty(&simpleList);
}

/* Depends on simpleEmptyCheck */
static enum nBool
nListSimpleEmptySizeCheck()
{
    return nListSize(&simpleList) == 0;
}

/* Depends on simpleEmptyCheck */
static enum nBool
nListAddRemoveHead()
{
    char                outputData = 'X', inputData = 'N';

    if (nListInsertHead(&simpleList, &inputData))
        return nFalse;
    if (nListRemoveTail(&simpleList, &outputData))
        return nFalse;
    if (inputData != outputData)
        return nFalse;
    return nTrue;
}

/* List must be empty */
static enum nBool
nListSimplePopExtra()
{
    char                dummyData;
    if (nListRemoveTail(&simpleList, &dummyData))
        return nTrue;
    else
        return nFalse;
}

/* Depends on simpleEmptyCheck */
static enum nBool
nListAddRemoveTail()
{
    char                outputData = 'A', inputData = 'B';

    if (nListInsertTail(&simpleList, &inputData))
        return nFalse;
    if (nListRemoveHead(&simpleList, &outputData))
        return nFalse;
    if (inputData != outputData)
        return nFalse;
    return nTrue;
}

/* List must be empty at this point */
static enum nBool
nListSimpleAddRemove()
{
    char                i, outData;

    for (i = 1; i <= 4; i++) {
        nListInsertHead(&simpleList, &i);       /* H 4 3 2 1 T */
    }
    if (nListRemoveHead(&simpleList, &outData)) /* H 3 2 1 T */
        return nFalse;
    if (outData != 4)
        return nFalse;
    if (nListRemoveTail(&simpleList, &outData)) /* H 3 2 T */
        return nFalse;
    if (outData != 1)
        return nFalse;
    i = 5;
    nListInsertTail(&simpleList, &i);   /* H 3 2 5 T */
    if (nListRemoveHead(&simpleList, &outData)) /* H 2 5 T */
        return nFalse;
    if (outData != 3)
        return nFalse;
    if (nListRemoveHead(&simpleList, &outData)) /* H 5 T */
        return nFalse;
    if (outData != 2)
        return nFalse;
    if (nListRemoveHead(&simpleList, &outData)) /* H T */
        return nFalse;
    if (outData != 5)
        return nFalse;
    return nListEmpty(&simpleList);
}

struct testInfo                 listTests[] = {

    /* Simple stack */
    {nListSimpleEmptyCheck, "Simple list reads initially empty"},
    {nListSimpleEmptySizeCheck, "Simple list size reads initially zero"},
    {nListAddRemoveHead, "Add single element to head and remove from tail"},
    {nListSimplePopExtra, "Removing too many elements is an error"},
    {nListAddRemoveTail, "Add single element to tail and remove from head"},
    {nListSimpleAddRemove, "Several insertions and removals"},

    {NULL, ""}

};
