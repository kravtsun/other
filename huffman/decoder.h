#ifndef HUFFMAN_DECODER_H
#define HUFFMAN_DECODER_H

#include <cassert>
#include "common.h"
#include "huffman_exception.h"

struct Decoder {
    explicit Decoder(istream_t &is);
    
    char_t read_character();
    
    int64_t read_int();
    
    std::vector<bool> read_codes(size_t size);
    
    void read_header();
    
    string_t read_all();
    
    encoding_table_t::const_iterator find_symbol_with_codes(const std::vector<bool> &codes) const;
    
private:
    istream_t &is_;
    encoding_table_t encoding_table;
};


#endif //HUFFMAN_DECODER_H
