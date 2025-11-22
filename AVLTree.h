/*
 * AVLTree.h
 */

#ifndef AVLTREE_H
#define AVLTREE_H
#include <optional>
#include <string>
#include <bits/stl_vector.h>

class AVLTree {
    public:
    using KeyType = std::string;
    using ValueType = size_t;

    protected:
    class AVLNode {
        public:
        KeyType key;
        ValueType value;
        size_t height;

        AVLNode* parent;
        AVLNode* left;
        AVLNode* right;

        AVLNode(const KeyType& key, ValueType value);

        // 0, 1 or 2
        size_t numChildren() const;
        // number of hops to deepest leaf node
        size_t getHeight() const;
        // true or false
        bool isLeaf() const;
    };

    public:
    AVLTree();

    AVLTree(const AVLTree& other);

    ~AVLTree();

    AVLNode* search(AVLNode* node, const std::string& key) const;

    size_t size() const;

    size_t getHeight() const;

    size_t &operator[](const std::string& key);

    void operator=(const AVLTree& other);

    friend std::ostream& operator<<(std::ostream& os, const AVLTree& avlTree);

    bool insert(const std::string& key, size_t value);

    bool remove(const std::string& key);

    bool contains(const std::string& key) const;

    std::optional<size_t> get(const std::string& key) const;

    std::vector<std::string> findRange(const std::string& lowKey, const std::string& highKey) const;

    std::vector<std::string> keys() const;

    private:
    AVLNode* root;
    size_t treeSize;

    AVLNode*& insertNode(AVLNode*& current, const std::string& newKey, size_t value);

    void updateHeight(AVLNode*& parentNode);

    int getBalance(AVLNode*& parentNode);

    bool setChild(AVLNode*& parent, const std::string& whichChild, AVLNode*& child);

    bool replaceChild(AVLNode*& parent, AVLNode*& currentChild, AVLNode*& newChild);

    AVLNode* rotateRight(AVLNode*& node);

    AVLNode* rotateLeft(AVLNode*& node);

    AVLNode* rebalanceNode(AVLNode*& node);

    /* Helper methods for remove */
    // this overloaded remove will do the recursion to remove the node
    bool remove(AVLNode*& current, KeyType key);

    // removeNode contains the logic for actually removing a node based on the number of children
    bool removeNode(AVLNode*& current);

    // You will implement this, but it is needed for removeNode()
    void balanceNode(AVLNode*& node);
};

#endif //AVLTREE_H
