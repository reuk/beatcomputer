#include "editor.h"

#include "string_builder.h"
#include "logger.h"

#include <fstream>
#include <iomanip>
#include <algorithm>

using namespace std;

string machine_word(uint32_t word) {
    return build_string(setfill('0'), setw(8), hex, word);
}

Editor::Editor(const InstructionList &instruction_list)
    : instruction_list(instruction_list)
    , head(0)
    , selected(Field::MNEMONICS) {
}

void Editor::load_from_file(const string &fname) {
    mnemonics.load_from_file(fname);
    sync_from_mnemonics();
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

void Editor::sync() {
    switch (get_selected()) {
        case Field::MEMORY:
            sync_from_memory();
            break;
        case Field::MNEMONICS:
            sync_from_mnemonics();
            break;
    }
}

void Editor::sync_from_memory() {
    auto memory_contents = get_memory();
    vector<string> ret(memory_contents.size());
    transform(begin(memory_contents), end(memory_contents), begin(ret),
              [this](auto i) { return instruction_list.disassemble(i); });
    mnemonics.set_contents(ret);
}

void Editor::sync_from_mnemonics() {
    auto mnemonics_contents = get_mnemonics();
    vector<string> ret(mnemonics_contents.size());
    transform(begin(mnemonics_contents), end(mnemonics_contents), begin(ret),
              [this](auto i) {
                  return machine_word(instruction_list.assemble(i).raw);
              });
    memory.set_contents(ret);
}

vector<Instruction> Editor::get_memory() const {
    auto memory_contents = memory.get_contents();
    vector<Instruction> ret(memory_contents.size());
    transform(begin(memory_contents), end(memory_contents), begin(ret),
              [](auto i) { return stol(i, 0, 16); });
    return ret;
}

vector<string> Editor::get_mnemonics() const {
    return mnemonics.get_contents();
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
