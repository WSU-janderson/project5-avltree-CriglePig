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

AVLTree::AVLNode* AVLTree::search(AVLNode* node, const std::string& searchKey) const {
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

void AVLTree::updateHeight(AVLNode*& parentNode) {
    int leftHeight = -1;
    if (parentNode->left) {
        leftHeight = static_cast<int>(parentNode->left->height);
    }

    int rightHeight = -1;
    if (parentNode->right) {
        rightHeight = static_cast<int>(parentNode->right->height);
    }
    parentNode->height = max(leftHeight, rightHeight) + 1;
}

int AVLTree::getBalance(AVLNode*& parentNode) {
    int leftHeight = -1;
    if (parentNode->left) {
        leftHeight =  static_cast<int>(parentNode->left->height);
    }

    int rightHeight = -1;
    if (parentNode->right) {
        rightHeight = static_cast<int>(parentNode->right->height);
    }
    return leftHeight - rightHeight;
}

bool AVLTree::setChild(AVLNode*& parent, const string& whichChild, AVLNode*& child) {
    if (whichChild != "left" && whichChild != "right") {
        return false;
    }

    if (whichChild == "left") {
        parent->left = child;
    } else {
        parent->right = child;
    }

    if (child) {
        child->parent = parent;
    }
    updateHeight(parent);
    return true;
}

bool AVLTree::replaceChild(AVLNode*& parent, AVLNode*& currentChild, AVLNode*& newChild) {
    if (parent->left == currentChild) {
        return setChild(parent, "left", newChild);
    } else if (parent->right == currentChild) {
        return setChild(parent, "right", newChild);
    }
    return false;
}

AVLTree::AVLNode* AVLTree::rotateRight(AVLNode*& node) {
    AVLNode* leftRightChild = node->left->right;
    AVLNode* newRoot = node->left;

    if (node->parent) {
        replaceChild(node->parent, node, node->left);
    } else { // node is root
        root = node->left;
        root->parent = nullptr;
    }

    setChild(node->left, "right", node);
    setChild(node, "left", leftRightChild);
    return newRoot;
}

AVLTree::AVLNode* AVLTree::rotateLeft(AVLNode*& node) {
    AVLNode* rightLeftChild = node->right->left;
    AVLNode* newRoot = node->right;

    if (node->parent) {
        replaceChild(node->parent, node, node->right);
    } else { // node is root
        root = newRoot;
        root->parent = nullptr;
    }

    setChild(node->right, "left", node);
    setChild(node, "right", rightLeftChild);
    return newRoot;
}

AVLTree::AVLNode* AVLTree::rebalanceNode(AVLNode*& node) {
    updateHeight(node);
    // Right heavy case
    if (getBalance(node) == -2) {
        // Double rotation case
        if (getBalance(node->right) == 1) {
            rotateRight(node->right);
        }
        return rotateLeft(node);
    // Left heavy case
    } else if (getBalance(node) == 2) {
        // Double rotation case
        if (getBalance(node->left) == -1) {
            rotateLeft(node->left);
        }
        return rotateRight(node);
    }
    return node;
}

AVLTree::AVLNode*& AVLTree::insertNode(AVLNode*& current, const std::string& newKey, size_t value) {
    if (current == nullptr) {
        current = new AVLNode(newKey, value);
        return current;
    }

    if (newKey < current->key) {
        return insertNode(current->left, newKey, value);
    }

    if (newKey > current->key) {
        return insertNode(current->right, newKey, value);
    }
    return current;
}

bool AVLTree::insert(const std::string& key, size_t value) {
    AVLNode*& inserted = insertNode(root, key, value);
    rebalanceNode(inserted);
    return true;
}

bool AVLTree::removeNode(AVLNode*& current) {
    if (!current) {
        return false;
    }

    AVLNode* toDelete = current;
    AVLNode* parent = current->parent;
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
        current->parent = parent;
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
        //remove(root, smallestInRight->key); // delete this one
        removeNode(smallestInRight); // delete this one

        current->key = newKey;
        current->value = newValue;
        updateHeight(current);

        rebalanceNode(current);
        return true; // we already deleted the one we needed to so return
    }

    while (parent) {
        rebalanceNode(parent);
        parent = parent->parent;
    }

    delete toDelete;
    return true;
}

bool AVLTree::remove(const std::string& key) {
    AVLNode* node = search(root, key);
    removeNode(node);
}

bool AVLTree::contains(const std::string& key) const {
    return search(root, key);
}

optional<size_t> AVLTree::get(const std::string& key) const {

}

vector<std::string> AVLTree::findRange(const std::string& lowKey, const std::string& highKey) const {

}

vector<std::string> AVLTree::keys() const {

}