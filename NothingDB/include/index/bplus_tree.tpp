#pragma once

#include <algorithm>

namespace NothingDB {

template<typename KeyType, typename ValueType>
BPlusTree<KeyType, ValueType>::BPlusTree() {

    root_ =
        new BPlusTreeLeafNode<KeyType, ValueType>();
}

template<typename KeyType, typename ValueType>
void BPlusTree<KeyType, ValueType>::Insert(
    const KeyType& key,
    const ValueType& value) {

    auto pos =
        std::lower_bound(
            root_->keys_.begin(),
            root_->keys_.end(),
            key);

    size_t index =
        pos - root_->keys_.begin();

    root_->keys_.insert(pos, key);

    root_->values_.insert(
        root_->values_.begin() + index,
        value);
}

template<typename KeyType, typename ValueType>
bool BPlusTree<KeyType, ValueType>::Search(
    const KeyType& key,
    ValueType& value) {

    auto it =
        std::lower_bound(
            root_->keys_.begin(),
            root_->keys_.end(),
            key);

    if (it == root_->keys_.end()) {
        return false;
    }

    if (*it != key) {
        return false;
    }

    size_t index =
        it - root_->keys_.begin();

    value = root_->values_[index];

    return true;
}

template<typename KeyType, typename ValueType>
void BPlusTree<KeyType, ValueType>::PrintKeys() {

    for (auto key : root_->keys_) {
        std::cout << key << " ";
    }

    std::cout << std::endl;
}

}