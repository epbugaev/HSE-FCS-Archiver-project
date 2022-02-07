#include "HuffmanTree.h"
#include <tuple>
#include <queue>
#include <algorithm>

#include <iostream>

Huffman::Letter Huffman::ReadLetter(BitReader& reader) {
    Huffman::Letter letter = 0;
    for (size_t i = 0; i < letter.size(); ++i) {
        letter[i] = reader.ReadBit();
    }
    return letter;
}

void Huffman::WriteLetter(BitWriter& writer, const Huffman::Letter& letter) {
    for (size_t i = 0; i < 9; i++) {
        writer.WriteBit(letter[i]);
    }
};

Huffman::HuffmanTree::Vertex::Vertex() {
    left_vertex = nullptr;
    right_vertex = nullptr;
    letter = 0;
}

Huffman::HuffmanTree::Vertex::~Vertex() {
    delete left_vertex;
    delete right_vertex;
}

Huffman::HuffmanTree::HuffmanTree() {
    root_ = nullptr;
}

Huffman::HuffmanTree::HuffmanTree(Letter letter_parameter) {
    Vertex* vertex = new Vertex();
    vertex->letter = letter_parameter;
    root_ = vertex;
}

Huffman::HuffmanTree::~HuffmanTree() {
    delete root_;
}

void Huffman::HuffmanTree::AddCode(Code code, Letter letter) {
    if (root_ == nullptr) {
        root_ = new Vertex();
    }
    Vertex *current_vertex = root_;

    std::reverse(code.begin(), code.end());
    while (!code.empty()) {
        if (code.back() == 0) {
            if (current_vertex->left_vertex == nullptr) {
                current_vertex->left_vertex = new Vertex();
            }
            current_vertex = current_vertex->left_vertex;
        } else {
            if (current_vertex->right_vertex == nullptr) {
                current_vertex->right_vertex = new Vertex();
            }
            current_vertex = current_vertex->right_vertex;
        }
        code.pop_back();
    }
    current_vertex->letter = letter;
}

Huffman::Letter Huffman::HuffmanTree::GetLetterFromStream(class BitReader& reader) {
    if (root_ == nullptr) {
        root_ = new Vertex();
    }
    Vertex *vertex = root_;
    while (vertex->left_vertex != nullptr || vertex->right_vertex != nullptr) {
        bool bit = reader.ReadBit();
        if (!bit) {
            vertex = vertex->left_vertex;
        } else {
            vertex = vertex->right_vertex;
        }
    }
    return vertex->letter;
}

Huffman::HuffmanTree* Huffman::HuffmanTree::Merge(HuffmanTree& left, HuffmanTree& right) {
    HuffmanTree* new_tree = new HuffmanTree();
    new_tree->root_ = new Vertex();
    new_tree->root_->left_vertex = left.root_;
    left.root_ = nullptr;
    new_tree->root_->right_vertex = right.root_;
    right.root_ = nullptr;
    return new_tree;
}

Huffman::CodesMap Huffman::HuffmanTree::GetCodes() const {
    if (root_ == nullptr) {
        return {};
    }

    std::unordered_map<Letter, Code> codes;
    Code current_code;
    GetCodeDfs(root_, current_code, codes);
    return codes;
}

void Huffman::HuffmanTree::GetCodeDfs(Vertex *vertex, Code& current_code, CodesMap& codes_map) const {
    if (vertex->left_vertex == nullptr && vertex->right_vertex == nullptr) {
        codes_map[vertex->letter] = current_code;
        return;
    }
    if (vertex->left_vertex != nullptr) {
        current_code.push_back(false);
        GetCodeDfs(vertex->left_vertex, current_code, codes_map);
        current_code.pop_back();
    }
    if (vertex->right_vertex != nullptr) {
        current_code.push_back(true);
        GetCodeDfs(vertex->right_vertex, current_code, codes_map);
        current_code.pop_back();
    }
}

bool Huffman::HuffmanTree::operator<(const HuffmanTree& b) const {
    return root_ < b.root_;
}



