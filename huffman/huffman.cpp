#include <bits/stdc++.h>
#include "huffman.h"
#include "encoder.h"
#include "decoder.h"

void huffman::encode(istream_t &is, ostream_t &os) {
    char_count_map_t char_count;
    
    std::vector<string_t> lines;
    
    while (!is.eof()) {
        string_t line;
        std::getline(is, line);
        if (!is.eof()) {
            line += endl;
        }
        for (auto const & c: line) {
            if (char_count[c] == std::numeric_limits<char_count_map_t::mapped_type>::max()) {
                throw HuffmanException("Integer overflow");
            }
            ++char_count[c];
        }
        lines.push_back(line);
    }
    
    Encoder encoder{os, char_count};
    encoder.write_header();
    encoder.write_all(lines);
}

void huffman::decode(istream_t &is, ostream_t &os) {
    Decoder decoder{is};
    
    decoder.read_header();
    
    auto output = decoder.read_all();
    os << output << std::flush;
}
