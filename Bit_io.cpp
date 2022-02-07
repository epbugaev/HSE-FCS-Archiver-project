#include "Bit_io.h"

void SetBit(size_t& number, const size_t bit_id, const bool value) {
    if (value) {
        number |= static_cast<size_t>(1) << bit_id;
    } else {
        number &= ~(static_cast<size_t>(1) << bit_id);
    }
}

bool GetBit(const size_t number, const size_t index) {
    return (number & (static_cast<size_t>(1) << index)) != 0;
}

BitReader::BitReader(std::ifstream& in)
        : in_(in) {
}

BitReader::~BitReader() {
    Close();
}

bool BitReader::HasFinished() {
    if (!left_behind_bits_.empty()) {
        return false;
    }
    return in_.eof();
}

bool BitReader::ReadBit() {
    if (left_behind_bits_.empty()) {
        if (in_.eof()) {
            throw std::invalid_argument("Invalid input file, reading after end of file");
        }

        unsigned char c = static_cast<unsigned char>(in_.get());
        for (int i = 7; i >= 0; i--) {
            left_behind_bits_.push(GetBit(c, i));
        }
    }

    bool value = left_behind_bits_.front();
    left_behind_bits_.pop();
    return value;
};

std::vector<bool> BitReader::ReadBits(size_t count) {
    std::vector<bool> value;
    for (size_t i = 0; i < count; ++i) {
        value.push_back(ReadBit());
    }
    return value;
}

void BitReader::Close() {
    in_.close();
}

BitWriter::BitWriter(std::ofstream& out)
        : out_(out) {
}

BitWriter::~BitWriter() {
    Close();
}

void BitWriter::WriteBit(bool value) {
    left_behind_bits_.push(value);
    UpdateWaitingBits();
};

void BitWriter::WriteBits(const std::vector<bool>& bits) {
    for (const auto bit : bits) {
        WriteBit(bit);
    }
}

void BitWriter::WriteByteToFile() {
    if (left_behind_bits_.empty()) {
        return;
    }
    size_t c_number = MergeOldestWaitingBits();
    auto c = static_cast<char> (c_number);
    out_.write(&c, 1);
}

void BitWriter::WriteChar(unsigned char c) {
    auto c_casted = static_cast<char>(c);
    out_.write(&c_casted, 1);
}

void BitWriter::UpdateWaitingBits() {
    while (left_behind_bits_.size() >= 8) {
        WriteByteToFile();
    }
}

void BitWriter::Close() {
    WriteByteToFile();
    out_.close();
}

size_t BitWriter::MergeOldestWaitingBits() {
    size_t c_number = 0;

    for (size_t i = 0; i < static_cast<size_t>(8) && !left_behind_bits_.empty(); ++i) {
        SetBit(c_number, 8 - i - 1, left_behind_bits_.front());
        left_behind_bits_.pop();
    }
    return c_number;
}