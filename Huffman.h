#include <fstream>
#include <vector>
#include <bitset>
#include <unordered_map>
#include "Heap.h"

namespace Huffman {
    class Encoder {
    public:
        explicit Encoder(std::ofstream& out);
        ~Encoder();
        void EncodeFile(const std::string& file_name, bool is_last_file);
        void ClearFileInfo();

    private:
        void AddInputTextChar(const Letter c);
        void AddSpecialLetters();
        void CreateHuffmanCodes();
        void ConvertToCannonCodes();
        void WriteArchiveHat(const std::string& file_name);
        void WriteCodedText(BitReader& reader);
        Letter GetLetterFromFile(BitReader& reader) const;
        std::vector<Letter> GetLettersSortedByCodes();
        std::vector<size_t> GetAmountOfCodesForEachLength();

        std::unordered_map<Letter, size_t> frequency_table_;
        CodesMap huffman_codes_;
        BitWriter writer_;
    };

    class Decoder {
    public:
        explicit Decoder(std::ifstream& in);
        ~Decoder();
        void DecodeArchive();

    private:
        void ClearData();
        void AddLettersWithCodes(std::vector<std::pair<size_t, Letter>>& letters_with_lengths);
        Letter GetNextLetter();
        std::vector<std::pair<size_t, Letter>> GetLettersWithLengths(const size_t alphabet_size);
        std::string GetArchivedFileName(Letter& current_letter);

        HuffmanTree* huffman_tree_;
        BitReader reader_;
    };
}