#pragma once

#include "instruction_list.h"
#include "instructions.h"
#include "editor_command.h"

#include <queue>
#include <string>

class Editor {
public:
    Editor(const InstructionList &instruction_list);

    void load_from_file(const std::string &fname);

    void do_command(std::unique_ptr<EditorCommand> &&command);
    void undo_command();
    void redo_command();
    void sync_from_memory();
    void sync_from_mnemonics();

    std::vector<Instruction> get_memory() const;
    std::vector<std::string> get_mnemonics() const;

private:
    const InstructionList &instruction_list;
    std::queue<std::unique_ptr<EditorCommand>> commands;

public:
    std::vector<std::pair<Instruction, std::string>> store;
};
