#pragma once

#include <ncurses.h>

#include <memory>
#include <string>

class Window {
private:
    struct Destructor { void operator()(WINDOW *win); };
    std::unique_ptr<WINDOW, Destructor> window;

public:
    Window(int height, int width, int starty, int startx);
    Window(WINDOW *parent, int height, int width, int starty, int startx);

    Window(Window &&rhs) noexcept;
    Window &operator=(Window &&rhs) noexcept;
    Window(const Window &rhs) = delete;
    Window &operator=(const Window &rhs) = delete;

    virtual int refresh() const;
    virtual int erase() const;
    virtual int box(int v, int h) const;
    virtual int move(int v, int h) const;
    virtual int print(int v, int h, const std::string &s) const;
    virtual int resize(int v, int h) const;

    virtual int w_attron(int attr) const;
    virtual int w_attroff(int attr) const;

    virtual void draw() const = 0;

    operator WINDOW *();
};
