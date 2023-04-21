#ifndef _VIEW_H_
#define _VIEW_H_

#include "def.h"

class View {
public:
    virtual void render() = 0;
    virtual void handle(UiKeys& keys) = 0;
};

#endif
