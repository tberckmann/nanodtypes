#include "nanodtypes.h"
#include "test.h"

static struct nList             simpleList;

static enum nBool
nListSimpleEmptyCheck()
{
    nListInit(&simpleList, 1);
    return nListEmpty(&simpleList);
}

/* Depends on simpleEmptyCheck */
static enum nBool
nListsimpleEmptySizeCheck()
{
    return nListSize(&simpleList) == 0;
}

struct testInfo                 listTests[] = {

    /* Simple stack */
    {nListSimpleEmptyCheck, "Simple list reads initially empty"},
    {nListsimpleEmptySizeCheck, "Simple list size reads initially zero"},

    {NULL, ""}

};
