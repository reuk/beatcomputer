#include "editor.h"

#include "string_builder.h"
#include "logger.h"

#include <fstream>
#include <iomanip>
#include <algorithm>

using namespace std;

string machine_word(Instruction word) {
    return build_string(setfill('0'), setw(8), hex, word.raw);
}

Editor::Editor(const InstructionList &instruction_list,
               int memory_w,
               int mnemonics_w)
        : instruction_list(instruction_list)
        , head(0)
        , selected(Field::MNEMONICS)
        , storage(32, 0)
        , memory(memory_w)
        , mnemonics(mnemonics_w) {
    memory.add_listener_line_update(this);
    mnemonics.add_listener_line_update(this);
}

void Editor::load_from_file(const string &fname) {
    mnemonics.load_from_file(fname);
}

void Editor::do_command(unique_ptr<EditorCommand> &&command) {
    if (head != 0) {
        commands.erase(commands.end() - head, commands.end());
    }

    command->do_command(*this);
    commands.push_back(move(command));
    head = 0;
}

void Editor::undo_command() {
    if (head != commands.size()) {
        head += 1;
        auto &i = *(commands.end() - head);
        i->undo_command(*this);
    }
}

void Editor::redo_command() {
    if (head != 0) {
        head -= 1;
        auto &i = *(commands.end() - head);
        i->do_command(*this);
    }
}

vector<Instruction> Editor::get_memory() const {
    return storage;
}

Editor::Field Editor::get_selected() const {
    return selected;
}

void Editor::set_selected(Field field) {
    selected = field;
    selected_editor().select();
}

TextEditor &Editor::selected_editor() {
    return get_editor(get_selected());
}

TextEditor &Editor::get_editor(Field field) {
    switch (field) {
        case Field::MEMORY:
            return memory;
        case Field::MNEMONICS:
            return mnemonics;
    }

    throw runtime_error("no such editor field");
}

void Editor::line_updated(int line, const string &contents) {
    switch (get_selected()) {
        case Field::MEMORY:
            try {
                auto word = storage[line] = stol(contents, 0, 16);
                mnemonics.set_line(line, instruction_list.disassemble(word));
                call(&CompileOutputListener::line_compiled,
                     line,
                     CompileOutputListener::Type::GOOD,
                     instruction_list.tooltip(word));
            } catch (const runtime_error &re) {
                call(&CompileOutputListener::line_compiled,
                     line,
                     CompileOutputListener::Type::ERROR,
                     re.what());
            }
            break;
        case Field::MNEMONICS:
            try {
                auto copy = contents;
                auto word = storage[line] = instruction_list.assemble(copy);
                memory.set_line(line, machine_word(word));
                call(&CompileOutputListener::line_compiled,
                     line,
                     CompileOutputListener::Type::GOOD,
                     instruction_list.tooltip(word));
            } catch (const runtime_error &re) {
                call(&CompileOutputListener::line_compiled,
                     line,
                     CompileOutputListener::Type::ERROR,
                     re.what());
            }
            break;
    }
}
