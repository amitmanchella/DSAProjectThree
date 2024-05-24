#pragma once
#include <sstream>
#include <exception>
#include <memory>
namespace cs251 {

// Custom exception classes
    class duplicate_key : public std::runtime_error {
    public:
        duplicate_key() : std::runtime_error("Duplicate key!") {}
    };

    class nonexistent_key : public std::runtime_error {
    public:
        nonexistent_key() : std::runtime_error("Key does not exist!") {}
    };

    class empty_tree : public std::runtime_error {
    public:
        empty_tree() : std::runtime_error("Tree is empty!") {}
    };

    template<typename K, typename V>
    class splay_tree {
    public:
        struct splay_tree_node {
            // Pointer to the left child
            std::shared_ptr <splay_tree_node> m_left{};
            // Pointer to the right child
            std::shared_ptr <splay_tree_node> m_right{};
            // Weak pointer to the parent
            std::weak_ptr <splay_tree_node> m_parent{};

            // The key of this element
            K m_key{};
            // Pointer to the value of this element
            std::unique_ptr <V> m_value{};
        };

        // Return a pointer to the root of the tree
        std::shared_ptr <splay_tree_node> get_root() const;

        // Default constructor - create an empty splay tree
        splay_tree();

        // Insert the key/value pair into the tree, if the key doesn't already exist
        // Throw duplicate_key if the key already exists
        void insert(const K &key, std::unique_ptr <V> value);

        // Return a const reference to the value associated with the given key
        // Throw nonexistent_key if the key is not in the splay tree
        const std::unique_ptr <V> &peek(const K &key);

        // Remove and return the key-value pair associated with the given key
        // Throw nonexistent_key if the key is not in the splay tree
        std::unique_ptr <V> extract(const K &key);

        // Return the minimum key in the splay tree, and splay the node
        // Throw empty_tree if the tree is empty
        K minimum_key();

        // Return the maximum key in the splay tree, and splay the node
        // Throw empty_tree if the tree is empty
        K maximum_key();

        // Return the current number of elements in the splay tree
        bool empty() const;

        // Return whether the splay tree is currently empty
        size_t size() const;

        size_t splay_tree_size = 0;

    private:
        // Pointer to the root node of the splay tree
        std::shared_ptr <splay_tree_node> m_root{};

        void splay(std::shared_ptr <splay_tree_node> node);

        void rotateRight(std::shared_ptr <splay_tree_node> nodeToRotate);

        void rotateLeft(std::shared_ptr <splay_tree_node> nodeToRotate);

        // TODO: Add any additional methods or variables here
    };

    template<typename K, typename V>
    std::shared_ptr<typename splay_tree<K, V>::splay_tree_node> splay_tree<K, V>::get_root() const {
        return m_root;
    }

    template<typename K, typename V>
    splay_tree<K, V>::splay_tree() {
        return;
    }

    template<typename K, typename V>
    void splay_tree<K, V>::splay(std::shared_ptr <splay_tree_node> node) {
        while (node->m_parent.lock()) {
            std::shared_ptr <splay_tree_node> parent = node->m_parent.lock();
            std::shared_ptr <splay_tree_node> grandparent = parent->m_parent.lock();
            if (grandparent == nullptr) {
                if (node == parent->m_left) {
                    rotateRight(parent);
                } else if (node == parent->m_right) {
                    rotateLeft(parent);
                }
            } else if (node == parent->m_left) {
                if (parent == grandparent->m_left) {
                    rotateRight(grandparent);
                    rotateRight(parent);
                } else if (parent == grandparent->m_right) {
                    rotateRight(parent);
                    rotateLeft(grandparent);
                }
            } else if (node == parent->m_right) {
                if (parent == grandparent->m_right) {
                    rotateLeft(grandparent);
                    rotateLeft(parent);
                } else if (parent == grandparent->m_left) {
                    rotateLeft(parent);
                    rotateRight(grandparent);
                }
            }
        }
    }

    template<typename K, typename V>
    void splay_tree<K, V>::rotateRight(std::shared_ptr<splay_tree_node> nodeToRotate) {
        std::shared_ptr<splay_tree_node> leftChild = nodeToRotate->m_left;
        if (leftChild == nullptr){
            return;
        }
        if (leftChild != nullptr) {
            nodeToRotate->m_left = leftChild->m_right;
            if (leftChild->m_right) leftChild->m_right->m_parent = nodeToRotate;
            leftChild->m_parent = nodeToRotate->m_parent;
        }
        if (nodeToRotate->m_parent.lock() == nullptr) { // if nodeToRotate is root
            m_root = leftChild;
        } else if (nodeToRotate == nodeToRotate->m_parent.lock()->m_left) { // if nodeToRotate is left child
            nodeToRotate->m_parent.lock()->m_left = leftChild;
        } else { // if nodeToRotate is right child
            nodeToRotate->m_parent.lock()->m_right = leftChild;
        }
        if (leftChild != nullptr) {
            leftChild->m_right = nodeToRotate;
        }
        nodeToRotate->m_parent = leftChild;
    }

    template<typename K, typename V>
    void splay_tree<K, V>::rotateLeft(std::shared_ptr<splay_tree_node> nodeToRotate) {
        std::shared_ptr<splay_tree_node> rightChild = nodeToRotate->m_right;
        if (!rightChild) return;
        if (rightChild) {
            nodeToRotate->m_right = rightChild->m_left;
            if (rightChild->m_left) rightChild->m_left->m_parent = nodeToRotate;
            rightChild->m_parent = nodeToRotate->m_parent;
        }
        if (!nodeToRotate->m_parent.lock()) {//if root
            m_root = rightChild;
        } else if (nodeToRotate == nodeToRotate->m_parent.lock()->m_left) {
            nodeToRotate->m_parent.lock()->m_left = rightChild;
        } else {
            nodeToRotate->m_parent.lock()->m_right = rightChild;
        }
        if (rightChild) {
            rightChild->m_left = nodeToRotate;
            nodeToRotate->m_parent = rightChild;
        }
    }

    template<typename K, typename V>
    void splay_tree<K, V>::insert(const K &key, std::unique_ptr <V> value) {
        auto temp = std::make_shared<splay_tree_node>();
        temp->m_key = key;
        temp->m_value = std::move(value);
        temp->m_right = nullptr;
        temp->m_left = nullptr;

        if (!m_root) {
            m_root = temp;
            splay_tree_size++;
            return;
        }
        auto updatedRoot = m_root;
        std::shared_ptr <splay_tree_node> parent = nullptr;
        while (true) {
            parent = updatedRoot;
            if (updatedRoot->m_key == key) {
                throw duplicate_key();
            } else if (updatedRoot->m_key < key) {
                if (!updatedRoot->m_right) {
                    updatedRoot->m_right = temp;
                    temp->m_parent = updatedRoot;
                    break;
                }
                updatedRoot = updatedRoot->m_right;
            } else {
                if (!updatedRoot->m_left) {
                    updatedRoot->m_left = temp;
                    temp->m_parent = updatedRoot;
                    break;
                }
                updatedRoot = updatedRoot->m_left;
            }
        }
        splay_tree_size++;
        splay(temp);
    }

    template<typename K, typename V>
    const std::unique_ptr <V> &splay_tree<K, V>::peek(const K &key) {
        auto current = m_root;
        while (current != nullptr) {
            if (key == current->m_key) {
                splay(current);
                return current->m_value;
            } else if (key < current->m_key) {
                current = current->m_left;
            } else {
                current = current->m_right;
            }
        }
        throw nonexistent_key();
    }

    template<typename K, typename V>
    std::unique_ptr <V> splay_tree<K, V>::extract(const K &key) {
        auto node = m_root;
        std::shared_ptr <splay_tree_node> parent;
        while (node != nullptr) {
            if (key < node->m_key) {
                node = node->m_left;
            } else if (key > node->m_key) {
                node = node->m_right;
            } else {
                splay(node);
                break;
            }
        }
        if (node == nullptr) {
            throw nonexistent_key();
        }
        // node to remove is the node
        auto child = node;
        std::unique_ptr <V> extractedValue = std::move(node->m_value);
        // rotating node with no children
        if ((node->m_left == nullptr) && (node->m_right == nullptr)) {
            m_root = nullptr;
        } // one child on rotating node
        else if ((node->m_left == nullptr) || (node->m_right == nullptr)) {
            if (node->m_left != nullptr) {
                child = node->m_left;
            } else {
                child = node->m_right;
            }
            m_root = child;
            m_root->m_parent.reset();
        }
        else { // two children on rotating node
            auto successor = node->m_right;
            while (successor->m_left != nullptr) {
                successor = successor->m_left;
            }
            node->m_key = successor->m_key;
            node->m_value = std::move(successor->m_value);
            auto succParent = successor->m_parent.lock();
            if (successor->m_right != nullptr) {
                successor->m_right->m_parent = succParent;
            }
            if (succParent->m_left == successor) {
                succParent->m_left = successor->m_right;
            } else {
                succParent->m_right = successor->m_right;
            }
        }
        if (parent) {
            splay(parent);
        }
        splay_tree_size--;
        return extractedValue;
    }

    template<typename K, typename V>
    K splay_tree<K, V>::minimum_key() {
        auto temp = m_root;
        if (temp == nullptr) {
            throw empty_tree();
        }
        while (temp->m_left != nullptr) {
            temp = temp->m_left;
        }
        splay(temp);
        return temp->m_key;
    }

    template<typename K, typename V>
    K splay_tree<K, V>::maximum_key() {
        auto temp = m_root;
        if (temp == nullptr) {
            throw empty_tree();
        }
        while (temp->m_right != nullptr) {
            temp = temp->m_right;
        }
        splay(temp);
        return temp->m_key;
    }

    template<typename K, typename V>
    bool splay_tree<K, V>::empty() const {
        if (m_root == nullptr) {
            return true;
        }
        return false;
    }

    template<typename K, typename V>
    size_t splay_tree<K, V>::size() const {
        if (splay_tree_size <= 0) {
            return 0;
        }
        return splay_tree_size;
    }
}
