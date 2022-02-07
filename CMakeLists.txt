cmake_minimum_required(VERSION 3.15)
project(archiever)

set(CMAKE_CXX_STANDARD 17)

add_executable(archiever main.cpp Bit_io.cpp Encoder.cpp HuffmanTree.cpp Heap.cpp Decoder.cpp Huffman_supplement.cpp)