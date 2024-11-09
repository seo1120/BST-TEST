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
    unsigned int height;

    Node() : __key_(__key_type()), __left_(nullptr), __right_(nullptr), height(1) {}
    explicit Node(const __key_type& key) : __key_(key), __left_(nullptr), __right_(nullptr), height(1) {}

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

    unsigned int __height(pointer node) {
        return node ? node->height : 0;
    }

    void __update_height(pointer node) {
        if (node) {
            node->height = 1 + std::max(__height(node->__left_), __height(node->__right_));
        }
    }

    pointer __insert(pointer& root, const key_type& key, bool& success) {
        pointer p = root, q = nullptr;
        std::stack<pointer> stack;

        while (p) {
            if (key == p->__key_) {
                success = false;
                return root; // 이미 존재하는 키
            }
            q = p;
            stack.push(q);
            p = (key < p->__key_) ? p->__left_ : p->__right_;
        }

        pointer newNode = new Node<key_type>(key);
        if (!q) {
            root = newNode; // 트리가 비어 있는 경우
        } else if (key < q->__key_) {
            q->__left_ = newNode;
        } else {
            q->__right_ = newNode;
        }

        success = true;

        while (!stack.empty()) {
            q = stack.top();
            stack.pop();
            __update_height(q);
        }

        return root;
    }

    pointer __delete(pointer& root, const key_type& key, bool& success) {
        pointer p = root, q = nullptr;
        std::stack<pointer> stack;

        while (p && p->__key_ != key) {
            q = p;
            stack.push(q);
            p = (key < p->__key_) ? p->__left_ : p->__right_;
        }

        if (!p) {
            success = false; // 키가 존재하지 않음
            return root;
        }

        if (p->__left_ && p->__right_) {
            stack.push(p);
            pointer temp = p;
            if (__height(p->__left_) < __height(p->__right_)) {
                p = p->__right_;
                while (p->__left_) {
                    stack.push(p);
                    p = p->__left_;
                }
            } else {
                p = p->__left_;
                while (p->__right_) {
                    stack.push(p);
                    p = p->__right_;
                }
            }
            temp->__key_ = p->__key_;
            q = stack.top();
        }

        pointer child = (p->__left_) ? p->__left_ : p->__right_;

        if (!q) {
            root = child; // 삭제할 노드가 루트인 경우
        } else if (q->__left_ == p) {
            q->__left_ = child;
        } else {
            q->__right_ = child;
        }

        delete p;
        success = true;

        while (!stack.empty()) {
            q = stack.top();
            stack.pop();
            __update_height(q);
        }

        return root;
    }

public:
    BST() : __root_(nullptr) {}

    void insert(const key_type& key) {
        bool success;
        __root_ = __insert(__root_, key, success);
        if (!success) {
            std::cerr << "i " << key << ": The key already exists" << std::endl;
        } else {
            std::cout << __root_ << std::endl;
        }
    }

    void erase(const key_type& key) {
        bool success;
        __root_ = __delete(__root_, key, success);
        if (!success) {
            std::cerr << "d " << key << ": The key does not exist" << std::endl;
        } else if (__root_) {
            std::cout << __root_ << std::endl;
        } else {
            std::cout << "<>" << std::endl;
        }
    }

    void clear() {
        while (__root_) {
            erase(__root_->__key_);
        }
    }
};

int main() {
    BST<int> tree;
    char command;
    int key;

    while (std::cin >> command >> key) {
        if (command == 'i') {
            tree.insert(key);
        } else if (command == 'd') {
            tree.erase(key);
        }
    }

    tree.clear();
    return 0;
}
