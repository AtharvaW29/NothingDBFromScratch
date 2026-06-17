#pragma once

namespace NothingDB {

    template<typename KeyType, typename ValueType>
    class BPlusTreeLeafNode;

    template<typename KeyType, typename ValueType>
    class BPlusTreeIterator {
    public:

        BPlusTreeIterator(
            BPlusTreeLeafNode<KeyType, ValueType>* leaf,
            size_t index);

        bool operator!=(
            const BPlusTreeIterator& other) const;

        void operator++();

        std::pair<KeyType, ValueType> operator*() const;

    private:

        BPlusTreeLeafNode<KeyType, ValueType>* leaf_;

        size_t index_;
    };

};

#include "index/bplus_tree_iterator.tpp"