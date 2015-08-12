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
    virtual void cursor_moved(const Vec2 & pos) = 0;
    virtual void line_modified(int line, const std::string & contents) = 0;
};

struct LineUpdateListener {
    virtual void line_updated(int line, const std::string & contents) = 0;
};

class TextEditor : public ListenerList<TextEditorListener>,
                   public ListenerList<LineUpdateListener> {
public:
    void load_from_file(const std::string & fname);

    void move_cursor(Direction direction);
    void insert_character(char character);
    char backspace();
    void select() const;

    const std::vector<std::string> & get_contents() const;
    void set_contents(const std::vector<std::string> & in);

    void set_line(int line, const std::string & in);

    void split_line();

    template <typename T>
    void add_listener_text_editor(T t) {
        ListenerList<TextEditorListener>::add_listener(t);
    }

    template <typename T>
    void add_listener_line_update(T t) {
        ListenerList<LineUpdateListener>::add_listener(t);
    }

private:
    std::vector<std::string> contents;
    Vec2 cursor;
};
