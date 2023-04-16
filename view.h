#ifndef _VIEW_H_
#define _VIEW_H_

#include "def.h"

class View {
public:
    virtual void render() = 0;
    virtual uint8_t update(UiKeys* keys) = 0;
};

#endif
