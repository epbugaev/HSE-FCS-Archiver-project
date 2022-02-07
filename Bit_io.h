#include <fstream>
#include <algorithm>
#include <vector>
#include <bitset>
#include <queue>

void SetBit(size_t& number, const size_t bit_id, const bool value);
bool GetBit(const size_t number, const size_t bit_id);

class BitReader {
public:
    explicit BitReader(std::ifstream& in_);
    ~BitReader();
    bool HasFinished();
    bool ReadBit();
    std::vector<bool> ReadBits(size_t count);
    void Close();
private:
    std::ifstream& in_;
    std::queue<bool> left_behind_bits_;
};

class BitWriter {
public:
    explicit BitWriter(std::ofstream& out_);
    ~BitWriter();
    void WriteBit(bool value);
    void WriteBits(const std::vector<bool>& bits);
    void WriteByteToFile();
    void WriteChar(unsigned char c);
    void Close();
private:
    void UpdateWaitingBits();
    size_t MergeOldestWaitingBits();

    std::ofstream& out_;
    std::queue<bool> left_behind_bits_;
};