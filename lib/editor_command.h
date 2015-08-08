#pragma once

enum class Direction { UP, DOWN, LEFT, RIGHT };

class Editor;

struct EditorCommand {
    virtual void do_command(Editor &e);
    virtual void undo_command(Editor &e);
};

//  Data required for do_command should be invariant/const
//  somehow it should be fed to the constructor

class InsertCommand : public EditorCommand {
public:
    InsertCommand(char character);
    void do_command(Editor &e) override;
    void undo_command(Editor &e) override;
private:
    const int character;
};

class BackspaceCommand : public EditorCommand {
public:
    void do_command(Editor &e) override;
    void undo_command(Editor &e) override;
};

class MoveCommand : public EditorCommand {
public:
    MoveCommand(Direction direction);
    void do_command(Editor &e) override;
    void undo_command(Editor &e) override;
private:
    Direction direction;
};

class SelectCommand : public EditorCommand {
public:
    void do_command(Editor &e) override;
    void undo_command(Editor &e) override;
};
