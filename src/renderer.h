#pragma once
#include "buffer.h"
#include <string>

namespace minivim {

class Renderer {
public:
    Renderer() = default;
    ~Renderer() = default;

    void clearScreen();
    void moveCursor(int row, int col);
    void draw(const GapBuffer& buffer, int screen_rows, int screen_cols, int row_offset, int col_offset);
    void drawStatusBar(const std::string& filename, int cx, int cy, int screen_rows, int screen_cols);
    void refresh();

private:
    std::string m_write_buffer;
};

} // namespace minivim
