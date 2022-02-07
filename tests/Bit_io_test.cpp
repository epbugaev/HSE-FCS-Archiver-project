#include "catch.hpp"
#include "Bit_io.h"

const size_t BYTE_SIZE = 8;

TEST_CASE("BitSequenceWriteReadTest", "[Bit_io]") {
    std::string file_name = "test";

    std::ofstream of(file_name, std::ios::binary);
    BitWriter writer(of);

    std::vector<bool> to_test;
    for (size_t i = 0; i < 10 * BYTE_SIZE; ++i) {
        to_test.push_back(i % 2 == 0);
    }
    writer.WriteBits(to_test);
    writer.Close();

    bool is_correct = true;

    std::ifstream in(file_name, std::ios::binary);
    BitReader reader(in);

    std::vector<bool> returned_value = reader.ReadBits(to_test.size());
    reader.ReadBits(BYTE_SIZE);
    is_correct &= reader.HasFinished();
	reader.Close();
    std::remove(file_name.c_str());

    is_correct &= (returned_value == to_test);
   
    REQUIRE(is_correct == true);
}

TEST_CASE("ReaderFinishTest", "Bit_io") {
	std::string file_name = "test";

    std::ofstream of(file_name, std::ios::binary);
    BitWriter writer(of);

    for (size_t i = 0; i < BYTE_SIZE; ++i) {
       	writer.WriteBit(i % 2 == 0);
    }
    writer.Close();

    bool is_correct = true;

    std::ifstream in(file_name, std::ios::binary);
    BitReader reader(in);

    reader.ReadBits(BYTE_SIZE);
    reader.ReadBits(BYTE_SIZE);
    is_correct = reader.HasFinished();
	reader.Close();
    std::remove(file_name.c_str());
       
    REQUIRE(is_correct == true);
}