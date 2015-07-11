#include <ncurses.h>

int main(int argc, char ** argv) {
    initscr();
    cbreak();
    keypad(stdscr, true);

    printw("Press F1 to exit");
    refresh();

    auto height = 3;
    auto width = 10;

    auto starty = (LINES - height) / 2;
    auto startx = (COLS - width) / 2;

    auto my_win = newwin(height, width, starty, startx);
    box(my_win, 0, 0);
    wrefresh(my_win);

    int ch;
    while ((ch = getch()) != KEY_F(1)) {
        switch (ch) {
            case KEY_LEFT:
                mvwin(my_win, starty, --startx);
                break;
            case KEY_RIGHT:
                mvwin(my_win, starty, ++startx);
                break;
            case KEY_UP:
                mvwin(my_win, --starty, startx);
                break;
            case KEY_DOWN:
                mvwin(my_win, ++starty, startx);
                break;
        }

        wnoutrefresh(my_win);
        wnoutrefresh(stdscr);
        doupdate();
    }

    delwin(my_win);

    endwin();
    return 0;
}
