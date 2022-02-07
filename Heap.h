#include <vector>
#include "HuffmanTree.h"

using WeightedVertex = std::pair<size_t, Huffman::HuffmanTree*>;

class Heap {
public:
    void Insert(WeightedVertex v);
    WeightedVertex Extract();
    size_t Size();
private:
    void SiftDown(size_t index);
    void SiftUp(size_t index);
    size_t GetLeftChildId(size_t) const;
    size_t GetRightChildId(size_t) const;
    size_t GetParentId(size_t) const;

public:
    std::vector<WeightedVertex> heap_;
};