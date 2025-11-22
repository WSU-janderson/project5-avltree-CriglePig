/* Filename: AVLTree.cpp
 * Author: Crystal Daniel
 * Project: Project - AVLTree
 * Due Date: 11/21/2025
 * Program Description:
 *    Implementation of an AVL (self-balancing binary search) tree that maps string keys
 *    to size_t values. Supports insertion, removal, lookup, range queries, copying,
 *    and in-order traversal. All operations maintain AVL balance via rotations and
 *    height updates to guarantee O(log n) search, insert, and delete on average.
 */
#include "AVLTree.h"
#include <iostream>
#include <optional>
#include <string>
#include <vector>
using namespace std;

/* Purpose:
 *    Construct a new AVL node with given key and value
 * Parameters:
 *    key – key for this node (const reference)
 *    value – value stored (copied)
 * Behavior:
 *    Initializes child/parent pointers to nullptr and height to 0 (leaf)
 */
AVLTree::AVLNode::AVLNode(const KeyType& key, const ValueType value) {
    this->key = key;
    this->value = value;
    height = 0;
    parent = nullptr;
    left = nullptr;
    right = nullptr;
}

/* Purpose:
 *    Return number of non-null children (0, 1, or 2)
 * Returns:
 *    size_t count of children
 */
size_t AVLTree::AVLNode::numChildren() const {
    size_t nChildren = 0;
    if (left) nChildren++;
    if (right) nChildren++;
    return nChildren;
}

/* Purpose:
 *    Read-only accessor for node height
 * Returns:
 *    height (size_t)
 */
size_t AVLTree::AVLNode::getHeight() const {
    return height;
}

/* Purpose:
 *    Check if node is a leaf (has no children)
 * Returns:
 *    true if height == 0, false otherwise
 * Note:
 *    Height 0 is used to indicate leaf nodes
 */
bool AVLTree::AVLNode::isLeaf() const {
    return height == 0;
}

/* Purpose:
 *    Default constructor for AVLTree
 * Behavior:
 *    Initializes an empty tree with root equals nullptr and size 0
 */
/* AVLTree */
AVLTree::AVLTree() {
    root = nullptr;
    treeSize = 0;
}

/* Purpose:
 *    Recursively deep-copy a subtree
 * Parameters:
 *    node – pointer to node to copy
 *    parent – parent pointer for the newly created node in copy
 * Returns:
 *    pointer to new subtree root (nullptr if node is nullptr)
 * Behavior:
 *    Produces a deep copy of the provided subtree, preserving heights and parent links
 */
AVLTree::AVLNode* AVLTree::copy(const AVLNode* node, AVLNode* parent) {
    if (!node) {
        return nullptr;
    }
    AVLNode* newNode = new AVLNode(node->key, node->value);
    newNode->parent = parent;
    newNode->height = node->height;
    newNode->left = copy(node->left, newNode);
    newNode->right = copy(node->right, newNode);
    return newNode;
}

/* Purpose:
 *    Copy constructor
 * Parameters:
 *    other – AVLTree to copy
 * Behavior:
 *    Creates a deep copy of other by copying its root subtree and size
 */
AVLTree::AVLTree(const AVLTree& other) {
    root = copy(other.root, nullptr);
    treeSize = other.treeSize;
}

/* Purpose:
 *    Recursively free memory for a subtree
 * Parameters:
 *    node – root of subtree to delete
 * Behavior:
 *    Post-order deletes nodes to avoid leaks
 */
void AVLTree::clear(AVLNode* node) {
    if (!node) {
        return;
    }
    clear(node->left);
    clear(node->right);
    delete node;
}

/* Purpose:
 *    Destructor
 * Behavior:
 *    Frees all nodes and resets root/size
 */
AVLTree::~AVLTree() {
    clear(root);
    root = nullptr;
    treeSize = 0;
}

/* Purpose:
 *    Search for a node with a given key starting at node
 * Parameters:
 *    node – root to begin search
 *    searchKey – key to locate
 * Returns:
 *    pointer to node containing searchKey, or nullptr if not found
 * Behavior:
 *    Standard binary search tree recursive search using string comparison
 */
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

/* Purpose:
 *    Return number of elements stored in the tree
 * Returns:
 *    size_t treeSize
 */
size_t AVLTree::size() const {
    return treeSize;
}

/* Purpose:
 *    Return height of the tree (height of root)
 * Returns:
 *    root height (size_t). Caller must ensure tree is non-empty before relying on root
 */
size_t AVLTree::getHeight() const {
    return root->height;
}

/* Purpose:
 *    Indexing operator to access value by key
 * Parameters:
 *    key – key to find
 * Returns:
 *    reference to stored value (size_t&)
 * Behavior:
 *    Assumes the key exists in the tree. If not found, this will dereference nullptr.
 *    Use contains/get to safely check for presence before calling
 */
size_t& AVLTree::operator[](const std::string& key) {
    AVLNode* node = search(root, key);
    return node->value;
}

/* Purpose:
 *    Assignment operator
 * Parameters:
 *    other – tree to assign from
 * Behavior:
 *    Clears current tree and deep-copies other. Handles self-assignment
 */
void AVLTree::operator=(const AVLTree& other) {
    if (this == &other) return;
    clear(root);
    root = copy(other.root, nullptr);
    treeSize = other.treeSize;
}

/* Purpose:
 *    In-order traversal printing helper
 * Parameters:
 *    os – output stream to write to
 *    node – current node
 * Behavior:
 *    Prints keys in sorted (ascending) order separated by spaces
 */
void AVLTree::printInOrder(ostream& os, const AVLNode* node) const {
    if (!node) return;
    printInOrder(os, node->left);
    os << node->key << " ";
    printInOrder(os, node->right);
}

/* Purpose:
 *    Stream insertion operator to print the tree keys in-order
 * Parameters:
 *    os – output stream
 *    tree – AVLTree to print
 * Returns:
 *    reference to output stream
 */
ostream& operator<<(ostream& os, const AVLTree& tree) {
    tree.printInOrder(os, tree.root);
    return os;
}

/* Purpose:
 *    Update the stored height of parentNode based on child heights
 * Parameters:
 *    parentNode – node whose height will be recalculated
 * Behavior:
 *    Height is max(left.height, right.height) + 1, with missing child treated as -1
 */
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

/* Purpose:
 *    Compute balance factor for parentNode: left.height - right.height
 * Parameters:
 *    parentNode – node to evaluate
 * Returns:
 *    int balance factor; positive means left heavy, negative means right heavy
 */
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

/* Purpose:
 *    Set either the left or right child of parent to child
 * Parameters:
 *    parent – parent node
 *    whichChild – "left" or "right"
 *    child – new child pointer
 * Returns:
 *    true if successful, false if whichChild is invalid
 * Behavior:
 *    Updates the child's parent pointer (if child != nullptr) and recalculates parent's height
 */
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

/* Purpose:
 *    Replace currentChild (one of parent's children) with newChild
 * Parameters:
 *    parent – parent node
 *    currentChild – pointer currently stored in parent
 *    newChild – replacement pointer
 * Returns:
 *    true if replacement occurred, false if currentChild is not a child of parent
 */
bool AVLTree::replaceChild(AVLNode*& parent, AVLNode*& currentChild, AVLNode*& newChild) {
    if (parent->left == currentChild) {
        return setChild(parent, "left", newChild);
    } else if (parent->right == currentChild) {
        return setChild(parent, "right", newChild);
    }
    return false;
}

/* Purpose:
 *    Perform right rotation about node (node must have a left child)
 * Parameters:
 *    node – pivot node to rotate
 * Returns:
 *    newRoot – the node that becomes the root of the rotated subtree
 * Behavior:
 *    Updates parent pointers and root if necessary, and updates heights via setChild calls
 */
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

/* Purpose:
 *    Perform left rotation about node (node must have a right child)
 * Parameters:
 *    node – pivot node to rotate
 * Returns:
 *    newRoot – the node that becomes the root of the rotated subtree
 * Behavior:
 *    Updates parent pointers and root if necessary, and updates heights via setChild calls
 */
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

/* Purpose:
 *    Rebalance a node if it is unbalanced (balance factor +/- 2)
 * Parameters:
 *    node – node to rebalance
 * Returns:
 *    pointer to the subtree root after rebalancing
 * Behavior:
 *    Performs single or double rotations for LL, LR, RR, RL cases as appropriate.
 *    Updates node heights before checking balance
 */
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

/* Purpose:
 *    Recursive helper to insert a new key/value pair into the tree
 * Parameters:
 *    current – reference to current node pointer
 *    parent – parent pointer for a newly created node
 *    newKey – key to insert
 *    value – value to insert
 * Returns:
 *    true if insertion succeeded (new node added), false if key already exists
 * Behavior:
 *    Standard BST insert. After creating a new node, walk up to root rebalancing
 *    ancestors to restore AVL property. Does not replace existing keys
 */
bool AVLTree::insertNode(AVLNode*& current, AVLNode* parent, const std::string& newKey, size_t value) {
    if (current == nullptr) {
        current = new AVLNode(newKey, value);
        current->parent = parent;

        AVLNode* node = current->parent;
        while (node) {
            AVLNode* nodeParent = node->parent;
            rebalanceNode(node);
            node = nodeParent;
        }

        treeSize++;
        return true;
    }

    if (newKey < current->key) {
        return insertNode(current->left, current, newKey, value);
    }

    if (newKey > current->key) {
        return insertNode(current->right,current, newKey, value);
    }
    return false;
}

/* Purpose:
 *    Public insert wrapper
 * Parameters:
 *    key – key to insert
 *    value – value to insert
 * Returns:
 *    true if inserted, false if key already present
 */
bool AVLTree::insert(const std::string& key, size_t value) {
    return insertNode(root,nullptr, key, value);
}

/* Purpose:
 *    Remove the node referenced by current pointer
 * Parameters:
 *    current – reference to pointer to node to remove
 * Returns:
 *    true if a node was removed, false if current was nullptr
 * Behavior:
 *    Handles three cases:
 *      1) current is a leaf – remove it
 *      2) current has one child – replace current with child
 *      3) current has two children – find in-order successor (smallest in right subtree),
 *         copy its key/value into current, then remove the successor node.
 *    After physical removal, walks up to root rebalancing ancestors
 */
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

        AVLNode* succParent = smallestInRight->parent;
        if (succParent->left == smallestInRight) {
            removeNode(succParent->left);
        } else {
            removeNode(succParent->right);
        }

        current->key = newKey;
        current->value = newValue;
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

/* Purpose:
 *    Public remove wrapper that finds node by key and removes it
 * Parameters:
 *    key – key to remove
 * Returns:
 *    true if removed, false if key not found
 * Notes:
 *    Decrements treeSize when removal succeeds
 */
bool AVLTree::remove(const std::string& key) {
    AVLNode* node = search(root, key);
    if (node) {
        const bool result = removeNode(node);
        treeSize--;
        return result;
    }
    return false;
}

/* Purpose:
 *    Check whether tree contains a key
 * Parameters:
 *    key – key to search for
 * Returns:
 *    true if found, false otherwise
 */
bool AVLTree::contains(const std::string& key) const {
    return search(root, key);
}

/* Purpose:
 *    Retrieve value for key safely
 * Parameters:
 *    key – key to look up
 * Returns:
 *    optional<size_t> containing the value if found; nullopt otherwise
 */
optional<size_t> AVLTree::get(const std::string& key) const {
    AVLNode* node = search(root, key);
    if (node) {
        return node->value;
    }
    return nullopt;
}

/* Purpose:
 *    Collect values whose keys lie in [lowKey, highKey] into result (in sorted order)
 * Parameters:
 *    node – current node pointer
 *    lowKey – lower bound
 *    highKey – upper bound
 *    result – vector to append matching values
 * Behavior:
 *    Performs an in-order traversal but prunes branches outside the interval for efficiency
 */
void AVLTree::collectInRange(
    const AVLNode* node,
    const std::string& lowKey,
    const std::string& highKey,
    vector<size_t>& result
) {
    if (!node) return;

    if (node->key > lowKey) {
        collectInRange(node->left, lowKey, highKey, result);
    }

    if (node->key >= lowKey && node->key <= highKey) {
        result.push_back(node->value);
    }

    if (node->key < highKey) {
        collectInRange(node->right, lowKey, highKey, result);
    }
}

/* Purpose:
 *    Public range query returning values whose keys are within [lowKey, highKey]
 * Parameters:
 *    lowKey, highKey – inclusive bounds
 * Returns:
 *    vector of values in ascending key order
 */
vector<size_t> AVLTree::findRange(const std::string& lowKey, const std::string& highKey) const {
    vector<size_t> result;
    collectInRange(root, lowKey, highKey, result);
    return result;
}

/* Purpose:
 *    Collect all keys in the tree (in-order) into result
 * Parameters:
 *    node – current node
 *    result – vector<string> to append keys to
 */
void AVLTree::collectKeys(const AVLNode* node, vector<string>& result) {
    if (!node) return;
    collectKeys(node->left, result);
    result.push_back(node->key);
    collectKeys(node->right, result);
}

/* Purpose:
 *    Return a vector of all keys in sorted (ascending) order
 * Returns:
 *    vector<string> of keys
 */
vector<std::string> AVLTree::keys() const {
    vector<std::string> result;
    collectKeys(root, result);
    return result;
}