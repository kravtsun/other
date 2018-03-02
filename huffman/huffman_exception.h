#ifndef HUFFMAN_HUFFMAN_EXCEPTION_H
#define HUFFMAN_HUFFMAN_EXCEPTION_H

#include <stdexcept>

namespace huffman {

class HuffmanException : public std::runtime_error {
public:
    explicit HuffmanException(const std::string &msg);
};

}

#endif //HUFFMAN_HUFFMAN_EXCEPTION_H
