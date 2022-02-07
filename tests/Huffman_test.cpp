#include "catch.hpp"
#include "Huffman.h"

std::vector<unsigned char> GetCharsFromFile(const std::string& file_name) {
    std::vector<unsigned char> file_chars;

    std::ifstream in(file_name, std::ios::binary);
    while (!in.eof()) {
        auto c = static_cast<unsigned char>(in.get());
        if (!in.eof()) {
            file_chars.push_back(c);
        }
    }
    in.close();
    return file_chars;
}

TEST_CASE("EncodeDecodeSingleFile", "[Huffman]") {
    std::string file_name = "test0";
    std::vector<unsigned char> original_file = GetCharsFromFile(file_name);
    std::string archive_name = "archive0";

    std::ofstream out(archive_name, std::ios::binary);
    Huffman::Encoder coder = Huffman::Encoder(out);

    coder.EncodeFile(file_name, true);
    coder.ClearFileInfo();
    out.close();

    std::remove(file_name.c_str());

    std::ifstream in(archive_name, std::ios::binary);
    Huffman::Decoder decoder(in);
    decoder.DecodeArchive();
    in.close();
    
    std::vector<unsigned char> decoded_file = GetCharsFromFile(file_name);

    std::remove(archive_name.c_str());
    REQUIRE(original_file == decoded_file); //original_files == decoded_files);
}

TEST_CASE("EncodeMultipleFiles", "[Huffman]") {
    std::vector<std::string> file_names = {"test1", "test2", "test3"};
    std::vector<std::vector<unsigned char>> original_files;
    for (const auto& string : file_names) {
        original_files.push_back(GetCharsFromFile(string));
    }

    std::string archive_name = "archive1";

    std::ofstream out(archive_name, std::ios::binary);
    Huffman::Encoder coder = Huffman::Encoder(out);

    for (size_t i = 0; i < file_names.size(); ++i) {
        bool is_last_file = (i + 1 == file_names.size());
        coder.EncodeFile(file_names[i], is_last_file);
        coder.ClearFileInfo();
    }
    out.close();

    for (const auto& string : file_names) {
        std::remove(string.c_str());
    }

    std::ifstream in(archive_name, std::ios::binary);
    Huffman::Decoder decoder(in);
    decoder.DecodeArchive();
    in.close();

    std::vector<std::vector<unsigned char>> decoded_files;
    for (const auto& string : file_names) {
        decoded_files.push_back(GetCharsFromFile(string));
    }

    std::remove(archive_name.c_str());
    REQUIRE(original_files == decoded_files);
}

TEST_CASE("IncorrectArchiveTest", "[Huffman]") {
	std::string archive_name = "incorrect_archive";
    std::ifstream in(archive_name, std::ios::binary);

    Huffman::Decoder decoder(in);

    REQUIRE_THROWS(decoder.DecodeArchive());
}