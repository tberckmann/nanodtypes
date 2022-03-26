#include "nanodtypes.h"
#include "test.h"

struct nTable                   simpleTable, leftTable;
const char                      skeys[] = {'A', 'B', 'C'};
const char                      svalues[] = {'1', '2', '3'};
const short                     keys[] = {2, 1, 0, -1};
const short                     vals[] = {102, 101, 100, -100};

static enum nBool
simpleAbsent()
{
    char                dataOut = 'X';
    nTableInit(&simpleTable, sizeof(char), sizeof(char));
    if (!nTableEmpty(&simpleTable))
        return nFalse;
    if (nCodeNotFound != nTablePeek(&simpleTable, skeys, &dataOut))
        return nFalse;
    if (dataOut != 'X')
        return nFalse;
    if (nCodeNotFound != nTableRemove(&simpleTable, skeys))
        return nFalse;
    return nTrue;
}

static enum nBool
simpleInsert()
{
    char                dataOut = 'X';
    if (nTableInsert(&simpleTable, skeys + 1, svalues + 1))
        return nFalse;
    if (nTablePeek(&simpleTable, skeys + 1, &dataOut))
        return nFalse;
    if (dataOut != svalues[1])
        return nFalse;
    return nTrue;
}

static enum nBool
removeAbsent()
{
    if (nCodeNotFound != nTableRemove(&simpleTable, skeys))
        return nFalse;
    return nTrue;
}

static enum nBool
tripleInsert()
{
    int                 elem;
    char                dataOut;
    size_t              expectedSize = 1;

    for (elem = 0; elem < 3; elem++) {
        dataOut = 'X';
        if (nTablePeek(&simpleTable, skeys + elem, &dataOut))
            expectedSize++;
        if (nTableInsert(&simpleTable, skeys + elem, svalues + elem))
            return nFalse;
        if (nTablePeek(&simpleTable, skeys + elem, &dataOut))
            return nFalse;
        if (dataOut != svalues[elem])
            return nFalse;
        if (nTableSize(&simpleTable) != expectedSize)
            return nFalse;
    }
    for (elem = 0; elem < 3; elem++) {
        dataOut = 'X';
        if (nTablePeek(&simpleTable, skeys + elem, &dataOut))
            return nFalse;
        if (dataOut != svalues[elem])
            return nFalse;
    }

    return nTrue;
}

static enum nBool
tripleDelete()
{
    int                 elem;
    char                dataOut;
    size_t              expectedSize = 3;

    for (elem = 0; elem < 3; elem++) {
        dataOut = 'X';
        if (nTableRemove(&simpleTable, skeys + elem))
            return nFalse;
        if (nCodeNotFound != nTablePeek(&simpleTable, skeys + elem, &dataOut))
            return nFalse;
        expectedSize--;
        if (nTableSize(&simpleTable) != expectedSize)
            return nFalse;
    }

    return nTrue;
}

static enum nBool
addLeft()
{
    int                 kIdx;
    short               dataOut;

    nTableInit(&leftTable, sizeof(short), sizeof(short));
    for (kIdx = 0; kIdx < 3; kIdx++) {

        dataOut = 555;
        if (nTableInsert(&leftTable, keys + kIdx, vals + kIdx))
            return nFalse;
        if (nTablePeek(&leftTable, keys + kIdx, &dataOut))
            return nFalse;
        if (dataOut != vals[kIdx])
            return nFalse;
        if (nCodeNotFound != nTablePeek(&simpleTable, keys + 2, &dataOut))
            return nFalse;
    }
    return nTrue;
}

static enum nBool
removeLeft()
{
    int                 kIdx;
    short               dataOut;

    for (kIdx = 2; kIdx >= 0; kIdx--) {
        if (nTableRemove(&leftTable, keys + kIdx))
            return nFalse;
        if (nCodeNotFound != nTablePeek(&leftTable, keys + kIdx, &dataOut))
            return nFalse;
    }
    return nTrue;
}

struct testInfo                 tableTests[] = {

    /* Simple table */
    {simpleAbsent, "Searching for or removing absent element fails"},
    {simpleInsert, "Add and check one element succeeds"},
    {removeAbsent, "Trying to remove absent element produces error"},
    {tripleInsert, "Add and check three elements succeeds"},
    {tripleDelete, "Removing three elements succeeds"},
    {addLeft, "Adding zero element to the left succeeds"},
    {removeLeft, "Removing these elements succeeds"},

    {NULL, ""}

};
