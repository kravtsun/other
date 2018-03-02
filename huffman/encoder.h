#ifndef HUFFMAN_ENCODER_H
#define HUFFMAN_ENCODER_H

#include "common.h"

struct Encoder {
    Encoder(ostream_t &os, const char_count_map_t &char_count);
    
    void write_header();
    
    void write_all(const std::vector<string_t> &lines);

private:
    ostream_t &os_;
    encoding_table_t encoding_table_;
    
    char_t current_byte;
    size_t current_index;
    
    int cnt = 0;

public:
    static encoding_table_t encoding_table_from_char_counts(const char_count_map_t &char_count);
    
    void write_character(const char_t &c);
    
    // TODO rename int64_t to int_t.
    void write_int(int64_t num);
    
    void write_byte();
    
    void flush();
    
    void write_encoded(char_t c);
};

#endif //HUFFMAN_ENCODER_H
