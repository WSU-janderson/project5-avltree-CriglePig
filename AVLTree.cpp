#include "AVLTree.h"
#include <string>
using namespace std;

size_t AVLTree::AVLNode::numChildren() const {
    return 0;
}

size_t AVLTree::AVLNode::getHeight() const {
    return 0;
}

bool AVLTree::AVLNode::isLeaf() const {
    return false;
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
