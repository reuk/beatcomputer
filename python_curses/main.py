import curses

class Window(object):
    def __init__(self, parent, nlines, ncols, begin_y = 0, begin_x = 0):
        self.window = parent.derwin(nlines, ncols, begin_y, begin_x)

    def derwin(self, nlines, ncols, begin_y = 0, begin_x = 0):
        return self.window.derwin(nlines, ncols, begin_y, begin_x)

    def addstr(self, y, x, s, attr):
        return self.window.addstr(y, x, s, attr)

    def erase(self):
        return self.window.erase()

    def box(self, y = 0, x = 0):
        return self.window.box(y, x)

    def refresh(self):
        return self.window.refresh()

    def getmaxyx(self):
        return self.window.getmaxyx()

    def draw(self):
        pass

class BoxedWindow(Window):
    def __init__(self, parent, inner_class, nlines, ncols, begin_y = 0, begin_x = 0):
        Window.__init__(self, parent, nlines, ncols, begin_y, begin_x)
        self.drawable_window = inner_class(self, nlines - 2, ncols - 2, 1, 1)
        self.box(0, 0)

    def draw(self):
        self.drawable_window.draw()

class SpecialWindow(Window):
    def __init__(self, parent, nlines, ncols, begin_y = 0, begin_x = 0):
        Window.__init__(self, parent, nlines, ncols, begin_y, begin_x)
        self.i = 0

    def draw(self):
        self.erase()
        s = "hello everybody: " + str(self.i)
        self.i += 1
        y, x = self.getmaxyx()
        self.window.addstr(y / 2, (x - len(s)) / 2, s, curses.color_pair(1))
        self.refresh()

def main(stdscr):
    curses.init_pair(1, curses.COLOR_BLUE, curses.COLOR_BLACK)

    boxed_window = BoxedWindow(stdscr, SpecialWindow, 20, 40, 0, 0)

    while True:
        boxed_window.draw()
        c = stdscr.getch()
        if c == ord('q'):
            break

if __name__ == "__main__":
    try:
        curses.wrapper(main)
    except Exception as e:
        print e
