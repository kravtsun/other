#include <iostream>
#include <set>
#include <algorithm>
#include <cassert>
#include "encoder.h"

namespace huffman {

Encoder::Encoder(ostream_t &os, const char_count_map_t &char_count)
        : os_(os) {
    encoding_table_ = encoding_table_from_char_counts(char_count);
}

void Encoder::write_character(const char_t &c) {
    os_.put(c);
    current_index_ = 0;
    current_byte_ = 0;
}

void Encoder::write_int(int_t num) {
    const int int_byte_count = INT_BYTES_COUNT / CHAR_BYTES_COUNT;
    for (int i = 0; i < int_byte_count; ++i) {
        auto const mask = (1LL << CHAR_BITS_COUNT) - 1;
        auto const c = static_cast<char_t>(num & mask);
        num >>= CHAR_BITS_COUNT;
        write_character(c);
    }
}

void Encoder::write_byte() {
    assert(current_index_ <= CHAR_BITS_COUNT);
    current_byte_ = static_cast<char_t>(current_byte_ << (CHAR_BITS_COUNT - current_index_));
    write_character(current_byte_);
    current_index_ = 0;
    current_byte_ = 0;
}

void Encoder::flush() {
    if (current_index_ > 0) {
        write_byte();
    }
    os_.flush();
}

void Encoder::write_encoded(char_t c) {
    auto const &codes = encoding_table_.at(c);
    
    for (auto const &bit : codes) {
        current_byte_ = static_cast<char_t>(current_byte_ << 1);
        current_byte_ |= static_cast<char_t>(bit);
        ++current_index_;
        if (current_index_ == CHAR_BITS_COUNT) {
            write_byte();
        }
    }
}

void Encoder::write_header() {
    write_int(static_cast<int_t>(encoding_table_.size()));
    
    flush();
    for (auto const &char_codes : encoding_table_) {
        auto const &c = char_codes.first;
        write_character(c);
        flush();
        auto const &codes = char_codes.second;
        write_int(static_cast<int_t>(codes.size()));
        assert(current_index_ == 0);
        write_encoded(c);
        flush();
    }
}

void Encoder::write_all(const std::vector<string_t> &lines) {
    // first pass - counting how many code bits.
    int_t bits_count = 0;
    for (auto const &line : lines) {
        for (auto const &c : line) {
            bits_count += encoding_table_.at(c).size();
        }
    }
    write_int(bits_count);
    
    // second pass - writing.
    for (auto const &line : lines) {
        for (auto const &c : line) {
            write_encoded(c);
        }
    }
    flush();
}

encoding_table_t Encoder::encoding_table_from_char_counts(const char_count_map_t &char_counts) {
    std::set<chars_count_t> counts_set;
    for (auto const &char_count : char_counts) {
        const char_t character = char_count.first;
        const auto count = char_count.second;
        counts_set.emplace(count, string_t(1, character));
    }
    
    // TODO optimize with encoding tree.
    encoding_table_t encoding_table;
    while (counts_set.size() > 1) {
        auto first = *counts_set.begin();
        counts_set.erase(counts_set.begin());
        auto first_count = first.first;
        auto first_chars = first.second;
        
        auto second = *counts_set.begin();
        counts_set.erase(counts_set.begin());
        auto second_count = second.first;
        auto second_chars = second.second;
        
        auto sum_count = first_count + second_count;
        auto sum_chars = first_chars + second_chars;
        counts_set.emplace(sum_count, sum_chars);
        
        for (auto const &c : first_chars) {
            encoding_table[c].push_back(false);
        }
        
        for (auto const &c : second_chars) {
            encoding_table[c].push_back(true);
        }
    }
    
    for (auto &char_codes: encoding_table) {
        auto &codes = char_codes.second;
        std::reverse(codes.begin(), codes.end());
    }
    return encoding_table;
}

} // namespace huffman.
