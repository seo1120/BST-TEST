#include <iostream>
#include <stack>

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
    int height_;

    Node() : __key_(__key_type()), __left_(nullptr), __right_(nullptr), height_(1) {}
    Node(const __key_type& key) : __key_(key), __left_(nullptr), __right_(nullptr), height_(1) {}

    friend std::ostream& operator<<(std::ostream& os, const __node_pointer_type& np) {
        if (np) {
            os << "<";
            if (np->__left_) os << np->__left_;
            os << " " << np->__key_ << " ";
            if (np->__right_) os << np->__right_;
            os << ">";
        } else {
            os << "<>";
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

    pointer getBSTNode(const key_type& key) {
        return new Node<key_type>(key);
    }

    void updateHeight(pointer node) {
        if (node) {
            int leftHeight = node->__left_ ? node->__left_->height_ : 0;
            int rightHeight = node->__right_ ? node->__right_->height_ : 0;
            node->height_ = 1 + std::max(leftHeight, rightHeight);
        }
    }

public:
    BST() : __root_(nullptr) {}

    bool insertBST(key_type newKey) {
        pointer p = __root_;
        pointer q = nullptr;
        std::stack<pointer> nodeStack;

        while (p != nullptr) {
            if (newKey == p->__key_) {
                return false; // newKey already exists
            }

            q = p;
            nodeStack.push(q);

            if (newKey < p->__key_) {
                p = p->__left_;
            } else {
                p = p->__right_;
            }
        }

        // Create new node
        pointer newNode = getBSTNode(newKey);

        if (q == nullptr) {
            __root_ = newNode; // Tree was empty, set root
        } else if (newKey < q->__key_) {
            q->__left_ = newNode;
        } else {
            q->__right_ = newNode;
        }

        // Update height while popping parent node from stack
        while (!nodeStack.empty()) {
            q = nodeStack.top();
            nodeStack.pop();
            updateHeight(q);
        }

        return true;
    }

    bool deleteBST(key_type deleteKey) {
        pointer p = __root_;
        pointer q = nullptr;
        std::stack<pointer> nodeStack;

        while (p != nullptr && deleteKey != p->__key_) {
            q = p;
            nodeStack.push(q);

            if (deleteKey < p->__key_) {
                p = p->__left_;
            } else {
                p = p->__right_;
            }
        }

        if (p == nullptr) {
            return false; // deleteKey was not found
        }

        if (p->__left_ != nullptr && p->__right_ != nullptr) {
            nodeStack.push(p);
            pointer tempNode = p;

            if (p->__left_->height_ <= p->__right_->height_) {
                p = p->__right_;
                while (p->__left_ != nullptr) {
                    nodeStack.push(p);
                    p = p->__left_;
                }
            } else {
                p = p->__left_;
                while (p->__right_ != nullptr) {
                    nodeStack.push(p);
                    p = p->__right_;
                }
            }
            tempNode->__key_ = p->__key_;
            q = nodeStack.top();
        }

        // Now degree of p is 0 or 1
        pointer child = (p->__left_ != nullptr) ? p->__left_ : p->__right_;

        if (q == nullptr) {
            __root_ = child; // Deleting root node
        } else if (q->__left_ == p) {
            q->__left_ = child;
        } else {
            q->__right_ = child;
        }

        delete p;

        // Update height while popping parent nodes from stack
        while (!nodeStack.empty()) {
            q = nodeStack.top();
            nodeStack.pop();
            updateHeight(q);
        }

        return true;
    }

    bool is_empty() const {
        return __root_ == nullptr;
    }

    friend std::ostream& operator<<(std::ostream& os, const BST& tree) {
        os << tree.__root_;
        return os;
    }
};

int main(int argc, char **argv) {
    BST<int> tree;
    char command;
    int key;

    while (std::cin >> command >> key) {
        bool display_tree = true; // Flag to control whether to display tree or not
        switch (command) {
            case 'i':
                if (!tree.insertBST(key)) {
                    std::cerr << "i " << key << ": The key already exists" << std::endl;
                    display_tree = false; // Suppress tree display on duplicate insert
                }
                break;
            case 'd':
                if (!tree.deleteBST(key)) {
                    std::cerr << "d " << key << ": The key does not exist" << std::endl;
                    display_tree = false; // Suppress tree display on non-existing delete
                }
                if (tree.is_empty()) {
                    display_tree = false; // Suppress tree display if tree is now empty
                }
                break;
            default:
                std::cerr << "Invalid command: " << command << std::endl;
                return 1;
        }
        
        // If tree should be displayed, output tree structure; otherwise, just a newline if the tree is empty.
        if (display_tree) {
            std::cout << tree << std::endl;
        } else if (tree.is_empty()) {
            std::cout << std::endl;
        }
    }

    return 0;
}
