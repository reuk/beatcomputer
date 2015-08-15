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

#define STORAGE_SIZE 32

Editor::Editor(const InstructionList &instruction_list,
               vector<Instruction> &storage,
               int memory_w,
               int mnemonics_w)
        : storage(storage)
        , instruction_list(instruction_list)
        , head(0)
        , selected(Field::MNEMONICS)
        , memory(storage.size(), memory_w, "00000000")
        , mnemonics(storage.size(), mnemonics_w, "NOP") {
    memory.add_listener_line_update(this);
    mnemonics.add_listener_line_update(this);
}

void Editor::load_from_file(const string &fname) {
    set_selected(Field::MNEMONICS);
    mnemonics.load_from_file(fname);
}

void Editor::blank() {
    set_selected(Field::MNEMONICS);
    mnemonics.blank();
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
                auto word = storage[line] = 0;
                mnemonics.set_line(line, instruction_list.disassemble(word));
                call(&CompileOutputListener::line_compiled,
                     line,
                     CompileOutputListener::Type::ERROR,
                     re.what());
            }
            break;
        case Field::MNEMONICS:
            auto copy = contents;
            try {
                auto word = storage[line] = instruction_list.assemble(copy);
                memory.set_line(line, machine_word(word));
                call(&CompileOutputListener::line_compiled,
                     line,
                     CompileOutputListener::Type::GOOD,
                     instruction_list.tooltip(word));
            } catch (const runtime_error &re) {
                auto word = storage[line] = 0;
                memory.set_line(line, machine_word(word));
                call(&CompileOutputListener::line_compiled,
                     line,
                     CompileOutputListener::Type::ERROR,
                     re.what());
            }
            break;
    }
}
