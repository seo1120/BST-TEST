#include <iostream>
#include <stack>

template <class _Tp> class Node;
template <class _Tp> class BST;

// Node 클래스
template <class _Tp>
class Node {
    public:
        typedef _Tp __key_type;
        typedef Node<__key_type>* __node_pointer_type;

        __key_type __key_;
        __node_pointer_type __left_;
        __node_pointer_type __right_;
        unsigned height_;
        unsigned size_;

        Node() : __key_(__key_type()), __left_(nullptr), __right_(nullptr), height_(1), size_(1) {}
        Node(const __key_type& key) : __key_(key), __left_(nullptr), __right_(nullptr), height_(1), size_(1) {}

        friend std::ostream& operator<<(std::ostream& os, const __node_pointer_type& np) {
            if (!np) return os << "<>";
            os << "<";
            if (np->__left_) os << np->__left_;
            os << " " << np->__key_ << " ";
            if (np->__right_) os << np->__right_;
            os << ">";
            return os;
        }
};

// Helper 함수들
template <class _NodePtr>
unsigned __height(_NodePtr __x) {
    return __x ? __x->height_ : 0;
}

template <class _NodePtr>
unsigned __size(_NodePtr __x) {
    return __x ? __x->size_ : 0;
}

template <class _NodePtr>
_NodePtr __tree_min(_NodePtr __x) {
    while (__x && __x->__left_) {
        __x = __x->__left_;
    }
    return __x;
}

template <class _NodePtr, class _Tp>
std::pair<_NodePtr, bool> __insertBST(_NodePtr& __root, const _Tp& key) {
    _NodePtr p = __root;
    _NodePtr q = nullptr;
    std::stack<_NodePtr> nodeStack;

    while (p) {
        if (key == p->__key_) return std::make_pair(p, false);
        q = p;
        nodeStack.push(q);
        p = (key < p->__key_) ? p->__left_ : p->__right_;
    }

    _NodePtr newNode = new Node<_Tp>(key);
    if (!q)
        __root = newNode;
    else if (key < q->__key_)
        q->__left_ = newNode;
    else
        q->__right_ = newNode;

    while (!nodeStack.empty()) {
        q = nodeStack.top();
        nodeStack.pop();
        q->height_ = 1 + std::max(__height(q->__left_), __height(q->__right_));
        q->size_ = 1 + __size(q->__left_) + __size(q->__right_);
    }
    return std::make_pair(newNode, true);
}

template <class _NodePtr, class _Tp>
bool __eraseBST(_NodePtr& __root, const _Tp& key) {
    _NodePtr p = __root;
    _NodePtr q = nullptr;
    std::stack<_NodePtr> nodeStack;

    while (p && p->__key_ != key) {
        q = p;
        nodeStack.push(q);
        p = (key < p->__key_) ? p->__left_ : p->__right_;
    }

    if (!p) return false; // 삭제할 키가 없는 경우

    // Case 1: 두 개의 자식을 가진 경우, 오른쪽 서브트리의 최소값 노드로 대체
    if (p->__left_ && p->__right_) {
        nodeStack.push(p);
        _NodePtr successor = __tree_min(p->__right_);
        p->__key_ = successor->__key_;
        q = p;
        p = successor;
    }

    // Case 2: 자식이 하나이거나 없는 경우
    _NodePtr child = (p->__left_) ? p->__left_ : p->__right_;
    if (q) {
        if (q->__left_ == p) q->__left_ = child;
        else q->__right_ = child;
    } else {
        __root = child;
    }

    delete p;

    while (!nodeStack.empty()) {
        q = nodeStack.top();
        nodeStack.pop();
        q->height_ = 1 + std::max(__height(q->__left_), __height(q->__right_));
        q->size_ = 1 + __size(q->__left_) + __size(q->__right_);
    }
    return true;
}

template <class _NodePtr>
void __clear(_NodePtr& __x) {
    if (__x) {
        __clear(__x->__left_);
        __clear(__x->__right_);
        delete __x;
        __x = nullptr;
    }
}

// BST 클래스
template <class _Tp>
class BST {
    public:
        typedef _Tp key_type;
        typedef Node<key_type>* pointer;
        typedef const Node<key_type>* const_pointer;

    private:
        pointer __root_;

    public:
        BST() : __root_(nullptr) {}

        size_t height() const {
            return __height(__root_);
        }

        size_t size() const {
            return __size(__root_);
        }

        std::pair<const_pointer, bool> insert(const key_type& key) {
            return __insertBST(__root_, key);
        }

        bool erase(const key_type& key) {
            return __eraseBST(__root_, key);
        }

        void clear() {
            __clear(__root_);
        }

        bool isEmpty() const {
            return __root_ == nullptr;
        }

        friend std::ostream& operator<<(std::ostream& os, const BST& tree) {
            if (tree.isEmpty()) os << "<>";
            else os << tree.__root_;
            return os;
        }
};

int main() {
    BST<int> tree;
    char command;
    int key;

    while (std::cin >> command >> key) {
        switch (command) {
            case 'i':
                if (!tree.insert(key).second) {
                    std::cerr << "i " << key << ": The key already exists" << std::endl;
                } else {
                    std::cout << tree << std::endl;
                }
                break;
            case 'd':
                if (!tree.erase(key)) {
                    std::cerr << "d " << key << ": The key does not exist" << std::endl;
                } else if (tree.isEmpty()) {
                    std::cout << std::endl; // 마지막 노드 삭제 후 빈 트리일 경우 개행만 출력
                } else {
                    std::cout << tree << std::endl;
                }
                break;
            default:
                std::cerr << "Invalid command: " << command << std::endl;
                return 1;
        }
    }

    tree.clear();
    return 0;
}
