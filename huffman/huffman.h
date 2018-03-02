#ifndef HUFFMAN_HUFFMAN_H
#define HUFFMAN_HUFFMAN_H

#include "common.h"

namespace huffman {
    void encode(istream_t &is, ostream_t &os);

    void decode(istream_t &is, ostream_t &os);
}


#endif //HUFFMAN_HUFFMAN_H
