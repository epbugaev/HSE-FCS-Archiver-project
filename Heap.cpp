#include "Heap.h"
#include <tuple>

void Heap::Insert(WeightedVertex vertex) {
    heap_.push_back(vertex);
    SiftUp(heap_.size() - 1);
}

WeightedVertex Heap::Extract() {
    WeightedVertex return_value = heap_.front();
    heap_.front() = heap_.back();
    heap_.pop_back();
    if (!heap_.empty()) {
        SiftDown(0);
    }
    return return_value;
}

size_t Heap::Size() {
    return heap_.size();
}

bool IsLessWeightedVertex(const WeightedVertex& a, const WeightedVertex& b) {
    return std::tie(a.first, (*a.second)) < std::tie(b.first, (*b.second));
}

void Heap::SiftDown(size_t index) {
    while (GetLeftChildId(index) < heap_.size()) {
        size_t swap_candidate_index = GetLeftChildId(index);
        if (GetRightChildId(index) < heap_.size() &&
            IsLessWeightedVertex(heap_[GetRightChildId(index)], heap_[swap_candidate_index])) {
            swap_candidate_index = GetRightChildId(index);
        }

        if (IsLessWeightedVertex(heap_[index], heap_[swap_candidate_index])) {
            break;
        }
        std::swap(heap_[index], heap_[swap_candidate_index]);
        index = swap_candidate_index;
    }
}

void Heap::SiftUp(size_t index) {
    while (index > 0 && IsLessWeightedVertex(heap_[index], heap_[GetParentId(index)])) {
        std::swap(heap_[index], heap_[GetParentId(index)]);
        index = GetParentId(index);
    }
}

size_t Heap::GetLeftChildId(size_t index) const {
    return index * 2 + 1;
}

size_t Heap::GetRightChildId(size_t index) const {
    return index * 2 + 2;
}

size_t Heap::GetParentId(size_t index) const {
    if (index == 0) {
        return 0;
    }
    return (index - 1) / 2;
}