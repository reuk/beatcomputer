#include "editor.h"

#include <fstream>

using namespace std;

Editor::Editor(const InstructionList &instruction_list)
    : instruction_list(instruction_list) {
}

void Editor::load_from_file(const string &fname) {
    ifstream infile(fname);

    auto index = 0;
    for (string line; getline(infile, line);) {
        if (!trim(line).empty()) {
            store.push_back(make_pair(instruction_list.assemble(line), ""));
            index += 1;
        }
    }

    sync_from_memory();
}

void Editor::do_command(unique_ptr<EditorCommand> &&command) {
    command->do_command(*this);
    commands.push(move(command));
}

void Editor::undo_command() {
    if (!commands.empty()) {
        auto i = move(commands.front());
        commands.pop();
        i->undo_command(*this);
    }
}

void Editor::sync_from_memory() {
    for (auto i : store) {
        i.second = instruction_list.disassemble(i.first);
    }
}

void Editor::sync_from_mnemonics() {
    for (auto i : store) {
        i.first = instruction_list.assemble(i.second);
    }
}

vector<Instruction> Editor::get_memory() const {
    vector<Instruction> ret(store.size());
    transform(begin(store), end(store), begin(ret),
              [](auto i) { return i.first; });
    return ret;
}

vector<string> Editor::get_mnemonics() const {
    vector<string> ret(store.size());
    transform(begin(store), end(store), begin(ret),
              [](auto i) { return i.second; });
    return ret;
}
