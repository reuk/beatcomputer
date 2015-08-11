#include "input.h"

Input::Input(Type type, int value)
        : type(type)
        , value(value) {
}

Input::Type Input::get_type() const {
    return type;
}

int Input::get_value() const {
    return value;
}
