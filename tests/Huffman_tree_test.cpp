#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <vector>
#include "HuffmanTree.h"

TEST_CASE("HuffmanTreeMerge", "[HuffmanTree]") {
    Huffman::Letter letter_left(Huffman::Letter(1));
    Huffman::Letter letter_right(Huffman::Letter(2));

    Huffman::HuffmanTree tree_left(letter_left);
    Huffman::HuffmanTree tree_right(letter_right);

    Huffman::HuffmanTree* merged_tree = Huffman::HuffmanTree::Merge(tree_left, tree_right);

    bool is_correct = true;
    Huffman::CodesMap codes_map = merged_tree->GetCodes();

    is_correct &= (codes_map.size() == 2);
    is_correct &= (codes_map.find(Huffman::Letter(1)) != codes_map.end());
    is_correct &= (codes_map.find(Huffman::Letter(2)) != codes_map.end());

    Huffman::Code letter_left_correct_code = {false};
    Huffman::Code letter_right_correct_code = {true};
    is_correct &= (codes_map[letter_left] == letter_left_correct_code);
    is_correct &= (codes_map[letter_right] == letter_right_correct_code);

    REQUIRE(is_correct == true);
}

TEST_CASE("HuffmanTreeAddCode", "[HuffmanTree]") {
    std::vector<std::pair<Huffman::Letter, Huffman::Code>> letter_code_pairs;
    letter_code_pairs.push_back({Huffman::Letter(0), {false, false}});
    letter_code_pairs.push_back({Huffman::Letter(1), {false, true}});
    letter_code_pairs.push_back({Huffman::Letter(2), {true, false}});
    letter_code_pairs.push_back({Huffman::Letter(3), {true, true}});

    Huffman::HuffmanTree tree;
    for (const auto& [letter, code] : letter_code_pairs) {
        tree.AddCode(code, letter);
    }

    Huffman::CodesMap codes_map = tree.GetCodes();
    bool is_correct = true;
    for (const auto& [letter, code] : letter_code_pairs) {
        is_correct &= (code == codes_map[letter]);
    }

    REQUIRE(is_correct == true);
}