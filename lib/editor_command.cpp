#include "editor_command.h"
#include "editor.h"

#include <stdexcept>

using namespace std;

void EditorCommand::do_command(Editor &e) {
}
void EditorCommand::undo_command(Editor &e) {
}

InsertCommand::InsertCommand(char character)
        : character(character) {
}

void InsertCommand::do_command(Editor &e) {
    e.selected_editor().insert_character(character);
}

void InsertCommand::undo_command(Editor &e) {
    e.selected_editor().backspace();
}

void BackspaceCommand::do_command(Editor &e) {
    character = e.selected_editor().backspace();
}

void BackspaceCommand::undo_command(Editor &e) {
    e.selected_editor().insert_character(character);
}

MoveCommand::MoveCommand(Direction direction)
        : direction(direction) {
}

void MoveCommand::do_command(Editor &e) {
    e.selected_editor().move_cursor(direction);
}

Direction opposite(Direction direction) {
    switch (direction) {
        case Direction::LEFT:
            return Direction::RIGHT;
        case Direction::RIGHT:
            return Direction::LEFT;
        case Direction::UP:
            return Direction::DOWN;
        case Direction::DOWN:
            return Direction::UP;
    }

    throw runtime_error("no such direction");
}

void MoveCommand::undo_command(Editor &e) {
    e.selected_editor().move_cursor(opposite(direction));
}

void SelectCommand::do_command(Editor &e) {
    switch (e.get_selected()) {
        case Editor::Field::MEMORY:
            e.set_selected(Editor::Field::MNEMONICS);
            break;
        case Editor::Field::MNEMONICS:
            e.set_selected(Editor::Field::MEMORY);
            break;
    }
}

void SelectCommand::undo_command(Editor &e) {
    switch (e.get_selected()) {
        case Editor::Field::MEMORY:
            e.set_selected(Editor::Field::MNEMONICS);
            break;
        case Editor::Field::MNEMONICS:
            e.set_selected(Editor::Field::MEMORY);
            break;
    }
}
