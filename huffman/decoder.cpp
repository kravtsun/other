#include "decoder.h"

Decoder::Decoder(istream_t &is)
        : is_(is)
{}

char_t Decoder::read_character() {
    char_t c;
//    is_.get(c);
    is_.read(&c, 1);
    return c;
}

int64_t Decoder::read_int() {
    static_assert(int_size % CHAR_BYTES_COUNT == 0, "sizeof(int) % sizeof(char_t) != 0");
    const int int_byte_count = int_size / CHAR_BYTES_COUNT;
    int64_t num = 0;
    for (int i = 0; i < int_byte_count; ++i) {
        auto const c = static_cast<uchar_t>(read_character());
        num |= static_cast<int64_t>(c) << (i * CHAR_BITS_COUNT);
    }
    return num;
}

std::vector<bool> Decoder::read_codes(size_t size) {
    std::vector<bool> codes;
    while (size > CHAR_BITS_COUNT) {
        auto c = read_character();
        for (int i = CHAR_BITS_COUNT - 1; i >= 0; --i) {
            auto const mask = 1 << i;
            codes.push_back(static_cast<bool>(c & mask));
        }
        size -= CHAR_BITS_COUNT;
    }
    if (size > 0) {
        auto c = read_character();
        for (int i = 0; i < size; ++i) {
            assert(CHAR_BITS_COUNT >= i + 1);
            auto const mask = 1 << (CHAR_BITS_COUNT - i - 1);
            codes.push_back(static_cast<bool>(c & mask));
        }
    }
    return codes;
}

void Decoder::read_header() {
    const int64_t encoding_table_size = read_int();
    for (int i = 0; i < encoding_table_size; ++i) {
        char_t c = read_character();
        const auto codes_size = static_cast<const size_t>(read_int());
        auto codes = read_codes(codes_size);
        encoding_table[c] = codes;
    }
}

string_t Decoder::read_all() {
    if (encoding_table.empty()) {
        throw HuffmanException("Header has not been read.");
    }

    string_t output;
    
    auto bits_count = read_int();
    assert(bits_count >= 0);
    std::vector<bool> codes;
    
    while (bits_count > 0) {
        auto const c = read_character();
        for (int current_index = CHAR_BITS_COUNT - 1; current_index >= 0 && bits_count > 0; --current_index, --bits_count) {
            codes.push_back(static_cast<bool>(c & (1 << current_index)));
            auto it = find_symbol_with_codes(codes);
            if (it != encoding_table.end()) {
                codes.clear();
                output.push_back(it->first);
                // TODO convert \n to \r if __WIN32
            }
        }
    }
    assert(codes.empty());
    
    return output;
    
}

encoding_table_t::const_iterator Decoder::find_symbol_with_codes(const std::vector<bool> &codes) const {
    for (auto it = encoding_table.begin(); it != encoding_table.end(); ++it) {
        if (it->second == codes) {
            return it;
        }
    }
    return encoding_table.end();
}
