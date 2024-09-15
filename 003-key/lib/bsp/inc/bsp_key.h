#ifndef BSP_KEY_H
#define BSP_KEY_H

#include "imx6ull.h"

enum keyvalue{
    KEY_NONE = 0,
    KEY0_VALUE = 1,
};

void key_init(void);

int key_getvalue(void);

#endif // BSP_KEY_H

