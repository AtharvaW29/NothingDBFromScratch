#include <iostream>
#include "bplus_tree_iterator.h"


namespace NothingDB {

    template<typename KeyType, typename ValueType>
    BPlusTreeIterator<KeyType, ValueType>::BPlusTreeIterator(
        BPlusTreeLeafNode<KeyType, ValueType>* leaf,
        size_t index)
        : leaf_(leaf),
        index_(index)
    {
    };

    template<typename KeyType, typename ValueType>
    std::pair<KeyType, ValueType>
        BPlusTreeIterator<KeyType, ValueType>::operator*() const
    {
        return {
            leaf_->keys_[index_],
            leaf_->values_[index_]
        };
    }

    template<typename KeyType, typename ValueType>
    bool BPlusTreeIterator<KeyType, ValueType>::operator!=(
        const BPlusTreeIterator& other) const
    {
        return leaf_ != other.leaf_ ||
            index_ != other.index_;
    }

    template<typename KeyType, typename ValueType>
    void BPlusTreeIterator<KeyType, ValueType>::operator++()
    {
        index_++;

        if (leaf_ == nullptr)
        {
            return;
        }

        if (index_ >= leaf_->keys_.size())
        {
            leaf_ = leaf_->next_;
            index_ = 0;
        }
    }

}