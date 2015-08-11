#include "cursor_storage.h"

#include <ncurses.h>

CursorStorage::CursorStorage(const Window & w)
        : w(w) {
    getsyx(y, x);
}

CursorStorage::~CursorStorage() {
    w.noutrefresh();
    setsyx(y, x);
}
