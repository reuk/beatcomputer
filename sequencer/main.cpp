#include "core.h"
#include "trim.h"
#include "instruction_list.h"
#include "window.h"
#include "logger.h"
#include "threaded_queue.h"

#include <ncurses.h>

#include <unistd.h>

#include <string>
#include <iomanip>
#include <sstream>
#include <iostream>
#include <fstream>
#include <bitset>
#include <chrono>
#include <thread>

#include "osc/OscReceivedElements.h"
#include "osc/OscPacketListener.h"
#include "ip/UdpSocket.h"

using namespace std;

string machine_word(uint32_t word) {
    stringstream ss;
    ss << setfill('0') << setw(8) << hex << word;
    return ss.str();
}

struct StatusDisplay {
    virtual void draw(const InstructionList &il, const Core &core,
                      const vector<Instruction> &memory) const = 0;
};

template <typename T>
class BoxedWindow : public Window, public StatusDisplay {
public:
    BoxedWindow(const string &title, WINDOW *parent, int height, int starty,
                int startx)
        : Window(parent, height, get_width(), starty, startx)
        , contents(*this, height - 2, 1, 1) {
        box(0, 0);

        w_attron(A_BOLD);
        print(0, (get_width() - title.size()) / 2, title);
        w_attroff(A_BOLD);
    }

    void draw(const InstructionList &il, const Core &core,
              const vector<Instruction> &memory) const override {
        contents.draw(il, core, memory);
    }

    static int get_width() {
        return 2 + T::get_width();
    }

private:
    T contents;
};

class MachineCodeWindow : public Window, public StatusDisplay {
public:
    MachineCodeWindow(WINDOW *parent, int height, int starty, int startx)
        : Window(parent, height, get_width(), starty, startx) {
    }

    void draw(const InstructionList &il, const Core &core,
              const vector<Instruction> &memory) const {
        erase();

        for (auto i = 0; i != memory.size(); ++i) {
            auto attr = i == core.ip;

            if (attr)
                w_attron(A_BOLD | COLOR_PAIR(1));
            print(i, 0, machine_word(memory[i].raw));
            if (attr)
                w_attroff(A_BOLD | COLOR_PAIR(1));
        }

        refresh();
    }

    static int get_width() {
        return 8;
    }
};

class MnemonicWindow : public Window, public StatusDisplay {
public:
    MnemonicWindow(WINDOW *parent, int height, int starty, int startx)
        : Window(parent, height, get_width(), starty, startx) {
    }

    void draw(const InstructionList &il, const Core &core,
              const vector<Instruction> &memory) const {
        erase();

        auto line = 0;
        for (const auto &i : memory) {
            auto attr = line == core.ip;

            if (attr)
                w_attron(A_BOLD | COLOR_PAIR(1));
            print(line, 0, il.disassemble(i));
            if (attr)
                w_attroff(A_BOLD | COLOR_PAIR(1));

            line += 1;
        }

        refresh();
    }

    static int get_width() {
        return 32;
    }
};

class TooltipWindow : public Window, public StatusDisplay {
public:
    TooltipWindow(WINDOW *parent, int height, int starty, int startx)
        : Window(parent, height, get_width(), starty, startx) {
    }

    void draw(const InstructionList &il, const Core &core,
              const vector<Instruction> &memory) const {
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

        refresh();
    }

    static int get_width() {
        return 50;
    }
};

class CoreCoreWindow : public Window, public StatusDisplay {
public:
    CoreCoreWindow(WINDOW *parent, int height, int starty, int startx)
        : Window(parent, height, get_width(), starty, startx) {
    }

    void draw(const InstructionList &il, const Core &core,
              const vector<Instruction> &memory) const {
        erase();

        auto register_string = [](auto i) {
            stringstream ss;
            ss << "R" << i;
            return ss.str();
        };

        auto line = 0;
        auto print_register = [this, &line](auto reg_id, auto value) {
            stringstream ss;
            ss << setw(3) << reg_id << ": " << machine_word(value);
            print(line, 0, ss.str());

            line += 1;
        };

        for (auto i = 0; i != 30; ++i) {
            print_register(register_string(i), core.reg[i]);
        }

        print_register("SP", core.sp);
        print_register("IP", core.ip);

        refresh();
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
        , window_machine_code("ram", *this, HEIGHT, 0, 0)
        , window_mnemonic("code", *this, HEIGHT, 0,
                          decltype(window_machine_code)::get_width())
        , window_tooltip("tooltips", *this, HEIGHT, 0,
                         decltype(window_machine_code)::get_width() +
                             decltype(window_mnemonic)::get_width())
        , window_core("status", *this, HEIGHT, 0,
                      decltype(window_machine_code)::get_width() +
                          decltype(window_mnemonic)::get_width() +
                          decltype(window_tooltip)::get_width())
        , memory(MEMORY_LOCATIONS) {
    }

    void set_memory(const vector<Instruction> &m) {
        auto limit = min(memory.size(), m.size());
        copy(m.begin(), m.begin() + limit, memory.begin());
    }

    void draw() {
        for (auto i :
             vector<StatusDisplay *>{&window_machine_code, &window_mnemonic,
                                     &window_tooltip, &window_core}) {
            i->draw(il, core, memory);
        }
        refresh();
    }

    void execute() {
        il.execute(core, memory);
        core.ip = core.ip % MEMORY_LOCATIONS;
    }

private:
    const InstructionList &il;

    static const int MEMORY_LOCATIONS = 32;
    static const int HEIGHT = 2 + MEMORY_LOCATIONS;

    BoxedWindow<MachineCodeWindow> window_machine_code;
    BoxedWindow<MnemonicWindow> window_mnemonic;
    BoxedWindow<TooltipWindow> window_tooltip;
    BoxedWindow<CoreCoreWindow> window_core;

    Core core;
    vector<Instruction> memory;
};

class Input {
public:
    enum class Type {
        TICK,
        KEY,
    };

    Input(Type type = Type::TICK, int value = 0)
        : type(type)
        , value(value) {
    }

    Type get_type() const {
        return type;
    }
    int get_value() const {
        return value;
    }

private:
    Type type;
    int value;
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
            }
        } catch (const osc::Exception &e) {
            cout << "error parsing message: " << m.AddressPattern() << ": "
                 << e.what() << endl;
        }
    }

private:
    ThreadedQueue<Input> &tq;
};

class Editor;

struct EditorCommand {
    virtual void do_command(Editor &e) = 0;
    virtual void undo_command(Editor &e) = 0;
};

class Editor {
public:
    Editor(const InstructionList &instruction_list)
        : instruction_list(instruction_list)
        , memory(32) {
    }

    void load_from_file(const string &fname) {
        ifstream infile(fname);

        auto index = 0;
        for (string line; getline(infile, line);) {
            if (!trim(line).empty()) {
                memory[index++] = instruction_list.assemble(line);
            }
        }
    }

    void do_command(unique_ptr<EditorCommand> &&command) {
        command->do_command(*this);
        commands.push(move(command));
    }

    void undo_command() {
        if (!commands.empty()) {
            auto i = move(commands.front());
            commands.pop();
            i->undo_command(*this);
        }
    }

    void sync_from_memory() {
        transform(begin(memory), end(memory), begin(mnemonics),
                  [this](auto i) { return instruction_list.disassemble(i); });
    }

    void sync_from_mnemonics() {
        transform(begin(mnemonics), end(mnemonics), begin(memory),
                  [this](auto i) { return instruction_list.assemble(i); });
    }

private:
    const InstructionList &instruction_list;
    queue<unique_ptr<EditorCommand>> commands;

public:
    vector<Instruction> memory;
    vector<string> mnemonics;
};

//  Data required for do_command should be invariant/const
//  somehow it should be fed to the constructor

class InsertCommand : public EditorCommand {
public:
    InsertCommand(int character)
        : character(character) {
    }

    void do_command(Editor &e) override {
        //  go to (y, x) and insert the character
    }

    void undo_command(Editor &e) override {
        //  go to (y, x) and remove the character
    }

private:
    const int character;
};

class BackspaceCommand : public EditorCommand {
public:
    void do_command(Editor &e) override {
        //  go to (y, x) and insert the character
    }

    void undo_command(Editor &e) override {
        //  go to (y, x) and remove the character
    }

private:
};

//  input queue
//  handles keyboard and osc input
//  osc receipt is done on a different thread, which pushes messages to the
//  queue on the main thread
//  keyboard input is done on main thread, messages pushed to queue
//  then, somewhere else, we can pull messages off the queue one at a time

int main(int argc, char **argv) {
    auto clean_up = [] {
        echo();
        keypad(stdscr, 0);
        nocbreak();
        endwin();
    };

    Logger::restart();

    InstructionList instruction_list;
    Editor editor(instruction_list);

    try {
        initscr();
        start_color();
        cbreak();
        keypad(stdscr, 1);
        noecho();

        init_pair(1, COLOR_BLUE, COLOR_BLACK);

        if (argc != 2) {
            throw runtime_error("expected one argument");
        }

        editor.load_from_file(argv[1]);
    } catch (const runtime_error &re) {
        clean_up();
        cout << "Exception: " << re.what() << endl;
        return EXIT_FAILURE;
    }

    CoreWindow cw(stdscr, instruction_list, 0, 0);

    mutex global_mutex;

    ThreadedQueue<Input> inputs;
    OscReceiver receiver(inputs);
    UdpListeningReceiveSocket s(
        IpEndpointName(IpEndpointName::ANY_ADDRESS, 7000), &receiver);

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

            //  no need to lock here because all other threads have been killed
            echo();
            keypad(stdscr, 0);
            nocbreak();
            endwin();
        };

    auto threaded_draw_refresh = [&global_mutex, &cw] {
        lock_guard<mutex> lock(global_mutex);
        cw.draw();
        cw.refresh();
    };

    try {
        cw.set_memory(editor.memory);
        threaded_draw_refresh();

        auto quit = false;
        while (!quit) {
            Input popped;
            inputs.pop(popped);

            switch (popped.get_type()) {
                case Input::Type::TICK:
                    cw.execute();
                    break;

                case Input::Type::KEY:
                    auto key = popped.get_value();

                    if (key < 127) {
                        // int y, x;
                        // getyx(stdscr, y, x);
                        editor.do_command(make_unique<InsertCommand>(key));
                    }

                    if (key == KEY_BACKSPACE) {
                        editor.do_command(make_unique<BackspaceCommand>());
                    }

                    /*
                    if (key == UNDO) {
                        editor.undo_command();
                    }
                    */

                    break;
            }

            //  for the time being we assume that any input event will
            //  require a redraw
            threaded_draw_refresh();
        }

        clean_up_threads();
        return EXIT_SUCCESS;
    } catch (const runtime_error &re) {
        clean_up_threads();
        cout << "Exception: " << re.what() << endl;
        return EXIT_FAILURE;
    }
}
