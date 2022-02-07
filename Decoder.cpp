#include "Huffman_supplement.h"

Huffman::Decoder::Decoder(std::ifstream& in) : reader_(in) {
    huffman_tree_ = new HuffmanTree();
}

Huffman::Decoder::~Decoder() {
    delete huffman_tree_;
    reader_.Close();
}

void Huffman::Decoder::DecodeArchive() {
    Letter current_letter = ONE_MORE_FILE;
    while (current_letter != ARCHIVE_END) {
        size_t alphabet_size = GetNumberByLetter(ReadLetter(reader_));
        std::vector<std::pair<size_t, Letter>> letters_with_lengths = GetLettersWithLengths(alphabet_size);

        AddLettersWithCodes(letters_with_lengths);
        current_letter = ONE_MORE_FILE;

        std::string file_name = GetArchivedFileName(current_letter);
        current_letter = GetNextLetter();

        std::ofstream out(file_name, std::ios::binary);
        BitWriter writer(out);
        while (current_letter != ONE_MORE_FILE && current_letter != ARCHIVE_END) {
            writer.WriteChar(GetCharByLetter(current_letter));
            current_letter = GetNextLetter();
        }
        writer.Close();
        ClearData();
    }
}

void Huffman::Decoder::ClearData() {
    delete huffman_tree_;
    huffman_tree_ = new HuffmanTree();
}

void Huffman::Decoder::AddLettersWithCodes(std::vector<std::pair<size_t, Letter>>& letters_with_lengths) {
    CodesMap codes_map = CannonCodesFromLetterLengthPairs(letters_with_lengths);

    for (const auto&[length, letter] : letters_with_lengths) {
        huffman_tree_->AddCode(codes_map[letter], letter);
    }
}

Huffman::Letter Huffman::Decoder::GetNextLetter() {
    return huffman_tree_->GetLetterFromStream(reader_);
}

std::vector<std::pair<size_t, Huffman::Letter>> Huffman::Decoder::GetLettersWithLengths(const size_t alphabet_size) {
    std::vector<std::pair<size_t, Letter>> letters_with_lengths;
    for (size_t i = 0; i < alphabet_size; ++i) {
        letters_with_lengths.push_back({0, ReadLetter(reader_)});
    }

    {
        size_t current_letter_id = 0;
        size_t current_length = 1;
        while (current_letter_id < letters_with_lengths.size()) {
            size_t codes_with_current_length = GetNumberByLetter(ReadLetter(reader_));
            for (size_t i = 0; i < codes_with_current_length; ++i) {
                letters_with_lengths[current_letter_id].first = current_length;
                ++current_letter_id;
            }
            ++current_length;
        }
    }
    return letters_with_lengths;
}

std::string Huffman::Decoder::GetArchivedFileName(Letter& current_letter) {
    std::string file_name;
    current_letter = GetNextLetter();

    while (current_letter != FILENAME_END) {
        file_name += GetCharByLetter(current_letter);
        current_letter = GetNextLetter();
    }
    return file_name;
}