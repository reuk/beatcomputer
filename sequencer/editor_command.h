#pragma once

class Editor;

struct EditorCommand {
    virtual void do_command(Editor &e) = 0;
    virtual void undo_command(Editor &e) = 0;
};

//  Data required for do_command should be invariant/const
//  somehow it should be fed to the constructor

class InsertCommand : public EditorCommand {
public:
    InsertCommand(int character);
    void do_command(Editor &e) override;
    void undo_command(Editor &e) override;
private:
    const int character;
};

class BackspaceCommand : public EditorCommand {
public:
    void do_command(Editor &e) override;
    void undo_command(Editor &e) override;
private:
};
