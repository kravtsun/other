#ifndef HUFFMAN_COMMON_H
#define HUFFMAN_COMMON_H

#include <limits>
#include <string>
#include <iosfwd>
#include <fstream>
#include <map>
#include <vector>


// TODO put into namespace.
//using char_t = wchar_t;
using char_t = char;
using uchar_t = unsigned char;

//const size_t CHAR_SIZE = static_cast<size_t>(std::numeric_limits<char_t>::max());
using string_t = std::basic_string<char_t>;
using istream_t = std::basic_istream<char_t>;
using ostream_t = std::basic_ostream<char_t>;
using ifstream_t = std::basic_ifstream<char_t>;
using ofstream_t = std::basic_ofstream<char_t>;

//constexpr size_t CHAR_BYTES_COUNT = sizeof(wchar_t);
constexpr size_t CHAR_BYTES_COUNT = 1;
constexpr size_t CHAR_BITS_COUNT = CHAR_BYTES_COUNT * 8;
constexpr size_t int_size = sizeof(int64_t);

using chars_count_t = std::pair<size_t, string_t>;
using char_count_map_t = std::map<char_t, size_t>;

using encoding_record_t = std::pair<char_t, std::vector<bool>>;
using encoding_table_t = std::map<encoding_record_t::first_type, encoding_record_t::second_type>;

#if defined(unix) || defined(__unix__) || defined(__unix)
const string_t endl = "\n";
#elif __WIN32
const string_t endl{'\n\r'};
#endif

#endif //HUFFMAN_COMMON_H
