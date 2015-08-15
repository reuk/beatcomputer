#pragma once

#include "instruction_list.h"
#include "instructions.h"
#include "editor_command.h"
#include "text_editor.h"

#include <list>
#include <string>

struct CompileOutputListener {
    enum class Type {
        GOOD,
        WARNING,
        ERROR,
    };
    virtual void line_compiled(int line,
                               Type type,
                               const std::string &output) = 0;
};

class Editor : public LineUpdateListener,
               public ListenerList<CompileOutputListener> {
public:
    Editor(const InstructionList &instruction_list,
           std::vector<Instruction> &storage,
           int memory_w,
           int mnemonics_w);

    void load_from_file(const std::string &fname);
    void blank();

    void do_command(std::unique_ptr<EditorCommand> &&command);
    void undo_command();
    void redo_command();

    enum class Field { MEMORY, MNEMONICS };

    Field get_selected() const;
    void set_selected(Field field);

    TextEditor &get_editor(Field field);
    TextEditor &selected_editor();

    void line_updated(int line, const std::string &contents) override;

private:
    std::vector<Instruction> &storage;

    const InstructionList &instruction_list;
    std::vector<std::unique_ptr<EditorCommand>> commands;
    decltype(commands)::size_type head;

    Field selected;

public:
    TextEditor memory;
    TextEditor mnemonics;
};

std::string machine_word(Instruction word);
