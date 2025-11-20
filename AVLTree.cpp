#include "AVLTree.h"
#include <optional>
#include <string>
#include <vector>
using namespace std;

/* AVLNode */
AVLTree::AVLNode::AVLNode(const KeyType& key, ValueType value) {
    this->key = key;
    this->value = value;
    height = 0;
    left = nullptr;
    right = nullptr;
}

size_t AVLTree::AVLNode::numChildren() const {
    size_t nChildren = 0;

    if (left) nChildren++;
    if (right) nChildren++;

    return nChildren;
}

size_t AVLTree::AVLNode::getHeight() const {
    return height;
}

bool AVLTree::AVLNode::isLeaf() const {
    return height == 0;
}

/* AVLTree */
AVLTree::AVLTree() {
    root = nullptr;
    treeSize = 0;
}

AVLTree::AVLTree(const AVLTree& other) {

}

AVLTree::~AVLTree() {

}

AVLTree::AVLNode* AVLTree::search(AVLNode* node, const std::string& searchKey) {
    if (node == nullptr) {
        return nullptr;
    }

    if (searchKey == node->key) {
        return node;
    }

    if (searchKey < node->key) {
        return search(node->left, searchKey);
    }

    if (searchKey > node->key) {
        return search(node->right, searchKey);
    }

    return nullptr;
}

size_t AVLTree::size() const {
    return treeSize;
}

size_t AVLTree::getHeight() const {
    return root->height;
}

size_t& AVLTree::operator[](const std::string& key) {

}

void AVLTree::operator=(const AVLTree& other) {

}

ostream& operator<<(ostream& os, const AVLTree& avlTree) {

}

bool AVLTree::insert(const std::string& key, size_t value) {

}

bool AVLTree::remove(const std::string& key) {

}

bool AVLTree::remove(AVLNode*& current, KeyType key) {
    return false;
}

bool AVLTree::removeNode(AVLNode*& current){
    if (!current) {
        return false;
    }

    AVLNode* toDelete = current;
    auto nChildren = current->numChildren();

    // case 1 we can delete the node
    if (current->isLeaf()) {
        current = nullptr;
    // case 2 - replace current with its only child
    } else if (current->numChildren() == 1) {
        if (current->right) {
            current = current->right;
        } else {
            current = current->left;
        }
    // case 3 - we have two children,
    // get the smallest key in right subtree by
    // getting right child and go left until left is null
    } else {
        AVLNode* smallestInRight = current->right;
        // I could check if smallestInRight is null,
        // but it shouldn't be since the node has two children
        while (smallestInRight->left) {
            smallestInRight = smallestInRight->left;
        }
        std::string newKey = smallestInRight->key;
        int newValue = smallestInRight->value;
        remove(root, smallestInRight->key); // delete this one

        current->key = newKey;
        current->value = newValue;
        current->height = current->getHeight();
        balanceNode(current);

        return true; // we already deleted the one we needed to so return
    }
    delete toDelete;

    return true;
}

void AVLTree::balanceNode(AVLNode*& node) {

}

bool AVLTree::contains(const std::string& key) const {

}

optional<size_t> AVLTree::get(const std::string& key) const {

}

vector<std::string> AVLTree::findRange(const std::string& lowKey, const std::string& highKey) const {

}

vector<std::string> AVLTree::keys() const {

}