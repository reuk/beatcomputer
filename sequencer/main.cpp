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

struct StatusDisplay {
    StatusDisplay(const InstructionList &il, const Core &core,
                  const vector<Instruction> &memory)
        : il(il)
        , core(core)
        , memory(memory) {
    }
    const InstructionList &il;
    const Core &core;
    const vector<Instruction> &memory;
};

class ContentWindow : public Window, public StatusDisplay, public TextEditorListener {
public:
    ContentWindow(WINDOW *parent, int height, int width, int starty, int startx,
                  const InstructionList &il, const Core &core,
                  const vector<Instruction> &memory)
        : Window(parent, height, width, starty, startx)
        , StatusDisplay(il, core, memory) {
    }

    void cursor_moved(int y, int x) override {
        Logger::log("cursor moved to: ", y, ", ", x);
        w_move(y, x);
        refresh();
    }

    void character_added(char character) override {
        w_addch(character);
        refresh();
    }
};

class MachineCodeWindow : public ContentWindow {
public:
    MachineCodeWindow(WINDOW *parent, int height, int starty, int startx,
                  const InstructionList &il, const Core &core,
                  const vector<Instruction> &memory)
        : ContentWindow(parent, height, get_width(), starty, startx, il, core, memory) {
    }

    static int get_width() {
        return 8;
    }
};

class MnemonicWindow : public ContentWindow {
public:
    MnemonicWindow(WINDOW *parent, int height, int starty, int startx,
                  const InstructionList &il, const Core &core,
                  const vector<Instruction> &memory)
        : ContentWindow(parent, height, get_width(), starty, startx, il, core, memory) {
    }

    static int get_width() {
        return 32;
    }
};

class TooltipWindow : public Window, public StatusDisplay {
public:
    TooltipWindow(WINDOW *parent, int height, int starty, int startx,
                  const InstructionList &il, const Core &core,
                  const vector<Instruction> &memory)
        : Window(parent, height, get_width(), starty, startx)
        , StatusDisplay(il, core, memory) {
    }

    void draw() const {
        int y, x;
        getsyx(y, x);

        erase();

        auto line = 0;
        for (const auto &i : memory) {
            auto attr = line == core.ip;

            if (attr)
                w_attron(A_BOLD | COLOR_PAIR(1));
            print(line, 0, il.tooltip(i));
            if (attr)
                w_attroff(A_BOLD | COLOR_PAIR(1));

            line += 1;
        }

        noutrefresh();

        setsyx(y, x);
    }

    static int get_width() {
        return 50;
    }
};

class CoreCoreWindow : public Window, public StatusDisplay {
public:
    CoreCoreWindow(WINDOW *parent, int height, int starty, int startx,
                   const InstructionList &il, const Core &core,
                   const vector<Instruction> &memory)
        : Window(parent, height, get_width(), starty, startx)
        , StatusDisplay(il, core, memory) {
    }

    void draw() const {
        int y, x;
        getsyx(y, x);

        erase();

        auto line = 0;
        auto print_register = [this, &line](auto reg_id, auto value) {
            print(line, 0,
                  build_string(setw(3), reg_id, ": ", machine_word(value)));

            line += 1;
        };

        for (auto i = 0; i != 30; ++i) {
            print_register(build_string("R", i), core.reg[i]);
        }

        print_register("SP", core.sp);
        print_register("IP", core.ip);

        noutrefresh();

        setsyx(y, x);
    }

    static int get_width() {
        return 5 + 8;
    }
};

class CoreWindow : public Window {
public:
    CoreWindow(WINDOW *parent, const InstructionList &il, int starty,
               int startx)
        : Window(parent, HEIGHT, decltype(window_machine_code)::get_width() +
                                     decltype(window_mnemonic)::get_width() +
                                     decltype(window_tooltip)::get_width() +
                                     decltype(window_core)::get_width(),
                 starty, startx)
        , il(il)
        , memory(MEMORY_LOCATIONS)
        , editor(il)
        , window_machine_code("ram", *this, HEIGHT, 0, 0, il, core, memory)
        , window_mnemonic("code", *this, HEIGHT, 0,
                          decltype(window_machine_code)::get_width(), il, core, memory)
        , window_tooltip("tooltips", *this, HEIGHT, 0,
                         decltype(window_machine_code)::get_width() +
                             decltype(window_mnemonic)::get_width(),
                         il, core, memory)
        , window_core("status", *this, HEIGHT, 0,
                      decltype(window_machine_code)::get_width() +
                          decltype(window_mnemonic)::get_width() +
                          decltype(window_tooltip)::get_width(),
                      il, core, memory) {
        editor.memory.add_listener(&window_machine_code.get_contents());
        editor.mnemonics.add_listener(&window_mnemonic.get_contents());
    }

    void set_memory(const vector<Instruction> &m) {
        auto limit = min(memory.size(), m.size());
        copy(m.begin(), m.begin() + limit, memory.begin());
    }

    void execute() {
        il.execute(core, memory);
        core.ip = core.ip % MEMORY_LOCATIONS;

        update_displays();
    }

    void update_displays() {
        window_tooltip.get_contents().draw();
        window_core.get_contents().draw();

        doupdate();
    }

    void load_from_file(const string & fname) {
        editor.load_from_file(fname);
        set_memory(editor.get_memory());

        window_mnemonic.get_contents().cursor_moved(0, 0);

        update_displays();
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

class OscReceiver : public osc::OscPacketListener {
public:
    OscReceiver(ThreadedQueue<Input> &tq)
        : tq(tq) {
    }

protected:
    void ProcessMessage(const osc::ReceivedMessage &m,
                        const IpEndpointName &ip) override {
        try {
            if (m.AddressPattern() == string("/time_server")) {
                osc::ReceivedMessageArgumentStream args = m.ArgumentStream();
                const char *str;
                args >> str >> osc::EndMessage;

                if (str == string("tick")) {
                    tq.push(Input(Input::Type::TICK));
                }
            } else if (m.AddressPattern() == string("/key_server")) {
                osc::ReceivedMessageArgumentStream args = m.ArgumentStream();
                osc::int32 key;
                args >> key >> osc::EndMessage;

                tq.push(Input(Input::Type::KEY, key));
            }

        } catch (const osc::Exception &e) {
            cout << "error parsing message: " << m.AddressPattern() << ": "
                 << e.what() << endl;
        }
    }

private:
    ThreadedQueue<Input> &tq;
};

int main(int argc, char **argv) {
    Logger::restart();
    gflags::ParseCommandLineFlags(&argc, &argv, true);

    if (argc != 2) {
        cout << "Expected an input file" << endl;
        return EXIT_FAILURE;
    }

    InstructionList instruction_list(
        InstructionManager(FLAGS_o_port, FLAGS_osc_prefix, FLAGS_osc_address));

    initscr();
    start_color();
    cbreak();
    keypad(stdscr, 1);
    noecho();
    init_pair(1, COLOR_BLUE, COLOR_BLACK);

    CoreWindow cw(stdscr, instruction_list, 0, 0);
    cw.load_from_file(argv[1]);

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
        auto quit = false;
        while (!quit) {
            Input popped;
            inputs.pop(popped);

            switch (popped.get_type()) {
                case Input::Type::TICK:
                    {
                        lock_guard<mutex> lock(global_mutex);
                        cw.execute();
                    }
                    break;

                case Input::Type::KEY:
                    auto key = popped.get_value();

                    unique_ptr<EditorCommand> command;

                    if (key == KEY_RIGHT) {
                        command = make_unique<MoveCommand>(Direction::RIGHT);
                    } else if (key == KEY_LEFT) {
                        command = make_unique<MoveCommand>(Direction::LEFT);
                    } else if (key == KEY_DOWN) {
                        command = make_unique<MoveCommand>(Direction::DOWN);
                    } else if (key == KEY_UP) {
                        command = make_unique<MoveCommand>(Direction::UP);
                    } else if (key < 127) {
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
