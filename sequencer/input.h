#pragma once

class Input {
public:
    enum class Type {
        TICK,
        KEY,
    };

    Input(Type type = Type::TICK, int value = 0);

    Type get_type() const;
    int get_value() const;

private:
    Type type;
    int value;
};
