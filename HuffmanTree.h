#include <array>
#include <bitset>
#include <unordered_map>
#include <vector>
#include "Bit_io.h"

namespace Huffman{
        using Letter = std::bitset<9>;
        using Code = std::vector<bool>;
        using CodesMap = std::unordered_map<Letter, Code>;

        Letter ReadLetter(BitReader& reader);
        void WriteLetter(BitWriter& writer, const Huffman::Letter& letter);

        class HuffmanTree {
            public:
            explicit HuffmanTree();
            explicit HuffmanTree(Letter letter_parameter);
            void AddCode(Code code, Letter letter);
            Letter GetLetterFromStream(BitReader& reader);
            static HuffmanTree* Merge(HuffmanTree & left, HuffmanTree & right);
            CodesMap GetCodes() const;
            bool operator<(const HuffmanTree&) const;
            ~HuffmanTree();

            private:
            struct Vertex {
                Letter letter;
                Vertex *right_vertex;
                Vertex *left_vertex;

                explicit Vertex();
                ~Vertex();
            };

            Vertex *root_;
            void GetCodeDfs(Vertex *vertex, Code& current_code, CodesMap& codes_map) const;
        };
};