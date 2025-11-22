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
        [[nodiscard]] size_t numChildren() const;
        // number of hops to deepest leaf node
        [[nodiscard]] size_t getHeight() const;
        // true or false
        [[nodiscard]] bool isLeaf() const;
    };

    public:
    AVLTree();

    AVLTree(const AVLTree& other);

    ~AVLTree();

    AVLNode* search(AVLNode* node, const std::string& key) const;

    [[nodiscard]] size_t size() const;

    [[nodiscard]] size_t getHeight() const;

    size_t &operator[](const std::string& key);

    void operator=(const AVLTree& other);

    friend std::ostream& operator<<(std::ostream& os, const AVLTree& tree);

    bool insert(const std::string& key, size_t value);

    bool remove(const std::string& key);

    [[nodiscard]] bool contains(const std::string& key) const;

    [[nodiscard]] std::optional<size_t> get(const std::string& key) const;

    static void collectInRange(
        const AVLNode* node,
        const std::string& lowKey,
        const std::string& highKey,
        std::vector<size_t>& result
    );

    [[nodiscard]] std::vector<size_t> findRange(const std::string& lowKey, const std::string& highKey) const;

    [[nodiscard]] std::vector<std::string> keys() const;

    private:
    AVLNode* root;
    size_t treeSize;

    void printInOrder(std::ostream& os, const AVLNode* node) const;

    AVLNode* copy(const AVLNode* node, AVLNode* parent);

    void clear(AVLNode* node);

    bool insertNode(AVLNode*& current, AVLNode* parent, const std::string& newKey, size_t value);

    bool removeNode(AVLNode*& current);

    static void updateHeight(AVLNode*& parentNode);

    static int getBalance(AVLNode*& parentNode);

    static bool setChild(AVLNode*& parent, const std::string& whichChild, AVLNode*& child);

    static bool replaceChild(AVLNode*& parent, AVLNode*& currentChild, AVLNode*& newChild);

    AVLNode* rotateRight(AVLNode*& node);

    AVLNode* rotateLeft(AVLNode*& node);

    AVLNode* rebalanceNode(AVLNode*& node);
};

#endif //AVLTREE_H
