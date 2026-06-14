#pragma once

#include <algorithm>

namespace NothingDB {

static constexpr int ORDER = 4;

template<typename KeyType, typename ValueType>
BPlusTree<KeyType, ValueType>::BPlusTree() {

    root_ =
        new BPlusTreeLeafNode<KeyType, ValueType>();
}

template<typename KeyType, typename ValueType>
void BPlusTree<KeyType, ValueType>::Insert(
    const KeyType& key,
    const ValueType& value) {
    if (root_->is_leaf_)
    {
	    auto* leaf = static_cast<BPlusTreeLeafNode<KeyType, ValueType>*>(root_);
        auto pos =
            std::lower_bound(
                leaf->keys_.begin(),
                leaf->keys_.end(),
                key);

        size_t index = pos - leaf->keys_.begin();

        leaf->keys_.insert(pos, key);

        leaf->values_.insert(leaf->values_.begin() + index, value);    

       if (leaf->keys_.size() >= ORDER)
       {
           SplitRootLeaf();
       }
    }
    else
    {
        // TODO: handle insertion when the root is an internal node.
		BPlusTreeNode<KeyType, ValueType>* current = root_;
		while (!current->is_leaf_)
		{
			auto* internal = static_cast<BPlusTreeInternalNode<KeyType, ValueType>*>(current);
			auto pos =
				std::lower_bound(
					internal->keys_.begin(),
					internal->keys_.end(),
					key);
			size_t index = pos - internal->keys_.begin();
			current = internal->children_[index];
		}

		auto* target_leaf = static_cast<BPlusTreeLeafNode<KeyType, ValueType>*>(current);
		auto pos =
			std::lower_bound(
				target_leaf->keys_.begin(),
				target_leaf->keys_.end(),
				key);
		size_t index = pos - target_leaf->keys_.begin();
		target_leaf->keys_.insert(pos, key);
		target_leaf->values_.insert(target_leaf->values_.begin() + index, value);
		if (target_leaf->keys_.size() >= ORDER)
        {
			// TODO: create a more fenerics Split function that can handle 
            // splitting any node, not just the root.
        }
    }
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
void BPlusTree<KeyType, ValueType>::SplitRootLeaf()
{
    auto* old_leaf = static_cast<BPlusTreeLeafNode<KeyType, ValueType>*>(root_);

    auto* new_leaf = new BPlusTreeLeafNode<KeyType, ValueType>();

    size_t mid = old_leaf->keys_.size() / 2;

    new_leaf->keys_.assign(old_leaf->keys_.begin() + mid,
                           old_leaf->keys_.end());

    new_leaf->values_.assign(old_leaf->values_.begin() + mid,
                       old_leaf->values_.end());

    old_leaf->keys_.resize(mid);
    old_leaf->values_.resize(mid);

    new_leaf->next_ = old_leaf->next_;
    old_leaf->next_ = new_leaf;

    auto* new_root = new BPlusTreeInternalNode<KeyType, ValueType>();
    new_root->keys_.push_back(new_leaf->keys_[0]);
    new_root->children_.push_back(old_leaf);
    new_root->children_.push_back(new_leaf);

    root_ = new_root;
}


template<typename KeyType, typename ValueType>
void BPlusTree<KeyType, ValueType>::PrintTree() {

    if(root_->is_leaf_)
    {
        auto* leaf = static_cast<BPlusTreeLeafNode<KeyType, ValueType>*>(root_);
        for (auto k : leaf->keys_) {
            std::cout << k << " ";
        }
        std::cout << std::endl;
        return;
    }

    auto* root = static_cast<BPlusTreeInternalNode <KeyType, ValueType>*>(root_);
    std::cout << "ROOT: ";
    for (auto k : root->keys_){
        std::cout << k << " ";
    }
    std::cout << std::endl;
	
    for (size_t i =0; i < root->children_.size(); i++)
    {
		auto* leaf = 
            static_cast<BPlusTreeLeafNode <KeyType, ValueType>* > (root->children_[i]);
        std::cout << "Leaf" << i << ": ";
		for (auto k : leaf->keys_) {
			std::cout << k << " ";
		}
        std::cout << std::endl;
    }

    }

}