#include "text_editor.h"

#include "trim.h"

#include <fstream>

using namespace std;

Vec2::Vec2(int y, int x): y (y), x(x) {}

int clamp(int in, int mini, int maxi) {
    return min(max(in, mini), maxi);
}

void TextEditor::load_from_file(const std::string & fname) {
    ifstream infile(fname);
    for (string line; getline(infile, line);) {
        if (! trim(line).empty()) {
            contents.push_back(line);
        }
    }
}

void TextEditor::move_cursor(Direction direction) {
    switch (direction) {
        case Direction::UP:
            if (cursor.y <= 0) {
                break;
            }
            cursor.y -= 1;
            break;
        case Direction::DOWN:
            if (cursor.y >= contents.size() - 1) {
                cursor.x = contents.back().size();
                break;
            }
            cursor.y += 1;
            break;
        case Direction::LEFT:
            cursor.x -= 1;
            break;
        case Direction::RIGHT:
            cursor.x += 1;
            break;
    }

    cursor.x = clamp(cursor.x, 0, contents[cursor.y].size());
}

void TextEditor::replace_character(char character) {
    if (character == '\n') {
        backspace();
        split_line();
    } else {
        contents[cursor.y][cursor.x] = character;
    }
}

void TextEditor::insert_character(char character) {
    if (character == '\n') {
        split_line();
    } else {
        auto & t = contents[cursor.y];
        t.insert(t.begin() + cursor.x, character);
    }
}

void TextEditor::backspace() {

}

void TextEditor::del() {

}

const vector<string> & TextEditor::get_contents() const {
    return contents;
}

void TextEditor::set_contents(const std::vector<std::string> & in) {
    contents = in;
}

void TextEditor::split_line() {

}
