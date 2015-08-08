#include "editor_command.h"
#include "editor.h"

void EditorCommand::do_command(Editor &e) {}
void EditorCommand::undo_command(Editor &e) {}

InsertCommand::InsertCommand(char character)
    : character(character) {
}

void InsertCommand::do_command(Editor &e) {
}

void InsertCommand::undo_command(Editor &e) {
}

void BackspaceCommand::do_command(Editor &e) {
}

void BackspaceCommand::undo_command(Editor &e) {
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
}

void MoveCommand::undo_command(Editor &e) {
    e.selected_editor().move_cursor(opposite(direction));
}
