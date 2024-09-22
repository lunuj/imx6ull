#ifndef BSP_KEY_H
#define BSP_KEY_H

#include "imx6ull.h"

enum keyvalue{
    KEY_NONE = 0,
    KEY0_VALUE = 1,
};

typedef struct {
    uint8_t triger;
    uint8_t cont;
} key_type_t;

void Key_init(void);

int Key_getValue(void);

void Key_read(key_type_t *key_read);

#endif // BSP_KEY_H

