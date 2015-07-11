#include <ncurses.h>

class WIN_BORDER {
public:
    WIN_BORDER()
        : ls('|')
        , rs('|')
        , ts('-')
        , bs('-')
        , tl('+')
        , tr('+')
        , bl('+')
        , br('+')
    {

    }

    chtype ls, rs, ts, bs, tl, tr, bl, br;
};

class WIN {
public:
    WIN()
        : height(3)
        , width(10)
        , starty((LINES - height) / 2)
        , startx((COLS - width) / 2)
    {

    }

    void print_params() const {
        mvprintw(25, 0, "%d %d %d %d", startx, starty, width, height);
        refresh();
    }

    void create_box(bool flag) {
        auto h = height;
        auto w = width;
        auto x = startx;
        auto y = starty;

        if (flag) {
            mvaddch(y, x, border.tl);
            mvaddch(y, x + w, border.tr);
            mvaddch(y + h, x, border.bl);
            mvaddch(y + h, x + w, border.br);
            mvhline(y, x + 1, border.ts, w - 1);
            mvhline(y + h, x + 1, border.bs, w - 1);
            mvvline(y + 1, x, border.ls, h - 1);
            mvvline(y + 1, x + w, border.rs, h - 1);
        } else {
            for (auto j = y; j <= y + h; ++j) {
                for (auto i = x; i <= x + w; ++i) {
                    mvaddch(j, i, ' ');
                }
            }
        }
    }

    int height, width;
    int starty, startx;
    WIN_BORDER border;
};

int main(int argc, char ** argv) {
    initscr();
    start_color();
    cbreak();
    keypad(stdscr, true);
    noecho();
    init_pair(1, COLOR_CYAN, COLOR_BLACK);

    attron(COLOR_PAIR(1));
    printw("Press F1 to exit");
    refresh();
    attroff(COLOR_PAIR(1));

    WIN win;
    win.create_box(true);

    int ch;
    while ((ch = getch()) != KEY_F(1)) {
        switch (ch) {
            case KEY_LEFT:
                win.create_box(false);
                --win.startx;
                win.create_box(true);
                break;
            case KEY_RIGHT:
                win.create_box(false);
                ++win.startx;
                win.create_box(true);
                break;
            case KEY_UP:
                win.create_box(false);
                --win.starty;
                win.create_box(true);
                break;
            case KEY_DOWN:
                win.create_box(false);
                ++win.starty;
                win.create_box(true);
                break;
        }
    }
    endwin();
    return 0;
}
