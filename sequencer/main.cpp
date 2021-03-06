#include "core.h"
#include "trim.h"
#include "instruction_list.h"
#include "window.h"
#include "boxed_window.h"
#include "logger.h"
#include "threaded_queue.h"
#include "input.h"
#include "editor.h"
#include "editor_command.h"
#include "osc_receiver.h"
#include "cursor_storage.h"

#include <ncurses.h>

#include <gflags/gflags.h>

#include "osc/OscReceivedElements.h"
#include "osc/OscPacketListener.h"
#include "ip/UdpSocket.h"

#include <string>
#include <iomanip>
#include <sstream>
#include <iostream>
#include <fstream>
#include <bitset>
#include <chrono>
#include <thread>
#include <regex>
#include <atomic>
#include <condition_variable>

#include <unistd.h>

using namespace std;

bool validate_port(const char *flagname, int32_t value) {
    return 0 < value && value < 32768;
}

bool validate_prefix(const char *flagname, const string &prefix) {
    regex reg("/[a-zA-Z0-9]+");
    smatch m;
    return regex_match(prefix, m, reg);
}

bool validate_address(const char *flagname, const string &address) {
    regex reg(
        "\\b(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:25[0-5]|2[0-4]"
        "[0-9]|[01]?[0-9][0-9]?)\\b");
    smatch m;
    return regex_match(address, m, reg);
}

DEFINE_int32(i_port, 7000, "Port on which to receive OSC");
DEFINE_int32(o_port, 7001, "Port on which to send OSC");
DEFINE_string(osc_prefix, "/beatcomputer", "Outbound OSC message prefix");
DEFINE_string(osc_address, "127.0.0.1", "Outbound OSC message address");

static const bool i_port_dummy =
    gflags::RegisterFlagValidator(&FLAGS_i_port, &validate_port);
static const bool o_port_dummy =
    gflags::RegisterFlagValidator(&FLAGS_o_port, &validate_port);
static const bool prefix_dummy =
    gflags::RegisterFlagValidator(&FLAGS_osc_prefix, &validate_prefix);
static const bool address_dummy =
    gflags::RegisterFlagValidator(&FLAGS_osc_address, &validate_address);

class TickListener {
public:
    TickListener()
            : prev(0) {
    }
    void tick(int line) {
        auto p = prev;
        prev = line;
        on_tick(p, line);
    }
    virtual void on_tick(int prev, int line) = 0;
    int get_tick_line() const {
        return prev;
    }

private:
    int prev;
};

class ContentWindow : public Window,
                      public TextEditorListener,
                      public TickListener {
public:
    ContentWindow(WINDOW *parent, int height, int width, int starty, int startx)
            : Window(parent, height, width, starty, startx) {
    }

    void cursor_moved(const Vec2 &pos) override {
        w_move(pos.y, pos.x);
        refresh();
    }

    void line_modified(int line, const string &contents) override {
        {
            STORE_CURSOR;

            w_move(line, 0);
            w_clrtoeol();
            print(line, 0, contents);

            if (line == get_tick_line())
                w_mvchgat(line, 0, -1, WA_BOLD, 1);

            touch();
        }
        doupdate();
    }

    void on_tick(int prev, int line) override {
        {
            STORE_CURSOR;

            w_mvchgat(prev, 0, -1, WA_NORMAL, 0);
            w_mvchgat(line, 0, -1, WA_BOLD, 1);

            touch();
        }
        doupdate();
    }
};

class MachineCodeWindow : public ContentWindow {
public:
    MachineCodeWindow(WINDOW *parent, int height, int starty, int startx)
            : ContentWindow(parent, height, get_width(), starty, startx) {
    }

    static int get_width() {
        return 8;
    }
};

class MnemonicWindow : public ContentWindow {
public:
    MnemonicWindow(WINDOW *parent, int height, int starty, int startx)
            : ContentWindow(parent, height, get_width(), starty, startx) {
    }

    static int get_width() {
        return 32;
    }
};

class TooltipWindow : public Window,
                      public TickListener,
                      public CompileOutputListener {
public:
    TooltipWindow(WINDOW *parent,
                  int height,
                  int starty,
                  int startx,
                  const InstructionList &il,
                  const vector<Instruction> &memory)
            : Window(parent, height, get_width(), starty, startx)
            , line_status(memory.size(), CompileOutputListener::Type::GOOD) {
    }

    void color_line(int line, CompileOutputListener::Type type) {
        line_status[line] = type;

        switch (type) {
            case CompileOutputListener::Type::GOOD:
                if (line == get_tick_line())
                    w_mvchgat(line, 0, -1, WA_BOLD, 1);
                else
                    w_mvchgat(line, 0, -1, WA_NORMAL, 0);
                break;
            case CompileOutputListener::Type::WARNING:
                if (line == get_tick_line())
                    w_mvchgat(line, 0, -1, WA_BOLD, 5);
                else
                    w_mvchgat(line, 0, -1, WA_BOLD, 3);
                break;
            case CompileOutputListener::Type::ERROR:
                if (line == get_tick_line())
                    w_mvchgat(line, 0, -1, WA_BOLD, 4);
                else
                    w_mvchgat(line, 0, -1, WA_BOLD, 2);
                break;
        }
    }

    void on_tick(int prev, int line) override {
        {
            STORE_CURSOR;

            color_line(prev, line_status[prev]);
            color_line(line, line_status[line]);

            touch();
        }
        doupdate();
    }

    void line_compiled(int line,
                       CompileOutputListener::Type type,
                       const string &contents) override {
        {
            STORE_CURSOR;

            w_move(line, 0);
            w_clrtoeol();
            print(line, 0, contents);

            color_line(line, type);

            touch();
        }
        doupdate();
    }

    static int get_width() {
        return 50;
    }

private:
    vector<CompileOutputListener::Type> line_status;
};

class CoreCoreWindow : public Window, public TickListener {
public:
    CoreCoreWindow(
        WINDOW *parent, int height, int starty, int startx, const Core &core)
            : Window(parent, height, get_width(), starty, startx)
            , core(core) {
    }

    void on_tick(int, int) override {
        STORE_CURSOR;

        erase();

        auto line = 0;
        auto print_register = [this, &line](auto reg_id, auto value) {
            print(line,
                  0,
                  build_string(setw(3), reg_id, ": ", machine_word(value)));

            line += 1;
        };

        for (auto i = 0; i != 30; ++i) {
            print_register(build_string("R", i), core.reg[i]);
        }

        print_register("SP", core.sp);
        print_register("IP", core.ip);
    }

    static int get_width() {
        return 5 + 8;
    }

private:
    const Core &core;
};

class CoreWindow : public Window, public ListenerList<TickListener> {
public:
    CoreWindow(WINDOW *parent,
               const InstructionList &il,
               int starty,
               int startx)
            : Window(parent,
                     HEIGHT,
                     decltype(window_machine_code)::get_width() +
                         decltype(window_mnemonic)::get_width() +
                         decltype(window_tooltip)::get_width() +
                         decltype(window_core)::get_width(),
                     starty,
                     startx)
            , il(il)
            , memory(MEMORY_LOCATIONS)
            , editor(il,
                     memory,
                     decltype(window_machine_code)::Contents::get_width(),
                     decltype(window_mnemonic)::Contents::get_width())
            , window_machine_code("ram", *this, HEIGHT, 0, 0)
            , window_mnemonic("code",
                              *this,
                              HEIGHT,
                              0,
                              decltype(window_machine_code)::get_width())
            , window_tooltip("tooltips",
                             *this,
                             HEIGHT,
                             0,
                             decltype(window_machine_code)::get_width() +
                                 decltype(window_mnemonic)::get_width(),
                             il,
                             memory)
            , window_core("status",
                          *this,
                          HEIGHT,
                          0,
                          decltype(window_machine_code)::get_width() +
                              decltype(window_mnemonic)::get_width() +
                              decltype(window_tooltip)::get_width(),
                          core) {
        editor.memory.add_listener_text_editor(
            &window_machine_code.get_contents());
        editor.mnemonics.add_listener_text_editor(
            &window_mnemonic.get_contents());

        editor.add_listener(&window_tooltip.get_contents());

        add_listener(&window_machine_code.get_contents());
        add_listener(&window_mnemonic.get_contents());
        add_listener(&window_tooltip.get_contents());
        add_listener(&window_core.get_contents());
    }

    void set_memory(const vector<Instruction> &m) {
        auto limit = min(memory.size(), m.size());
        copy(m.begin(), m.begin() + limit, memory.begin());
    }

    void execute() {
        il.execute(core, memory);

        core.ip = core.ip % MEMORY_LOCATIONS;

        call(&TickListener::tick, core.ip);
    }

    void load_from_file(const string &fname) {
        editor.load_from_file(fname);
        call(&TickListener::tick, 0);
    }

    void blank() {
        editor.blank();
        call(&TickListener::tick, 0);
    }

private:
    const InstructionList &il;
    Core core;
    vector<Instruction> memory;

    static const int MEMORY_LOCATIONS = 32;
    static const int HEIGHT = 2 + MEMORY_LOCATIONS;

public:
    Editor editor;

private:
    BoxedWindow<MachineCodeWindow> window_machine_code;
    BoxedWindow<MnemonicWindow> window_mnemonic;
    BoxedWindow<TooltipWindow> window_tooltip;
    BoxedWindow<CoreCoreWindow> window_core;
};

int main(int argc, char **argv) {
    Logger::restart();
    gflags::ParseCommandLineFlags(&argc, &argv, true);

    InstructionList instruction_list(
        InstructionManager(FLAGS_o_port, FLAGS_osc_prefix, FLAGS_osc_address));

    initscr();
    start_color();
    cbreak();
    keypad(stdscr, 1);
    noecho();

    init_pair(1, COLOR_BLACK, COLOR_WHITE);

    init_pair(2, COLOR_RED, COLOR_BLACK);
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);

    init_pair(4, COLOR_RED, COLOR_WHITE);
    init_pair(5, COLOR_YELLOW, COLOR_WHITE);

    CoreWindow cw(stdscr, instruction_list, 0, 0);
    cw.blank();

    if (argc == 2) {
        cw.load_from_file(argv[1]);
    }

    doupdate();

    mutex global_mutex;

    ThreadedQueue<Input> inputs;
    OscReceiver receiver(inputs);
    UdpListeningReceiveSocket s(
        IpEndpointName(IpEndpointName::ANY_ADDRESS, FLAGS_i_port), &receiver);

    thread osc_thread([&s] { s.Run(); });

    atomic_bool run_keyboard_thread(true);
    thread keyboard_thread([&run_keyboard_thread, &global_mutex, &inputs] {
        //  set up file descriptor set with just stdin entry
        fd_set fd;
        FD_ZERO(&fd);
        FD_SET(fileno(stdin), &fd);

        //  set up timeout struct
        struct timeval tv;
        tv.tv_sec = 1;
        tv.tv_usec = 0;

        //  we keep running select with a short timeout
        //  so essentially we check whether to keep polling input every
        //  second, or after every stdin event
        while (run_keyboard_thread) {
            auto sav = fd;
            if (select(fileno(stdin) + 1, &sav, nullptr, nullptr, &tv) > 0) {
                //  if there is some input to fetch, we lock the 'curses' mutex
                //  and then use getch to find out what the key is
                lock_guard<mutex> lock(global_mutex);
                inputs.push(Input(Input::Type::KEY, getch()));
            }
        }
    });

    auto clean_up_threads =
        [&s, &osc_thread, &run_keyboard_thread, &keyboard_thread] {
            s.AsynchronousBreak();
            osc_thread.join();

            run_keyboard_thread = false;
            keyboard_thread.join();

            echo();
            keypad(stdscr, 0);
            nocbreak();
            endwin();
        };

    try {
        for (;;) {
            Input popped;
            inputs.pop(popped);

            switch (popped.get_type()) {
                case Input::Type::TICK: {
                    lock_guard<mutex> lock(global_mutex);
                    cw.execute();
                } break;

                case Input::Type::KEY:
                    auto key = popped.get_value();
                    Logger::log("key pressed: ", key);

                    unique_ptr<EditorCommand> command =
                        make_unique<EditorCommand>();

                    if (key == KEY_RIGHT) {
                        command = make_unique<MoveCommand>(Direction::RIGHT);
                    } else if (key == KEY_LEFT) {
                        command = make_unique<MoveCommand>(Direction::LEFT);
                    } else if (key == KEY_DOWN) {
                        command = make_unique<MoveCommand>(Direction::DOWN);
                    } else if (key == KEY_UP) {
                        command = make_unique<MoveCommand>(Direction::UP);
                    } else if (key == '\t') {
                        command = make_unique<SelectCommand>();
                    } else if (key == KEY_BACKSPACE || key == KEY_DC ||
                               key == 127) {
                        command = make_unique<BackspaceCommand>();
                    } else if (isalnum(key) || key == '\n' || key == ' ') {
                        command = make_unique<InsertCommand>(key);
                    }

                    {
                        lock_guard<mutex> lock(global_mutex);
                        cw.editor.do_command(move(command));
                    }

                    break;
            }
        }

        clean_up_threads();
        return EXIT_SUCCESS;
    } catch (const runtime_error &re) {
        clean_up_threads();
        cout << "Exception: " << re.what() << endl;
        return EXIT_FAILURE;
    }
}
