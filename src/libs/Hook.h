#ifndef HOOK_H
#define HOOK_H
#include "libs/FPointer.h"

class Hook : public FPointer {
    public:
        Hook();
        uint16_t         frequency;
        uint16_t         counter;
};

#endif
