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

using namespace std;

string machine_word(uint32_t word) {
    stringstream ss;
    ss << setfill('0') << setw(8) << hex << word;
    return ss.str();
}

class MachineCodeWindow : public Window {
public:
    MachineCodeWindow(WINDOW * parent, int height, int width, int starty,
                      int startx)
        : Window(parent, height, width, starty, startx) {
    }

    void draw(Core & core, vector<Instruction> & memory) {
        erase();
        box(0, 0);

        auto line = 0;
        for (const auto & i : memory) {
            auto attr = line == core.ip;

            if (attr)
                w_attron(A_BOLD | COLOR_PAIR(1));
            print(1 + line, 1, machine_word(i.raw));
            if (attr)
                w_attroff(A_BOLD | COLOR_PAIR(1));

            line += 1;
        }

        refresh();
    }
};

class LabelWindow : public Window {
public:
    LabelWindow(WINDOW * parent, int height, int width, int starty, int startx)
        : Window(parent, height, width, starty, startx) {
    }

    void draw(Core & core, vector<Instruction> & memory) {
        erase();
        box(0, 0);

        refresh();
    }
};

class MnemonicWindow : public Window {
public:
    MnemonicWindow(WINDOW * parent, int height, int width, int starty,
                   int startx)
        : Window(parent, height, width, starty, startx) {
    }

    void draw(const InstructionList & il, Core & core,
              vector<Instruction> & memory) {
        erase();
        box(0, 0);

        auto line = 0;
        for (const auto & i : memory) {
            auto attr = line == core.ip;

            if (attr)
                w_attron(A_BOLD | COLOR_PAIR(1));
            print(1 + line, 1, il.disassemble(i));
            if (attr)
                w_attroff(A_BOLD | COLOR_PAIR(1));

            line += 1;
        }

        refresh();
    }
};

class TooltipWindow : public Window {
public:
    TooltipWindow(WINDOW * parent, int height, int width, int starty,
                  int startx)
        : Window(parent, height, width, starty, startx) {
    }

    void draw(const InstructionList & il, Core & core,
              vector<Instruction> & memory) {
        erase();
        box(0, 0);

        auto line = 0;
        for (const auto & i : memory) {
            auto attr = line == core.ip;

            if (attr)
                w_attron(A_BOLD | COLOR_PAIR(1));
            print(1 + line, 1, il.tooltip(i));
            if (attr)
                w_attroff(A_BOLD | COLOR_PAIR(1));

            line += 1;
        }

        refresh();
    }
};

class CoreCoreWindow : public Window {
public:
    CoreCoreWindow(WINDOW * parent, int height, int width, int starty,
                   int startx)
        : Window(parent, height, width, starty, startx) {
    }

    void draw(Core & core) {
        erase();
        box(0, 0);

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

        refresh();
    }
};

class CoreWindow : public Window {
public:
    CoreWindow(WINDOW * parent, const InstructionList & il, int starty,
               int startx)
        : Window(parent, HEIGHT,
                 WIDTH_MACHINE_CODE + WIDTH_LABEL + WIDTH_MNEMONIC +
                     WIDTH_TOOLTIP + WIDTH_CORE,
                 starty, startx)
        , il(il)
        , tooltips_shown(true)
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

    void set_memory(const vector<Instruction> & m) {
        auto limit = min(memory.size(), m.size());
        copy(m.begin(), m.begin() + limit, memory.begin());
    }

    void draw() {
        window_machine_code.draw(core, memory);
        window_label.draw(core, memory);
        window_mnemonic.draw(il, core, memory);
        window_tooltip.draw(il, core, memory);
        window_core.draw(core);
    }

    void execute() {
        il.execute(core, memory);
        core.ip = core.ip % MEMORY_LOCATIONS;
    }

private:
    const InstructionList & il;

    static const int MEMORY_LOCATIONS = 32;
    static const int HEIGHT = 2 + MEMORY_LOCATIONS;

    static const int WIDTH_MACHINE_CODE = 2 + 8;
    static const int WIDTH_LABEL = 2 + 10;
    static const int WIDTH_MNEMONIC = 2 + 20;
    static const int WIDTH_TOOLTIP = 2 + 30;
    static const int WIDTH_CORE = 2 + 5 + 8;

    bool tooltips_shown;

    MachineCodeWindow window_machine_code;
    LabelWindow window_label;
    MnemonicWindow window_mnemonic;
    TooltipWindow window_tooltip;
    CoreCoreWindow window_core;

public:
    Core core;
    vector<Instruction> memory;
};

int main(int argc, char ** argv) {
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

        CoreWindow cw(stdscr, instruction_list, 0, 0);
        cw.set_memory(memory);

        for (;;) {
            cw.refresh();
            cw.draw();
            this_thread::sleep_for(std::chrono::milliseconds(500));
            cw.execute();
        }

    } catch (const runtime_error & re) {
        cout << re.what() << endl;
        return 1;
    }

    endwin();

    return 0;
}
