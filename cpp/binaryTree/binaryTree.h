#ifndef ZHBINARYTREE_BINARYTREE_HEADER
#define ZHBINARYTREE_BINARYTREE_HEADER
#include <cassert>
#include <functional>

template <typename K, typename V>
struct BinaryTreeNode
{
    BinaryTreeNode* parent;
    BinaryTreeNode* left;
    BinaryTreeNode* right;
    K               key;
    V               value;
    BinaryTreeNode() {
        parent = nullptr;
        left = nullptr;
        right = nullptr;
    }
    BinaryTreeNode(const K& _key, const V& _v):
        key(_key), value(_v)
    {
        parent = nullptr;
        left = nullptr;
        right = nullptr;
    } 
    void setLeft(BinaryTreeNode<K, V> *node) {
        left = node;
        node->parent = this;
    }
    void setRight(BinaryTreeNode<K, V> *node) {
        right = node;
        node->parent = this;
    }
    void removeChild(BinaryTreeNode<K, V> *node) {
        if (node == left) {
            left = nullptr;
            node->parent = nullptr;
        }
        else if (node == right) {
            right = nullptr;
            node->parent = nullptr;
        }
    }
};

namespace {
    template <typename K, typename V>
    void removeNode(BinaryTreeNode<K, V> *node) {
        assert(node != nullptr);
        assert(node->parent != nullptr);
        BinaryTreeNode<K, V> *parent = node->parent;
        if (node->left == nullptr && node->right == nullptr) {
            parent->removeChild(node);
            delete node;
        }
        else if (node->left == nullptr || node->right == nullptr) {
            if (node->left != nullptr) {
                node->value = node->left->value;
                node->key = node->left->key;
                delete node->left;
                node->left = nullptr;
            } else {
                node->value = node->right->value;
                node->key = node->right->key;
                delete node->right;
                node->right = nullptr;
            }
        }
        else {
            node->value = node->left->value;
            node->key = node->left->key;
            removeNode(node->left);
        }
    }
    template <typename K, typename V>
    void enumerateTree(BinaryTreeNode<K, V> *node,
            std::function<void(const K& key, V& value)> func) {
        if (node != nullptr) {
            enumerateTree(node->left, func);
            func(node->key, node->value);
            enumerateTree(node->right, func);
        }
    }
    template <typename K, typename V>
    void clearTree(BinaryTreeNode<K, V> *node) {
        if (node != nullptr) {
            clearTree(node->left);
            clearTree(node->right);
            delete node;
        }
    }
}

template <typename K, typename V>
class BinaryTree
{
    typedef BinaryTree<K, V> _my_type;
    typedef BinaryTreeNode<K, V> _node_type;
public:
    ~BinaryTree() {
        clear();
    }
public:
    void set(const K& key, const V& v);
    V find(const K& key) const;
    bool exist(const K& key) const;
    void remove(const K& key);
    unsigned long size() const;
    void enumerate(std::function<void(const K& key, V& value)> func);
    void clear();

private:
    _node_type* _find(const K& key) const;
private:
   _node_type   *m_root = nullptr; 
   unsigned long m_size = 0;
};

template <typename K, typename V>
void BinaryTree<K, V>::set(const K& key, const V& value)
{
    m_size++;
    if (m_root == nullptr) {
        m_root = new _node_type(key, value);
        return ;
    }
    _node_type *currentNode = m_root;
    do {
        if (key < currentNode->key) {
            if (currentNode->left == nullptr) {
                currentNode->setLeft(new _node_type(key, value));
                break;
            } else {
                currentNode = currentNode->left;
                continue;
            }
        } 
        else if (key == currentNode->key) {
            //just update
            currentNode->value = value;
            break;
        }
        else {
            if (currentNode->right == nullptr) {
                currentNode->setRight(new _node_type(key, value));
                break;
            } else {
                currentNode = currentNode->right;
                continue;
            }
        }
    } while (true);
}

template <typename K, typename V>
bool BinaryTree<K, V>::exist(const K& key) const {
    _node_type *node = _find(key);
    return node != nullptr;
}

template <typename K, typename V>
V BinaryTree<K, V>::find(const K& key) const
{
    _node_type *node = _find(key);
    if (node != nullptr) {
        return node->value;
    } else {
        throw "key not found";
    }
}

template <typename K, typename V>
void ::BinaryTree<K, V>::enumerate(std::function<void(const K& key, V& value)> func) {
    enumerateTree(m_root, func);
}

template <typename K, typename V>
void BinaryTree<K, V>::remove(const K& key) {
    _node_type *node = _find(key);
    if (node != nullptr) {
        m_size--;
        if (node == m_root) {
            m_root = nullptr;
            delete node;
            return ;
        }

        removeNode<K, V>(node);
    }
}

template <typename K, typename V>
unsigned long BinaryTree<K, V>::size() const {
    return m_size;
}

template <typename K, typename V>
void BinaryTree<K, V>::clear() {
    clearTree(m_root);
    m_root = nullptr;
    m_size = 0;
}


template <typename K, typename V>
auto BinaryTree<K, V>::_find(const K& key) const -> _node_type* {
    _node_type *currentNode = m_root;
    while (currentNode != nullptr) {
        if (key == currentNode->key) {
            return currentNode;
        }
        else if (key < currentNode->key) {
            currentNode = currentNode->left;
        }
        else {
            currentNode = currentNode->right;
        }
    }
    return nullptr;
}

#endif
