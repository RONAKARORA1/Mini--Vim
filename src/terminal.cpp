#include "terminal.h"
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdexcept>
#include <cerrno>
#include <system_error>

namespace minivim {

Terminal::Terminal() {
    if (tcgetattr(STDIN_FILENO, &m_original_termios) == -1) {
        throw std::system_error(errno, std::generic_category(), "tcgetattr failed");
    }

    termios raw = m_original_termios;
    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    raw.c_oflag &= ~(OPOST);
    raw.c_cflag |= (CS8);
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) {
        throw std::system_error(errno, std::generic_category(), "tcsetattr failed");
    }
}

Terminal::~Terminal() {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &m_original_termios);
}

int Terminal::readKey() {
    int nread;
    char c;
    
    while ((nread = read(STDIN_FILENO, &c, 1)) != 1) {
        if (nread == -1 && errno != EAGAIN) {
            throw std::system_error(errno, std::generic_category(), "read failed");
        }
    }

    if (c == '\x1b') {
        char seq[3];
        if (read(STDIN_FILENO, &seq[0], 1) != 1) return '\x1b';
        if (read(STDIN_FILENO, &seq[1], 1) != 1) return '\x1b';

        if (seq[0] == '[') {
            if (seq[1] >= '0' && seq[1] <= '9') {
                if (read(STDIN_FILENO, &seq[2], 1) != 1) return '\x1b';
                if (seq[2] == '~') {
                    switch (seq[1]) {
                        case '3': return static_cast<int>(Key::DEL_KEY);
                        case '5': return static_cast<int>(Key::PAGE_UP);
                        case '6': return static_cast<int>(Key::PAGE_DOWN);
                    }
                }
            } else {
                switch (seq[1]) {
                    case 'A': return static_cast<int>(Key::ARROW_UP);
                    case 'B': return static_cast<int>(Key::ARROW_DOWN);
                    case 'C': return static_cast<int>(Key::ARROW_RIGHT);
                    case 'D': return static_cast<int>(Key::ARROW_LEFT);
                }
            }
        }
        return '\x1b';
    }
    
    return c;
}

std::pair<int, int> Terminal::getWindowSize() {
    winsize ws;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
        throw std::system_error(errno, std::generic_category(), "ioctl TIOCGWINSZ failed");
    }
    return {ws.ws_row, ws.ws_col};
}

} // namespace minivim
