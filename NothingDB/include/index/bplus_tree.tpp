#pragma once

#include <algorithm>
#include <queue>

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

        _ASSERT(internal->children_.size() == internal->keys_.size() + 1);

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

    if (parent->keys_.size() >= ORDER)
    {
        SplitInternal(parent);
    }
}

template<typename KeyType, typename ValueType>
void BPlusTree<KeyType, ValueType>::SplitInternal(
    BPlusTreeInternalNode<KeyType, ValueType>*node)
{
    auto* new_internal = new BPlusTreeInternalNode<KeyType, ValueType>();
    size_t mid = node->keys_.size() / 2;

    KeyType promote_key = node->keys_[mid];

    new_internal->keys_.assign(
        node->keys_.begin() + mid + 1,
        node->keys_.end());
    node->keys_.resize(mid);

    new_internal->children_.assign(
        node->children_.begin() + mid + 1,
        node->children_.end());
    node->children_.resize(mid + 1);

    for (auto child : new_internal->children_)
    {
        child->parent_ = new_internal;
    }
    if (node == root_) // case1 root split
    {
        auto* new_root = new BPlusTreeInternalNode<KeyType, ValueType>();
        new_root->keys_.push_back(promote_key);
        new_root->children_.push_back(node);
        new_root->children_.push_back(new_internal);
        node->parent_ = new_root;

        new_internal->parent_ = new_root;
        root_ = new_root;
        return;
    }
    else // case2 normal internal split
    {
        auto* parent = 
        static_cast<BPlusTreeInternalNode<KeyType, ValueType>*
        >(node->parent_);
        auto pos = std::upper_bound(
            parent->keys_.begin(),
            parent->keys_.end(),
            promote_key);
        size_t index = pos - parent->keys_.begin();
        parent->keys_.insert(pos, promote_key);
        parent->children_.insert(
            parent->children_.begin() + index + 1,
            new_internal);
        new_internal->parent_ = parent;
        if(parent->keys_.size() >= ORDER)
        {
            SplitInternal(parent);
        }
    }
}

template<typename KeyType, typename ValueType>
BPlusTreeLeafNode<KeyType, ValueType>*
BPlusTree<KeyType, ValueType>::GetLeftmostLeaf()
{
    auto* current = root_;

    while (!current->is_leaf_)
    {
        auto* internal =
            static_cast<
            BPlusTreeInternalNode<
            KeyType,
            ValueType>*
            >(current);

        current =
            internal->children_[0];
    }

    return static_cast<
        BPlusTreeLeafNode<
        KeyType,
        ValueType>*
    >(current);
}

template<typename KeyType, typename ValueType>
BPlusTreeIterator<KeyType, ValueType>
BPlusTree<KeyType, ValueType>::Begin()
{
    return BPlusTreeIterator<
        KeyType,
        ValueType>(
            GetLeftmostLeaf(),
            0);
}

template<typename KeyType, typename ValueType>
BPlusTreeIterator<KeyType, ValueType>
BPlusTree<KeyType, ValueType>::End()
{
    return BPlusTreeIterator<KeyType, ValueType>(nullptr, 0);
}


template<typename KeyType, typename ValueType>
std::vector<ValueType>
BPlusTree<KeyType, ValueType>::RangeScan(const KeyType& start, 
    const KeyType& end)
{
    std::vector<ValueType> result;

    auto* leaf = FindLeaf(start);
    if(!leaf)
    {
        return result;
    }
    while (leaf)
    {
        for(size_t i = 0; i < leaf->keys_.size(); i++)
        {
            auto key = leaf->keys_[i];
            if (key > end)
            {
                return result;
            }
            if(key >= start)
            {
                result.push_back(leaf->values[i]);
            }
        }
        leaf = leaf->next_;
    }
    return result;
}

template<typename KeyType, typename ValueType>
void BPlusTree<KeyType, ValueType>::PrintTree() {
    if (root_ == nullptr) {
        std::cout << "Tree is empty." << std::endl;
        return;
    }

    // Queue holds pairs of (Node*, Current Level)
    std::queue<std::pair<BPlusTreeNode<KeyType, ValueType>*, int>> q;
    q.push({ root_, 0 });

    int current_level = 0;
    std::cout << "--- B+ TREE LAYOUT ---" << std::endl;
    std::cout << "Level 0 (Root): ";

    while (!q.empty()) {
        auto [node, level] = q.front();
        q.pop();

        // If we transitioned to a new level, print a newline anchor
        if (level > current_level) {
            std::cout << std::endl;
            current_level = level;
            if (node->is_leaf_) {
                std::cout << "Level " << current_level << " (Leaves): ";
            }
            else {
                std::cout << "Level " << current_level << " (Internal): ";
            }
        }

        if (node->is_leaf_) {
            // Process and print leaf node
            auto* leaf = static_cast<BPlusTreeLeafNode<KeyType, ValueType>*>(node);
            std::cout << "[ ";
            for (const auto& k : leaf->keys_) {
                std::cout << k << " ";
            }
            std::cout << "] ";
        }
        else {
            // Process and print internal node, then queue children
            auto* internal = static_cast<BPlusTreeInternalNode<KeyType, ValueType>*>(node);
            std::cout << "[ ";
            for (const auto& k : internal->keys_) {
                std::cout << k << " ";
            }
            std::cout << "] ";

            // Push all child pointers into the queue for the next level
            for (auto* child : internal->children_) {
                if (child != nullptr) {
                    q.push({ child, level + 1 });
                }
            }
        }
    }
    std::cout << "\n----------------------" << std::endl;
    std::cout << "Root Key Count: " << root_->keys_.size() << std::endl;
    std::cout << "\n----------------------" << std::endl;
}

}