#include "Huffman_supplement.h"
#include <map>
#include <algorithm>
#include <stdexcept>

Huffman::Letter GetLetterByChar(unsigned char c) {
    auto c_number = static_cast<size_t>(c);
    Huffman::Letter letter;
    for (size_t i = 0; i < 8; ++i) {
        letter[i] = GetBit(c_number, i);
    }
    return letter;
}

Huffman::Encoder::Encoder(std::ofstream& out) : writer_(out) {
}

Huffman::Encoder::~Encoder() {
    writer_.Close();
}

void Huffman::Encoder::EncodeFile(const std::string& file_name, bool is_last_file) {
    AddSpecialLetters();

    std::ifstream in(file_name, std::ios::binary);
    if (!in.good()) {
        throw(std::invalid_argument("File " + file_name + " not found"));
    }

    BitReader reader(in);
    for (const signed char c : file_name) {
        AddInputTextChar(GetLetterByChar(static_cast<unsigned char>(c)));
    }

    while (!reader.HasFinished()) {
        Letter x = GetLetterFromFile(reader);
        if (reader.HasFinished()) {
            continue;
        }
        AddInputTextChar(x);
    }
    reader.Close();
    CreateHuffmanCodes();
    ConvertToCannonCodes();
    WriteArchiveHat(file_name);

    in = std::ifstream(file_name, std::ios::binary);
    BitReader reader_for_coding(in);

    WriteCodedText(reader_for_coding);
    reader_for_coding.Close();

    if (!is_last_file) {
        writer_.WriteBits(huffman_codes_[ONE_MORE_FILE]);
    } else {
        writer_.WriteBits(huffman_codes_[ARCHIVE_END]);
    }
}

void Huffman::Encoder::AddSpecialLetters() {
    frequency_table_[FILENAME_END] = 1;
    frequency_table_[ONE_MORE_FILE] = 1;
    frequency_table_[ARCHIVE_END] = 1;
}

void Huffman::Encoder::ClearFileInfo() {
    huffman_codes_.clear();
    frequency_table_.clear();
}

void Huffman::Encoder::AddInputTextChar(const Huffman::Letter c) {
    frequency_table_[c] += 1;
}

void Huffman::Encoder::CreateHuffmanCodes() {
    Heap heap;
    for (const auto kvp : frequency_table_) {
        Huffman::HuffmanTree *new_tree = new Huffman::HuffmanTree(kvp.first);
        heap.Insert({kvp.second, new_tree});
    }

    while (heap.Size() > 1) {
        WeightedVertex first_element = heap.Extract();
        WeightedVertex second_element = heap.Extract();

        HuffmanTree* new_huffman_tree = HuffmanTree::Merge(*first_element.second, *second_element.second);
        delete first_element.second;
        delete second_element.second;
        WeightedVertex merged_element = {first_element.first + second_element.first,
                                         new_huffman_tree};
        heap.Insert(merged_element);
    }

    HuffmanTree* tree = heap.Extract().second;
    CodesMap codes_map = tree->GetCodes();
    huffman_codes_ = codes_map;
    delete tree;
}

void Huffman::Encoder::ConvertToCannonCodes() {
    std::vector<std::pair<size_t, Letter>> letters_with_lengths;
    for (const auto& kvp : huffman_codes_) {
        letters_with_lengths.push_back({kvp.second.size(), kvp.first});
    }
    huffman_codes_ = CannonCodesFromLetterLengthPairs(letters_with_lengths);
}

bool CompareCodeLetterPairs(const std::pair<Huffman::Code, Huffman::Letter>& a, const std::pair<Huffman::Code, Huffman::Letter>& b) {
    if (a.first.size() != b.first.size()) {
        return a.first.size() < b.first.size();
    }
    if (a.first != b.first) {
        return a.first < b.first;
    }
    return a.second.to_ullong() < b.second.to_ullong();
}

void Huffman::Encoder::WriteArchiveHat(const std::string& file_name) {
    WriteLetter(writer_, Letter(huffman_codes_.size()));
    std::vector<Letter> sorted_letters = GetLettersSortedByCodes();
    for (const auto& x : sorted_letters) {
        WriteLetter(writer_, x);
    }

    std::vector<size_t> codes_for_lengths = GetAmountOfCodesForEachLength();
    for (size_t j = 1; j < codes_for_lengths.size(); ++j) {
        WriteLetter(writer_, Letter(codes_for_lengths[j]));
    }

    for (const auto& c : file_name) {
        writer_.WriteBits(huffman_codes_[GetLetterByChar(static_cast<unsigned char>(c))]);
    }
    writer_.WriteBits(huffman_codes_[FILENAME_END]);
}

void Huffman::Encoder::WriteCodedText(BitReader& reader) {
    while (!reader.HasFinished()) {
        Letter letter = GetLetterFromFile(reader);
        if (reader.HasFinished()) {
            continue;
        }
        auto iter = huffman_codes_.find(letter);
        if (iter != huffman_codes_.end()) {
            writer_.WriteBits(huffman_codes_.at(letter));
        } else {
            // Выкинуть исключение
        }
    }
}

Huffman::Letter Huffman::Encoder::GetLetterFromFile(BitReader& reader) const {
    std::vector<bool> temp = reader.ReadBits(8);
    std::reverse(temp.begin(), temp.end());
    Letter x = 0;
    for (int j = 0; j < 8; ++j) {
        x[j] = temp[j];
    }
    return x;
}

std::vector<Huffman::Letter> Huffman::Encoder::GetLettersSortedByCodes() {
    std::vector<std::pair<Code, Letter>> reversed_pairs;
    for (const auto& kvp : huffman_codes_) {
        reversed_pairs.push_back({kvp.second, kvp.first});
    }
    std::sort(reversed_pairs.begin(), reversed_pairs.end(), CompareCodeLetterPairs);
    std::vector<Letter> sorted_letters;
    for (const auto& pair : reversed_pairs) {
        sorted_letters.push_back(pair.second);
    }
    return sorted_letters;
}

std::vector<size_t> Huffman::Encoder::GetAmountOfCodesForEachLength() {
    std::vector<size_t> codes_for_length;
    for (const auto& kvp : huffman_codes_) {
        while (codes_for_length.size() <= kvp.second.size()) {
            codes_for_length.push_back(0);
        }
        ++codes_for_length[kvp.second.size()];
    }
    return codes_for_length;
}