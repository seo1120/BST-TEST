#include <iostream>

template <class _Tp> class Node;
template <class _Tp> class BST;

template <class _Tp>
class Node {
public:
    typedef _Tp __key_type;
    typedef Node<__key_type>* __node_pointer_type;

    __key_type __key_;
    __node_pointer_type __left_;
    __node_pointer_type __right_;

    Node() : __key_(__key_type()), __left_(NULL), __right_(NULL) {}
    explicit Node(const __key_type& key) : __key_(key), __left_(NULL), __right_(NULL) {}

    friend std::ostream& operator<<(std::ostream& os, const __node_pointer_type& np) {
        if (np) {
            os << "<";
            if (np->__left_) os << np->__left_;
            os << " " << np->__key_ << " ";
            if (np->__right_) os << np->__right_;
            os << ">";
        }
        return os;
    }
};

template <class _Tp>
class BST {
public:
    typedef _Tp key_type;
    typedef Node<key_type>* pointer;

private:
    pointer __root_;
    bool display_tree;

    int max(int a, int b) const {
        return (a > b) ? a : b;
    }

    unsigned __height(pointer node) const {
        if (!node) return 0;
        return 1 + max(__height(node->__left_), __height(node->__right_));
    }

    pointer __tree_min(pointer node) const {
        while (node && node->__left_) {
            node = node->__left_;
        }
        return node;
    }

    pointer __tree_max(pointer node) const {
        while (node && node->__right_) {
            node = node->__right_;
        }
        return node;
    }

    std::pair<pointer, bool> __insertBST(pointer& root, const key_type& key) {
        if (!root) {
            root = new Node<key_type>(key);
            return std::make_pair(root, true);
        }
        if (key < root->__key_) {
            return __insertBST(root->__left_, key);
        } else if (key > root->__key_) {
            return __insertBST(root->__right_, key);
        }
        return std::make_pair(root, false); // Key already exists
    }

    pointer __eraseBST(pointer& root, const key_type& key, bool& success) {
        if (!root) {
            success = false;
            return NULL;
        }

        if (key < root->__key_) {
            root->__left_ = __eraseBST(root->__left_, key, success);
        } else if (key > root->__key_) {
            root->__right_ = __eraseBST(root->__right_, key, success);
        } else {
            // Node found
            if (!root->__left_) {
                pointer temp = root->__right_;
                delete root;
                root = temp;
            } else if (!root->__right_) {
                pointer temp = root->__left_;
                delete root;
                root = temp;
            } else {
                if (__height(root->__left_) >= __height(root->__right_)) {
                    pointer temp = __tree_max(root->__left_);
                    root->__key_ = temp->__key_;
                    root->__left_ = __eraseBST(root->__left_, temp->__key_, success);
                } else {
                    pointer temp = __tree_min(root->__right_);
                    root->__key_ = temp->__key_;
                    root->__right_ = __eraseBST(root->__right_, temp->__key_, success);
                }
            }
        }
        return root;
    }

    void __clear(pointer& node) {
        if (node) {
            __clear(node->__left_);
            __clear(node->__right_);
            delete node;
            node = NULL;
        }
    }

public:
    BST() : __root_(NULL), display_tree(true) {}

    void insert(const key_type& key) {
        std::pair<pointer, bool> result = __insertBST(__root_, key);
        if (!result.second) {
            std::cerr << "i " << key << ": The key already exists" << std::endl;
        } else {
            std::cout << __root_ << std::endl;
        }
    }

    void erase(const key_type& key) {
        bool success = true;
        __eraseBST(__root_, key, success);
        if (!success) {
            std::cerr << "d " << key << ": The key does not exist" << std::endl;
            display_tree = false;
        } else if (__root_) {
            std::cout << __root_ << std::endl;
        }
        // 빈 트리가 된 경우 아무것도 출력하지 않음
    }

    void clear() {
        __clear(__root_);
    }
};

int main() {
    BST<int> tree;
    char command;
    int key;

    while (std::cin >> command >> key) {
        switch (command) {
            case 'i':
                tree.insert(key);
                break;
            case 'd':
                tree.erase(key);
                break;
            default:
                std::cerr << "Invalid command: " << command << std::endl;
                return 1;
        }
    }

    tree.clear();
    return 0;
}
