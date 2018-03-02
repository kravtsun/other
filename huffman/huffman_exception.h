#ifndef HUFFMAN_HUFFMAN_EXCEPTION_H
#define HUFFMAN_HUFFMAN_EXCEPTION_H

#include <stdexcept>

class HuffmanException : public std::runtime_error {
public:
    HuffmanException(const std::string &msg)
            : std::runtime_error(msg)
    {}
};


#endif //HUFFMAN_HUFFMAN_EXCEPTION_H
