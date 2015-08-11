#pragma once

#include "instruction_list.h"
#include "instructions.h"
#include "editor_command.h"
#include "text_editor.h"

#include <list>
#include <string>

class Editor : public LineUpdateListener {
public:
    Editor(const InstructionList &instruction_list);

    void load_from_file(const std::string &fname);

    void do_command(std::unique_ptr<EditorCommand> &&command);
    void undo_command();
    void redo_command();

    std::vector<Instruction> get_memory() const;

    enum class Field { MEMORY, MNEMONICS };

    Field get_selected() const;
    void set_selected(Field field);

    TextEditor &get_editor(Field field);
    TextEditor &selected_editor();

    void line_updated(int line, const std::string & contents) override;

private:
    const InstructionList &instruction_list;
    std::vector<std::unique_ptr<EditorCommand>> commands;
    decltype(commands)::size_type head;

    Field selected;

    std::vector<Instruction> storage;

public:
    TextEditor memory;
    TextEditor mnemonics;
};

std::string machine_word(Instruction word);
