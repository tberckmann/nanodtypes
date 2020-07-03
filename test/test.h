#ifndef TEST_H
#define TEST_H

#include "nanodtypes.h"

typedef enum nBool              (*testFunction) ();

struct testInfo {
    testFunction                    func;
    char                           *description;
};

#endif
