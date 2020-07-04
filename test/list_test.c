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

struct testInfo                 listTests[] = {

    /* Simple stack */
    {nListSimpleEmptyCheck, "Simple list reads initially empty"},
    {nListSimpleEmptySizeCheck, "Simple list size reads initially zero"},
    {nListAddRemoveHead, "Add single element to head and remove from tail"},
    {nListAddRemoveTail, "Add single element to tail and remove from head"},

    {NULL, ""}

};
