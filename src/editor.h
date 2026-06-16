#pragma once
#include "terminal.h"
#include "buffer.h"
#include "renderer.h"
#include <string>

namespace minivim {

constexpr int CTRL_KEY(int k) { return k & 0x1f; }

class Editor {
public:
    explicit Editor(const std::string& filepath = "");
    ~Editor() = default;

    void run();

private:
    void processKey(int key);
    void moveCursor(int key);
    size_t getBufferIndex() const; 
    void updateScroll(); 

    Terminal m_terminal;
    GapBuffer m_buffer;
    Renderer m_renderer;

    bool m_running;
    std::string m_filepath;

    int m_cx, m_cy;
    int m_row_offset, m_col_offset;
    int m_screen_rows, m_screen_cols;
};

} // namespace minivim
