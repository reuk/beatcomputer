#include "editor_command.h"

InsertCommand::InsertCommand(int character)
    : character(character) {
}

void InsertCommand::do_command(Editor &e) {
    //  go to (y, x) and insert the character
}

void InsertCommand::undo_command(Editor &e) {
    //  go to (y, x) and remove the character
}
void BackspaceCommand::do_command(Editor &e) {
    //  go to (y, x) and insert the character
}

void BackspaceCommand::undo_command(Editor &e) {
    //  go to (y, x) and remove the character
}
