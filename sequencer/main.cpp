#include "core.h"
#include "trim.h"
#include "instruction_list.h"
#include "window.h"

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

class BoxedWindow : public Window {
public:
    BoxedWindow(WINDOW *parent, int height, int width, int starty, int startx)
        : Window(parent, height, width, starty, startx) {
    }

    void draw(const InstructionList &il, Core &core,
              vector<Instruction> &memory) const {
        erase();
        box(0, 0);

        draw_contents(il, core, memory);

        refresh();
    }

    virtual void draw_contents(const InstructionList &il, Core &core,
                               vector<Instruction> &memory) const = 0;
};

class MachineCodeWindow : public BoxedWindow {
public:
    MachineCodeWindow(WINDOW *parent, int height, int starty, int startx)
        : BoxedWindow(parent, height, get_width(), starty, startx) {
    }

    void draw_contents(const InstructionList &il, Core &core,
                       vector<Instruction> &memory) const override {
        auto line = 0;
        for (const auto &i : memory) {
            auto attr = line == core.ip;

            if (attr)
                w_attron(A_BOLD | COLOR_PAIR(1));
            print(1 + line, 1, machine_word(i.raw));
            if (attr)
                w_attroff(A_BOLD | COLOR_PAIR(1));

            line += 1;
        }
    }

    static int get_width() {
        return 10;
    }
};

class MnemonicWindow : public BoxedWindow {
public:
    MnemonicWindow(WINDOW *parent, int height, int starty, int startx)
        : BoxedWindow(parent, height, get_width(), starty, startx) {
    }

    void draw_contents(const InstructionList &il, Core &core,
                       vector<Instruction> &memory) const override {
        auto line = 0;
        for (const auto &i : memory) {
            auto attr = line == core.ip;

            if (attr)
                w_attron(A_BOLD | COLOR_PAIR(1));
            print(1 + line, 1, il.disassemble(i));
            if (attr)
                w_attroff(A_BOLD | COLOR_PAIR(1));

            line += 1;
        }
    }

    static int get_width() {
        return 32;
    }
};

class TooltipWindow : public BoxedWindow {
public:
    TooltipWindow(WINDOW *parent, int height, int starty, int startx)
        : BoxedWindow(parent, height, get_width(), starty, startx) {
    }

    void draw_contents(const InstructionList &il, Core &core,
                       vector<Instruction> &memory) const override {
        auto line = 0;
        for (const auto &i : memory) {
            auto attr = line == core.ip;

            if (attr)
                w_attron(A_BOLD | COLOR_PAIR(1));
            print(1 + line, 1, il.tooltip(i));
            if (attr)
                w_attroff(A_BOLD | COLOR_PAIR(1));

            line += 1;
        }
    }

    static int get_width() {
        return 32;
    }
};

class CoreCoreWindow : public BoxedWindow {
public:
    CoreCoreWindow(WINDOW *parent, int height, int starty, int startx)
        : BoxedWindow(parent, height, get_width(), starty, startx) {
    }

    void draw_contents(const InstructionList &il, Core &core,
                       vector<Instruction> &memory) const override {
        auto register_string = [](auto i) {
            stringstream ss;
            ss << "R" << i;
            return ss.str();
        };

        auto line = 0;
        auto print_register = [this, &line](auto reg_id, auto value) {
            stringstream ss;
            ss << setw(3) << reg_id << ": " << machine_word(value);
            print(1 + line, 1, ss.str());

            line += 1;
        };

        for (auto i = 0; i != 30; ++i) {
            print_register(register_string(i), core.reg[i]);
        }

        print_register("SP", core.sp);
        print_register("IP", core.ip);
    }

    static int get_width() {
        return 2 + 5 + 8;
    }
};

class CoreWindow : public Window {
public:
    CoreWindow(WINDOW *parent, const InstructionList &il, int starty,
               int startx)
        : Window(parent, HEIGHT,
                 MachineCodeWindow::get_width() + MnemonicWindow::get_width() +
                     TooltipWindow::get_width() + CoreCoreWindow::get_width(),
                 starty, startx)
        , il(il)
        , window_machine_code(*this, 0, 0, 0)
        , window_mnemonic(*this, 0, 0, MachineCodeWindow::get_width())
        , window_tooltip(*this, 0, 0, MachineCodeWindow::get_width() +
                                          MnemonicWindow::get_width())
        , window_core(*this, 0, 0, MachineCodeWindow::get_width() +
                                       MnemonicWindow::get_width() +
                                       TooltipWindow::get_width())
        , memory(MEMORY_LOCATIONS) {
    }

    void set_memory(const vector<Instruction> &m) {
        auto limit = min(memory.size(), m.size());
        copy(m.begin(), m.begin() + limit, memory.begin());
    }

    void draw() {
        for (auto i :
             vector<BoxedWindow *>{&window_machine_code, &window_mnemonic,
                                   &window_tooltip, &window_core}) {
            i->draw(il, core, memory);
        }
    }

    void execute() {
        il.execute(core, memory);
        core.ip = core.ip % MEMORY_LOCATIONS;
    }

private:
    const InstructionList &il;

    static const int MEMORY_LOCATIONS = 32;
    static const int HEIGHT = 2 + MEMORY_LOCATIONS;

    MachineCodeWindow window_machine_code;
    MnemonicWindow window_mnemonic;
    TooltipWindow window_tooltip;
    CoreCoreWindow window_core;

    Core core;
    vector<Instruction> memory;
};

class SyncedCoreWindow : public CoreWindow, public osc::OscPacketListener {
public:
    SyncedCoreWindow(WINDOW *parent, const InstructionList &il, int starty,
                     int startx)
        : CoreWindow(parent, il, starty, startx) {
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
                    execute();
                    draw();
                    refresh();
                }
            }
        } catch (const osc::Exception &e) {
            cout << "error parsing message: " << m.AddressPattern() << ": "
                 << e.what() << endl;
        }
    }
};

int main(int argc, char **argv) {
    try {
        initscr();

        if (!has_colors()) {
            throw runtime_error("terminal doesn't support color");
        }

        start_color();

        init_pair(1, COLOR_RED, COLOR_BLACK);

        cbreak();
        noecho();

        InstructionList instruction_list;

        if (argc != 2) {
            throw runtime_error("expected one argument");
        }

        ifstream infile(argv[1]);

        auto index = 0;
        vector<Instruction> memory(32);

        for (string line; getline(infile, line);) {
            if (!trim(line).empty()) {
                auto instr = instruction_list.assemble(line);
                memory[index++] = instr;
            }
        }

        SyncedCoreWindow cw(stdscr, instruction_list, 0, 0);
        cw.set_memory(memory);
        cw.draw();

        UdpListeningReceiveSocket s(
            IpEndpointName(IpEndpointName::ANY_ADDRESS, 7000), &cw);

        s.RunUntilSigInt();

    } catch (const runtime_error &re) {
        endwin();
        cout << re.what() << endl;
        return 1;
    }

    endwin();

    return 0;
}
