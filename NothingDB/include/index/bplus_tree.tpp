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
				std::upper_bound(
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
            SplitLeaf(target_leaf);
        }
    }
}

template<typename KeyType, typename ValueType>
bool BPlusTree<KeyType, ValueType>::Search(
    const KeyType& key,
    ValueType& value) {

    auto* leaf = FindLeaf(key);
    auto it = std::lower_bound(
        leaf->keys_.begin(),
        leaf->keys_.end(),
        key);

    if(it == leaf->keys_.end())
        {
           return false;
        }
    if(*it != key)
        {
            return false;
        }
    size_t index = it - leaf->keys_.begin();
    value = leaf->values_[index];

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

    // Updatingf assign patterns because of a memory corrutpion exception
    old_leaf->parent_ = new_root;
    new_leaf->parent_ = new_root;
    new_root->parent_ = nullptr;

    root_ = new_root;
}

template<typename KeyType, typename ValueType>
BPlusTreeLeafNode<KeyType, ValueType>*
BPlusTree<KeyType, ValueType>::FindLeaf(const KeyType& key)
{
    auto* current = root_;
    while(!current->is_leaf_)
    {
        auto* internal = static_cast<
            BPlusTreeInternalNode<KeyType, ValueType>*>(current);

        auto pos = std::upper_bound(
            internal->keys_.begin(),
            internal->keys_.end(),
            key
        );
        size_t index = pos - internal->keys_.begin();
        current = internal->children_[index];
    }
    return static_cast<BPlusTreeLeafNode<KeyType, ValueType>*>(current);
}

template<typename KeyType, typename ValueType>
void BPlusTree<KeyType, ValueType>::SplitLeaf(
    BPlusTreeLeafNode<KeyType, ValueType>* leaf)
{
    auto* new_leaf = new BPlusTreeLeafNode<KeyType, ValueType>();
    size_t mid = leaf->keys_.size() / 2;

    new_leaf->keys_.assign(leaf->keys_.begin() + mid, leaf->keys_.end());
    new_leaf->values_.assign(leaf->values_.begin() + mid, leaf->values_.end());

    leaf->keys_.resize(mid);
    leaf->values_.resize(mid);

    new_leaf->next_ = leaf->next_;
    leaf->next_ = new_leaf;

    auto* parent = 
    static_cast<BPlusTreeInternalNode<KeyType, ValueType>*>(leaf->parent_);

    new_leaf->parent_ = parent;
    KeyType separator = new_leaf->keys_[0];

    auto pos = std::upper_bound(
        parent->keys_.begin(),
        parent->keys_.end(),
        separator);

    size_t index = pos - parent->keys_.begin();

    parent->keys_.insert(pos, separator);
    parent->children_.insert(
        parent->children_.begin() + index+1, new_leaf);

    //if (parent->keys.size() >= ORDER)
    //{
    //    SplitInternal(parent);
    //}
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