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

#define MEM_SIZE 32

void TextEditor::insert_character(char character) {
    if (character == '\n' && contents.size() != MEM_SIZE) {
        split_line();
        return;
    }

    auto & t = contents[cursor.y];

    if (t.size() >= line_length) {
        t[cursor.x] = character;
    } else {
        t.insert(t.begin() + cursor.x, character);
    }

    ListenerList<TextEditorListener>::call(
        &TextEditorListener::line_modified, cursor.y, t);
    ListenerList<LineUpdateListener>::call(
        &LineUpdateListener::line_updated, cursor.y, t);
    move_cursor(Direction::RIGHT);
}

char TextEditor::backspace() {
    if (cursor.x == 0) {
        if (cursor.y != 0)
            join_line();
        return '\n';
    }

    auto & t = contents[cursor.y];
    auto ret = t[cursor.x];

    t.erase(t.begin() + cursor.x - 1);

    ListenerList<TextEditorListener>::call(
        &TextEditorListener::line_modified, cursor.y, t);
    ListenerList<LineUpdateListener>::call(
        &LineUpdateListener::line_updated, cursor.y, t);
    move_cursor(Direction::LEFT);

    return ret;
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
    auto t = contents[cursor.y];
    contents[cursor.y] = string(t.begin(), t.begin() + cursor.x);
    contents.insert(contents.begin() + cursor.y + 1, string(t.begin() + cursor.x, t.end()));

    for (auto i = cursor.y; i != contents.size(); ++i) {
        ListenerList<TextEditorListener>::call(
            &TextEditorListener::line_modified, i, contents[i]);
        ListenerList<LineUpdateListener>::call(
            &LineUpdateListener::line_updated, i, contents[i]);
    }

    move_cursor(Direction::DOWN);
    while (cursor.x != 0)
        move_cursor(Direction::LEFT);
}

void TextEditor::join_line() {
    auto & l_prev = contents[cursor.y - 1];
    auto pos = l_prev.size();
    auto & l_this = contents[cursor.y];
    l_prev.insert(l_prev.end(), l_this.begin(), l_this.end());
    l_prev.resize(line_length);

    contents.erase(contents.begin() + cursor.y);

    for (auto i = cursor.y - 1; i != contents.size(); ++i) {
        ListenerList<TextEditorListener>::call(
            &TextEditorListener::line_modified, i, contents[i]);
        ListenerList<LineUpdateListener>::call(
            &LineUpdateListener::line_updated, i, contents[i]);
    }

    move_cursor(Direction::UP);
    while (pos--)
        move_cursor(Direction::RIGHT);
}

void TextEditor::set_line(int line, const string & in) {
    contents[line] = in;

    ListenerList<TextEditorListener>::call(
        &TextEditorListener::line_modified, line, contents[line]);
}
