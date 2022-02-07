#include "Huffman.h"

namespace Huffman {
const Letter FILENAME_END = 0b100'000'000;
const Letter ONE_MORE_FILE = 0b100'000'001;
const Letter ARCHIVE_END = 0b100'000'010;
}

unsigned char GetCharByLetter(Huffman::Letter letter);
size_t GetNumberByLetter(Huffman::Letter letter);
Huffman::CodesMap CannonCodesFromLetterLengthPairs(std::vector<std::pair<size_t, Huffman::Letter>>& letters_with_lengths);