#pragma once

#include "window.h"

#include <vector>
#include <string>

struct Vec2 {
    Vec2(int y = 0, int x = 0);
    int y, x;
};

class TextEditor {
public:
    enum class Direction { UP, DOWN, LEFT, RIGHT };

    void load_from_file(const std::string & fname);

    void move_cursor(Direction direction);
    void replace_character(char character);
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
