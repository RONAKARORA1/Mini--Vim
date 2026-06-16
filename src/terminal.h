#pragma once
#include <termios.h>
#include <utility>

namespace minivim {

enum class Key {
    ARROW_LEFT = 1000,
    ARROW_RIGHT,
    ARROW_UP,
    ARROW_DOWN,
    DEL_KEY,
    PAGE_UP,
    PAGE_DOWN,
    BACKSPACE = 127,
    ENTER = 13,
    ESC = 27
};

class Terminal {
public:
    Terminal();
    ~Terminal();

    Terminal(const Terminal&) = delete;
    Terminal& operator=(const Terminal&) = delete;

    int readKey();
    std::pair<int, int> getWindowSize();

private:
    termios m_original_termios;
};

} // namespace minivim
