#include "huffman_exception.h"

namespace huffman {

HuffmanException::HuffmanException(const std::string &msg)
        : std::runtime_error(msg) {}
    
} // namespace huffman.
