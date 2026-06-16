#include "renderer.h"
#include <unistd.h>

namespace minivim {

void Renderer::clearScreen() {
    m_write_buffer.append("\x1b[2J");
    m_write_buffer.append("\x1b[H");
}

void Renderer::moveCursor(int row, int col) {
    m_write_buffer.append("\x1b[" + std::to_string(row + 1) + ";" + std::to_string(col + 1) + "H");
}

void Renderer::draw(const GapBuffer& buffer, int screen_rows, int screen_cols, int row_offset, int col_offset) {
    m_write_buffer.append("\x1b[H");

    size_t buf_idx = 0;
    size_t current_row = 0;

    while (current_row < static_cast<size_t>(row_offset) && buf_idx < buffer.getLength()) {
        if (buffer.charAt(buf_idx) == '\n') current_row++;
        buf_idx++;
    }

    for (int r = 0; r < screen_rows; ++r) {
        int current_col = 0;

        size_t temp_idx = buf_idx;
        int skipped = 0;
        while (skipped < col_offset && temp_idx < buffer.getLength() && buffer.charAt(temp_idx) != '\n') {
            temp_idx++;
            skipped++;
        }
        buf_idx = temp_idx;

        while (buf_idx < buffer.getLength() && current_col < screen_cols) {
            char c = buffer.charAt(buf_idx);
            if (c == '\n') break;
            m_write_buffer.push_back(c);
            current_col++;
            buf_idx++;
        }

        while (buf_idx < buffer.getLength() && buffer.charAt(buf_idx) != '\n') buf_idx++;
        if (buf_idx < buffer.getLength() && buffer.charAt(buf_idx) == '\n') buf_idx++;

        m_write_buffer.append("\x1b[K");

        if (r < screen_rows - 1) {
            m_write_buffer.append("\r\n");
        }
    }
}

void Renderer::drawStatusBar(const std::string& filename, int cx, int cy, int screen_rows, int screen_cols) {
    m_write_buffer.append("\x1b[" + std::to_string(screen_rows) + ";1H");
    m_write_buffer.append("\x1b[7m");

    std::string display_name = filename.empty() ? "[No Name]" : filename;
    std::string status = " mini_vim - " + display_name;
    std::string cursor_pos = "Row: " + std::to_string(cy) + " Col: " + std::to_string(cx) + " ";

    int len = status.length();
    if (len > screen_cols) len = screen_cols;
    m_write_buffer.append(status.substr(0, len));

    while (len < screen_cols) {
        if (screen_cols - len == static_cast<int>(cursor_pos.length())) {
            m_write_buffer.append(cursor_pos);
            break;
        } else {
            m_write_buffer.append(" ");
            len++;
        }
    }

    m_write_buffer.append("\x1b[m");
}

void Renderer::refresh() {
    if (!m_write_buffer.empty()) {
        write(STDOUT_FILENO, m_write_buffer.data(), m_write_buffer.size());
        m_write_buffer.clear();
    }
}

} // namespace minivim
