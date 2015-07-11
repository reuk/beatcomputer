#include "window.h"

using namespace std;

Window::Window(int height, int width, int starty, int startx)
    : window(newwin(height, width, starty, startx)) {
}

Window::Window(WINDOW * parent, int height, int width, int starty, int startx)
    : window(derwin(parent, height, width, starty, startx)) {
}

Window::Window(Window && rhs) noexcept = default;
Window & Window::operator=(Window && rhs) noexcept = default;

int Window::refresh() {
    return wrefresh(window.get());
}
int Window::erase() {
    return werase(window.get());
}
int Window::box(int v, int h) {
    return ::box(window.get(), v, h);
}
int Window::move(int v, int h) {
    return mvwin(window.get(), v, h);
}
int Window::print(int v, int h, const string & s) {
    return mvwaddstr(window.get(), v, h, s.c_str());
}

int Window::w_attron(int attr) {
    return wattron(window.get(), attr);
}
int Window::w_attroff(int attr) {
    return wattroff(window.get(), attr);
}

Window::operator WINDOW *() {
    return window.get();
}
