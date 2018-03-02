#include <bits/stdc++.h>

#include "common.h"
#include "huffman.h"
#include "huffman_exception.h"

int main(int argc, char **argv) {
    if (argc < 3) {
        std::cout << "USAGE: " << argv[0] << " <input-file> <output-file>";
        return 0;
    }
    const std::string input_filename = argv[1];
    const std::string output_filename = argv[2];
    
    ifstream_t fin;
    fin.open(input_filename, std::ios_base::binary);
    if (!fin.is_open()) {
        throw HuffmanException("Failed to open file for reading: " + input_filename);
    }
    
    ofstream_t fout;
    fout.open(output_filename);
    if (!fout.is_open()) {
        throw HuffmanException("Failed to open file for writing: " + output_filename);
    }
    
    huffman::decode(fin, fout);
    
    return 0;
}
