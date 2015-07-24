#!/bin/zsh
ls sequencer/*.(h|cpp) | xargs clang-format -i
ls time_server/*.(h|cpp) | xargs clang-format -i
ls tests/*.(h|cpp) | xargs clang-format -i
