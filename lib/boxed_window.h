#pragma once

#include "window.h"

template <typename T>
class BoxedWindow : public Window {
public:
    using Contents = T;

    template <typename... Ts>
    BoxedWindow(const std::string &title,
                WINDOW *parent,
                int height,
                int starty,
                int startx,
                const Ts &... ts)
            : Window(parent, height, get_width(), starty, startx)
            , contents(*this, height - 2, 1, 1, ts...) {
        box(0, 0);

        w_attron(A_BOLD);
        print(0, (get_width() - title.size()) / 2, title);
        w_attroff(A_BOLD);

        refresh();
    }

    static int get_width() {
        return 2 + T::get_width();
    }

    T &get_contents() {
        return contents;
    }

private:
    T contents;
};
