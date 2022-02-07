#include "Huffman_supplement.h"

unsigned char GetCharByLetter(Huffman::Letter letter) {
    size_t c_number = 0;
    for (size_t i = 0; i < 8; ++i) {
        SetBit(c_number, i, letter[i]);
    }
    return static_cast<unsigned char>(c_number);
}

size_t GetNumberByLetter(Huffman::Letter letter) {
    return static_cast<size_t>(letter.to_ulong());
}

bool ComparerByLengthAndLetter(const std::pair<size_t, Huffman::Letter>& a, const std::pair<size_t, Huffman::Letter>& b) {
    if (a.first != b.first) {
        return a.first < b.first;
    }
    return a.second.to_ulong() < b.second.to_ullong();
}

void IncrementCodeByOne(Huffman::Code& code) {
    std::reverse(code.begin(), code.end());
    for (size_t i = 0; i < code.size(); ++i) {
        if (code[i] == false) {
            code[i] = true;
            std::reverse(code.begin(), code.end());
            return;
        }
        code[i] = false;
    }
    code.push_back(true);
    std::reverse(code.begin(), code.end());
}

Huffman::CodesMap CannonCodesFromLetterLengthPairs(std::vector<std::pair<size_t, Huffman::Letter>>& letters_with_lengths) {
    std::sort(letters_with_lengths.begin(), letters_with_lengths.end(), ComparerByLengthAndLetter);

    Huffman::CodesMap canon_codes;
    Huffman::Code current_code;
    for (const auto& [length, letter] : letters_with_lengths) {
        while (current_code.size() < length) {
            current_code.push_back(false);
        }

        canon_codes[letter] = current_code;
        IncrementCodeByOne(current_code);
    }
    return canon_codes;
}
