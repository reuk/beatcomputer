#include "text_editor.h"

#include "trim.h"

#include <fstream>

using namespace std;

Vec2::Vec2(int y, int x)
        : y(y)
        , x(x) {
}

TextEditor::TextEditor(int line_length): line_length(line_length) {}

int clamp(int in, int mini, int maxi) {
    return min(max(in, mini), maxi);
}

void TextEditor::load_from_file(const string & fname) {
    contents.clear();

    ifstream infile(fname);
    for (string line; getline(infile, line);) {
        if (!trim(line).empty()) {
            contents.push_back(line);
        }
    }

    set_contents(contents);
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

    cursor.x = clamp(cursor.x, 0, min(line_length - 1, (int)contents[cursor.y].size()));

    ListenerList<TextEditorListener>::call(
        &TextEditorListener::cursor_moved, cursor);
}

void TextEditor::select() const {
    ListenerList<TextEditorListener>::call(
        &TextEditorListener::cursor_moved, cursor);
}

void TextEditor::insert_character(char character) {
    if (character == '\n') {
        split_line();
        return;
    }

    auto & t = contents[cursor.y];

    if (t.size() >= line_length) {
        t[cursor.x] = character;
    } else {
        t.insert(t.begin() + cursor.x, character);
    }

    move_cursor(Direction::RIGHT);
    ListenerList<TextEditorListener>::call(
        &TextEditorListener::line_modified, cursor.y, t);
    ListenerList<LineUpdateListener>::call(
        &LineUpdateListener::line_updated, cursor.y, t);
}

char TextEditor::backspace() {
    auto & t = contents[cursor.y];

    if (cursor.x == 0) {
        join_line();
    } else {
        t.erase(t.begin() + cursor.x - 1);
    }

    move_cursor(Direction::LEFT);
    ListenerList<TextEditorListener>::call(
        &TextEditorListener::line_modified, cursor.y, t);
    ListenerList<LineUpdateListener>::call(
        &LineUpdateListener::line_updated, cursor.y, t);
}

const vector<string> & TextEditor::get_contents() const {
    return contents;
}

void TextEditor::set_contents(const vector<string> & in) {
    contents = in;

    auto y = 0;
    for (auto line : contents) {
        ListenerList<TextEditorListener>::call(
            &TextEditorListener::line_modified, y, line);
        ListenerList<LineUpdateListener>::call(
            &LineUpdateListener::line_updated, y, line);

        y += 1;
    }
}

void TextEditor::split_line() {
    //  TODO
}

void TextEditor::join_line() {
    //  TODO
}

void TextEditor::set_line(int line, const string & in) {
    contents[line] = in;

    ListenerList<TextEditorListener>::call(
        &TextEditorListener::line_modified, line, contents[line]);
}
