#ifndef BSP_KEY_H
#define BSP_KEY_H

#include "imx6ull.h"

enum keyvalue{
    KEY_NONE = 0,
    KEY0_VALUE = 1,
};

void Key_init(void);

int Key_getValue(void);

#endif // BSP_KEY_H

