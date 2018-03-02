#ifndef HUFFMAN_DECODER_H
#define HUFFMAN_DECODER_H

#include "common.h"

namespace huffman {

struct Decoder {
    explicit Decoder(istream_t &is);
    
    void read_header();
    
    string_t read_all();

private:
    istream_t &is_;
    encoding_table_t encoding_table_;

private:
    char_t read_character();
    
    int_t read_int();
    
    std::vector<bool> read_codes(size_t size);
    
    encoding_table_t::const_iterator find_symbol_with_codes(const std::vector<bool> &codes) const;
};

} // namespace huffman.

#endif //HUFFMAN_DECODER_H
