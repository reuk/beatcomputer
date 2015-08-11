#pragma once

#include "window.h"

class CursorStorage {
public:
    CursorStorage(const Window & w);
    ~CursorStorage();

private:
    const Window & w;
    int y, x;
};

#define STORE_CURSOR CursorStorage sl_##__COUNTER__(*this);
