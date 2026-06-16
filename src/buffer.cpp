#include "buffer.h"
#include <cstring>
#include <fstream>
#include <stdexcept>

namespace minivim {

GapBuffer::GapBuffer(size_t initial_capacity) 
    : m_size(initial_capacity), m_gap_start(0), m_gap_end(initial_capacity) {
    m_buffer = std::make_unique<char[]>(m_size);
}

void GapBuffer::insert(char c) {
    if (m_gap_start == m_gap_end) growGap();
    m_buffer[m_gap_start++] = c;
}

void GapBuffer::backspace() {
    if (m_gap_start > 0) m_gap_start--;
}

void GapBuffer::deleteChar() {
    if (m_gap_end < m_size) m_gap_end++;
}

void GapBuffer::moveCursor(size_t new_pos) {
    if (new_pos > getLength()) new_pos = getLength();
    if (new_pos == m_gap_start) return;

    if (new_pos < m_gap_start) {
        size_t shift_amount = m_gap_start - new_pos;
        std::memmove(m_buffer.get() + m_gap_end - shift_amount, m_buffer.get() + new_pos, shift_amount);
        m_gap_start -= shift_amount;
        m_gap_end -= shift_amount;
    } else {
        size_t shift_amount = new_pos - m_gap_start;
        std::memmove(m_buffer.get() + m_gap_start, m_buffer.get() + m_gap_end, shift_amount);
        m_gap_start += shift_amount;
        m_gap_end += shift_amount;
    }
}

char GapBuffer::charAt(size_t index) const {
    if (index >= getLength()) throw std::out_of_range("Index out of bounds");
    if (index < m_gap_start) return m_buffer[index];
    else return m_buffer[index + (m_gap_end - m_gap_start)];
}

void GapBuffer::growGap() {
    size_t new_size = m_size * 2;
    auto new_buffer = std::make_unique<char[]>(new_size);
    std::memcpy(new_buffer.get(), m_buffer.get(), m_gap_start);
    
    size_t after_gap_length = m_size - m_gap_end;
    size_t new_gap_end = new_size - after_gap_length;
    
    std::memcpy(new_buffer.get() + new_gap_end, m_buffer.get() + m_gap_end, after_gap_length);

    m_buffer = std::move(new_buffer);
    m_gap_end = new_gap_end;
    m_size = new_size;
}

void GapBuffer::loadFromFile(const std::string& path) {
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file.is_open()) return;

    size_t file_size = file.tellg();
    file.seekg(0, std::ios::beg);

    size_t required_capacity = file_size + 1024;
    m_buffer = std::make_unique<char[]>(required_capacity);
    m_size = required_capacity;

    file.read(m_buffer.get(), file_size);
    m_gap_start = file_size;
    m_gap_end = m_size;
}

void GapBuffer::saveToFile(const std::string& path) const {
    std::ofstream file(path, std::ios::binary | std::ios::trunc);
    if (!file.is_open()) throw std::runtime_error("Could not open file for writing.");

    file.write(m_buffer.get(), m_gap_start);
    size_t after_gap_length = m_size - m_gap_end;
    file.write(m_buffer.get() + m_gap_end, after_gap_length);
}

} // namespace minivim
