#!/bin/zsh
ls src/*.(h|cpp) | xargs clang-format -i
