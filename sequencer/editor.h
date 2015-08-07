#pragma once

#include "instruction_list.h"
#include "instructions.h"
#include "editor_command.h"
#include "text_editor.h"

#include <list>
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
    void sync();

    std::vector<Instruction> get_memory() const;
    std::vector<std::string> get_mnemonics() const;

    enum class Field { MEMORY, MNEMONICS };

    Field get_selected() const;
    void set_selected(Field field);

    TextEditor &get_editor(Field field);
    TextEditor &selected_editor();
private:
    const InstructionList &instruction_list;
    std::vector<std::unique_ptr<EditorCommand>> commands;
    int head;

    Field selected;

public:
    TextEditor memory;
    TextEditor mnemonics;
};

std::string machine_word(uint32_t word);
