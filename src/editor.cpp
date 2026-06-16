#include "editor.h"

namespace minivim {

Editor::Editor(const std::string& filepath) 
    : m_running(true), m_filepath(filepath), 
      m_cx(0), m_cy(0), m_row_offset(0), m_col_offset(0) {
    
    auto [rows, cols] = m_terminal.getWindowSize();
    m_screen_rows = rows;
    m_screen_cols = cols;

    if (!m_filepath.empty()) {
        m_buffer.loadFromFile(m_filepath);
    }
}

void Editor::run() {
    while (m_running) {
        updateScroll();

        m_renderer.clearScreen();
        
        // Draw text, leaving the bottom row open for the status bar
        m_renderer.draw(m_buffer, m_screen_rows - 1, m_screen_cols, m_row_offset, m_col_offset);
        
        // Draw status bar
        m_renderer.drawStatusBar(m_filepath, m_cx, m_cy, m_screen_rows, m_screen_cols);
        
        // Restore cursor position for typing
        m_renderer.moveCursor(m_cy - m_row_offset, m_cx - m_col_offset);
        m_renderer.refresh();

        int key = m_terminal.readKey();
        processKey(key);
    }
    
    m_renderer.clearScreen();
    m_renderer.refresh();
}

size_t Editor::getBufferIndex() const {
    size_t index = 0;
    int current_row = 0;

    while (current_row < m_cy && index < m_buffer.getLength()) {
        if (m_buffer.charAt(index) == '\n') current_row++;
        index++;
    }

    int current_col = 0;
    while (current_col < m_cx && index < m_buffer.getLength() && m_buffer.charAt(index) != '\n') {
        current_col++;
        index++;
    }

    return index;
}

void Editor::updateScroll() {
    if (m_cy < m_row_offset) m_row_offset = m_cy;
    if (m_cy >= m_row_offset + m_screen_rows - 1) m_row_offset = m_cy - m_screen_rows + 2;

    if (m_cx < m_col_offset) m_col_offset = m_cx;
    if (m_cx >= m_col_offset + m_screen_cols) m_col_offset = m_cx - m_screen_cols + 1;
}

void Editor::processKey(int key) {
    m_buffer.moveCursor(getBufferIndex());

    if (key == CTRL_KEY('q')) {
        m_running = false;
        return;
    } 
    
    if (key == CTRL_KEY('s')) {
        if (!m_filepath.empty()) m_buffer.saveToFile(m_filepath);
        return;
    }

    if (key == static_cast<int>(Key::ARROW_UP) || 
        key == static_cast<int>(Key::ARROW_DOWN) ||
        key == static_cast<int>(Key::ARROW_LEFT) || 
        key == static_cast<int>(Key::ARROW_RIGHT)) {
        moveCursor(key);
        return;
    }

    if (key == static_cast<int>(Key::BACKSPACE)) {
        if (getBufferIndex() > 0) {
            m_buffer.backspace();
            if (m_cx > 0) m_cx--;
            else if (m_cy > 0) {
                m_cy--;
                m_cx = 0;
                size_t temp_idx = getBufferIndex();
                while (temp_idx < m_buffer.getLength() && m_buffer.charAt(temp_idx) != '\n') {
                    m_cx++;
                    temp_idx++;
                }
            }
        }
        return;
    }

    if (key == static_cast<int>(Key::ENTER)) {
        m_buffer.insert('\n');
        m_cy++;
        m_cx = 0;
        return;
    }

    if (key >= 32 && key < 127) {
        m_buffer.insert(static_cast<char>(key));
        m_cx++;
    }
}

void Editor::moveCursor(int key) {
    if (key == static_cast<int>(Key::ARROW_LEFT)) {
        if (m_cx > 0) m_cx--;
    } else if (key == static_cast<int>(Key::ARROW_RIGHT)) {
        m_cx++; 
    } else if (key == static_cast<int>(Key::ARROW_UP)) {
        if (m_cy > 0) m_cy--;
    } else if (key == static_cast<int>(Key::ARROW_DOWN)) {
        m_cy++;
    }
}

} // namespace minivim
