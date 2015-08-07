#pragma once

#include "editor_command.h"
#include "listener_list.h"

#include <vector>
#include <string>

struct Vec2 {
    Vec2(int y = 0, int x = 0);
    int y, x;
};

struct TextEditorListener {
    virtual void cursor_moved(int y, int x) = 0;
    virtual void character_added(char character) = 0;
};

class TextEditor : public ListenerList<TextEditorListener> {
public:
    void load_from_file(const std::string & fname);

    void move_cursor(Direction direction);
    void insert_character(char character);
    void backspace();
    void del();

    const std::vector<std::string> & get_contents() const;
    void set_contents(const std::vector<std::string> & in);

    void split_line();

private:
    std::vector<std::string> contents;
    Vec2 cursor;
};
