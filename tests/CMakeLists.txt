cmake_minimum_required(VERSION 3.15)
project(archiver)

set(CMAKE_CXX_STANDARD 17)

add_executable(archiever Bit_io.cpp Encoder.cpp HuffmanTree.cpp Heap.cpp Decoder.cpp Huffman_supplement.cpp Huffman_tree_test.cpp Bit_io_test.cpp Huffman_test.cpp)