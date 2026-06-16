#pragma once
#include <memory>
#include <string>
#include <cstddef>

namespace minivim {

class GapBuffer {
public:
    explicit GapBuffer(size_t initial_capacity = 1024);
    ~GapBuffer() = default;

    void insert(char c);
    void backspace();
    void deleteChar(); 
    void moveCursor(size_t new_pos);

    size_t getCursor() const { return m_gap_start; }
    size_t getLength() const { return m_size - (m_gap_end - m_gap_start); }
    char charAt(size_t index) const;

    void loadFromFile(const std::string& path);
    void saveToFile(const std::string& path) const;

private:
    void growGap();

    std::unique_ptr<char[]> m_buffer;
    size_t m_size;
    size_t m_gap_start;
    size_t m_gap_end;
};

} // namespace minivim
