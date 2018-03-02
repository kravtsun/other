#ifndef HUFFMAN_ENCODER_H
#define HUFFMAN_ENCODER_H

#include "common.h"

namespace huffman {

struct Encoder {
    Encoder(ostream_t &os, const char_count_map_t &char_count);
    
    void write_header();
    
    void write_all(const std::vector<string_t> &lines);

private:
    ostream_t &os_;
    encoding_table_t encoding_table_;
    
    char_t current_byte_;
    size_t current_index_;
    
private:
    static encoding_table_t encoding_table_from_char_counts(const char_count_map_t &char_count);
    
    void write_character(const char_t &c);
    
    void write_int(int_t num);
    
    void write_byte();
    
    void flush();
    
    void write_encoded(char_t c);
};

} // namespace huffman.

#endif //HUFFMAN_ENCODER_H

