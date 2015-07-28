#include "core.h"
#include "trim.h"
#include "instruction_list.h"
#include "window.h"
#include "logger.h"
#include "threaded_queue.h"

#include <ncurses.h>

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
        refresh();
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
        : Window(parent, HEIGHT, BoxedWindow<MachineCodeWindow>::get_width() +
                                     BoxedWindow<MnemonicWindow>::get_width() +
                                     BoxedWindow<TooltipWindow>::get_width() +
                                     BoxedWindow<CoreCoreWindow>::get_width(),
                 starty, startx)
        , il(il)
        , window_machine_code("ram", *this, HEIGHT, 0, 0)
        , window_mnemonic("code", *this, HEIGHT, 0,
                          BoxedWindow<MachineCodeWindow>::get_width())
        , window_tooltip("tooltips", *this, HEIGHT, 0,
                         BoxedWindow<MachineCodeWindow>::get_width() +
                             BoxedWindow<MnemonicWindow>::get_width())
        , window_core("status", *this, HEIGHT, 0,
                      BoxedWindow<MachineCodeWindow>::get_width() +
                          BoxedWindow<MnemonicWindow>::get_width() +
                          BoxedWindow<TooltipWindow>::get_width())
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
    Input(int i)
        : i(i) {
    }

private:
    int i;
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
                    tq.push(0);
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
    vector<Instruction> memory(32);

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

        ifstream infile(argv[1]);

        auto index = 0;

        for (string line; getline(infile, line);) {
            if (!trim(line).empty()) {
                auto instr = instruction_list.assemble(line);
                memory[index++] = instr;
            }
        }
    } catch (const runtime_error &re) {
        clean_up();
        cout << "Exception: " << re.what() << endl;
        return EXIT_FAILURE;
    }

    ThreadedQueue<Input> inputs;
    OscReceiver receiver(inputs);
    UdpListeningReceiveSocket s(
        IpEndpointName(IpEndpointName::ANY_ADDRESS, 7000), &receiver);
    thread osc_thread([&s] { s.Run(); });

    auto clean_up_thread = [&s, &osc_thread] {
        s.AsynchronousBreak();
        osc_thread.join();
        echo();
        keypad(stdscr, 0);
        nocbreak();
        endwin();
    };

    try {
        CoreWindow cw(stdscr, instruction_list, 0, 0);
        cw.set_memory(memory);
        cw.draw();
        cw.refresh();

        bool read = true;
        int in;
        while (read && (in = getch())) {
            switch (in) {
                case 'q':
                    read = false;
                    break;
                default:
                    cw.execute();
                    cw.draw();
                    cw.refresh();
                    break;
            }
        }

        clean_up_thread();
        return EXIT_SUCCESS;
    } catch (const runtime_error &re) {
        clean_up_thread();
        cout << "Exception: " << re.what() << endl;
        return EXIT_FAILURE;
    }
}
