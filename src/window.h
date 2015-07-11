#pragma once

#include <ncurses.h>

#include <memory>
#include <string>

class Window {
private:
    struct Destructor {
        inline void operator()(WINDOW * win) {
            delwin(win);
        }
    };

public:
    Window(int height, int width, int starty, int startx);
    Window(WINDOW * parent, int height, int width, int starty, int startx);

    Window(Window && rhs) noexcept;
    Window & operator=(Window && rhs) noexcept;
    Window(const Window & rhs) = delete;
    Window & operator=(const Window & rhs) = delete;

    virtual int refresh();
    virtual int erase();
    virtual int box(int v, int h);
    virtual int move(int v, int h);
    virtual int print(int v, int h, const std::string & s);

    virtual int w_attron(int attr);
    virtual int w_attroff(int attr);

    operator WINDOW *();

private:
    std::unique_ptr<WINDOW, Destructor> window;
};
