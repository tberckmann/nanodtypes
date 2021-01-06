#include <stdio.h>

#include "test.h"

extern struct testInfo          stackTests[], listTests[], tableTests[];

struct {
    struct testInfo                *testDefs;
    char                           *title;
}                              *curSet, testSetList[] = {
    {
        stackTests, "Stack Tests"
    },
    {
        listTests, "List Tests"
    },
    {
        tableTests, "Table Tests"
    },

    {
        NULL, ""
    }
};

static void
printMsg(char *msg, enum nBool success)
{
    char               *resultText;
    char               *failText = "\033[0;31mFAILURE\033[0m";
    char               *succeedText = "Success";

    resultText = success ? succeedText : failText;
    printf("%-60.60s[%s]\n", msg, resultText);
}

int
main()
{

    struct testInfo    *nextTest;
    enum nBool          finalResult = nTrue;

    curSet = testSetList;

    printf("\nTest suite for nanodtypes\n");

    while (curSet->testDefs) {

        printf("\n");
        printf("%s\n", curSet->title);
        nextTest = curSet->testDefs;
        while (nextTest->func) {
            enum nBool          res;
            res = (*(nextTest->func)) ();
            if (!res)
                finalResult = nFalse;
            printMsg(nextTest->description, res);
            nextTest++;
        }
        curSet++;
    }

    printf("\n");
    printMsg("Final result", finalResult);

    return 0;
}
