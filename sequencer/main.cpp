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
    MachineCodeWindow(WINDOW *parent, int height, int width, int starty,
                      int startx)
        : BoxedWindow(parent, height, width, starty, startx) {
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
};

class LabelWindow : public BoxedWindow {
public:
    LabelWindow(WINDOW *parent, int height, int width, int starty, int startx)
        : BoxedWindow(parent, height, width, starty, startx) {
    }

    void draw_contents(const InstructionList &il, Core &core,
                       vector<Instruction> &memory) const override {
    }
};

class MnemonicWindow : public BoxedWindow {
public:
    MnemonicWindow(WINDOW *parent, int height, int width, int starty,
                   int startx)
        : BoxedWindow(parent, height, width, starty, startx) {
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
};

class TooltipWindow : public BoxedWindow {
public:
    TooltipWindow(WINDOW *parent, int height, int width, int starty, int startx)
        : BoxedWindow(parent, height, width, starty, startx) {
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
};

class CoreCoreWindow : public BoxedWindow {
public:
    CoreCoreWindow(WINDOW *parent, int height, int width, int starty,
                   int startx)
        : BoxedWindow(parent, height, width, starty, startx) {
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
};

class CoreWindow : public Window {
public:
    CoreWindow(WINDOW *parent, const InstructionList &il, int starty,
               int startx)
        : Window(parent, HEIGHT,
                 WIDTH_MACHINE_CODE + WIDTH_LABEL + WIDTH_MNEMONIC +
                     WIDTH_TOOLTIP + WIDTH_CORE,
                 starty, startx)
        , il(il)
        , window_machine_code(*this, 0, WIDTH_MACHINE_CODE, 0, 0)
        , window_label(*this, 0, WIDTH_LABEL, 0, WIDTH_MACHINE_CODE)
        , window_mnemonic(*this, 0, WIDTH_MNEMONIC, 0,
                          WIDTH_MACHINE_CODE + WIDTH_LABEL)
        , window_tooltip(*this, 0, WIDTH_TOOLTIP, 0,
                         WIDTH_MACHINE_CODE + WIDTH_LABEL + WIDTH_MNEMONIC)
        , window_core(
              *this, 0, WIDTH_CORE, 0,
              WIDTH_MACHINE_CODE + WIDTH_LABEL + WIDTH_MNEMONIC + WIDTH_TOOLTIP)
        , memory(MEMORY_LOCATIONS) {
        box(0, 0);

        window_machine_code.box(0, 0);
        window_label.box(0, 0);
        window_mnemonic.box(0, 0);
        window_tooltip.box(0, 0);
        window_core.box(0, 0);
    }

    void set_memory(const vector<Instruction> &m) {
        auto limit = min(memory.size(), m.size());
        copy(m.begin(), m.begin() + limit, memory.begin());
    }

    void draw() {
        for (auto i : vector<BoxedWindow *>{&window_machine_code, &window_label,
                                            &window_mnemonic, &window_tooltip,
                                            &window_core}) {
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

    static const int WIDTH_MACHINE_CODE = 2 + 8;
    static const int WIDTH_LABEL = 2 + 10;
    static const int WIDTH_MNEMONIC = 2 + 20;
    static const int WIDTH_TOOLTIP = 2 + 30;
    static const int WIDTH_CORE = 2 + 5 + 8;

    MachineCodeWindow window_machine_code;
    LabelWindow window_label;
    MnemonicWindow window_mnemonic;
    TooltipWindow window_tooltip;
    CoreCoreWindow window_core;

public:
    Core core;
    vector<Instruction> memory;
};

class SyncedCoreWindow : public CoreWindow, public osc::OscPacketListener {
public:
    SyncedCoreWindow(WINDOW *parent, const InstructionList &il, int starty, int startx)
        : CoreWindow(parent, il, starty, startx) {
    }
protected:
    void ProcessMessage(const osc::ReceivedMessage & m, const IpEndpointName & ip) override {
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
        } catch (const osc::Exception & e) {
            cout << "error parsing message: " << m.AddressPattern() << ": " << e.what() << endl;
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

        UdpListeningReceiveSocket s(IpEndpointName(IpEndpointName::ANY_ADDRESS, 7000), &cw);

        s.RunUntilSigInt();

    } catch (const runtime_error &re) {
        cout << re.what() << endl;
        return 1;
    }

    endwin();

    return 0;
}
