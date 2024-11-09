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

    Node() : __key_(__key_type()), __left_(nullptr), __right_(nullptr) {}
    explicit Node(const __key_type& key) : __key_(key), __left_(nullptr), __right_(nullptr) {}

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

// std::max를 대체하는 사용자 정의 함수
template <class _Tp>
_Tp max(_Tp a, _Tp b) {
    return (a > b) ? a : b;
}

template <class _NodePtr>
unsigned __height(_NodePtr __x) {
    if (!__x) return 0;
    return 1 + max(__height(__x->__left_), __height(__x->__right_));
}

template <class _NodePtr>
unsigned __size(_NodePtr __x) {
    if (!__x) return 0;
    return 1 + __size(__x->__left_) + __size(__x->__right_);
}

template <class _NodePtr>
_NodePtr __tree_min(_NodePtr __x) {
    while (__x && __x->__left_) {
        __x = __x->__left_;
    }
    return __x;
}

template <class _NodePtr>
_NodePtr __tree_max(_NodePtr __x) {
    while (__x && __x->__right_) {
        __x = __x->__right_;
    }
    return __x;
}

template <class _NodePtr>
void __inorder(_NodePtr __x) {
    if (__x) {
        __inorder(__x->__left_);
        std::cout << __x->__key_ << " ";
        __inorder(__x->__right_);
    }
}

template <class _NodePtr, class _Tp>
std::pair<_NodePtr, bool> __insertBST(_NodePtr& __root, const _Tp& key) {
    if (!__root) {
        __root = new Node<_Tp>(key);
        return std::pair<_NodePtr, bool>(__root, true);
    }
    if (key < __root->__key_) {
        return __insertBST(__root->__left_, key);
    } else if (key > __root->__key_) {
        return __insertBST(__root->__right_, key);
    }
    return std::pair<_NodePtr, bool>(__root, false); // Key already exists
}

template <class _NodePtr, class _Tp>
_NodePtr __eraseBST(_NodePtr& __root, const _Tp& key, bool& display_tree) {
    if (!__root) {
        display_tree = false;
        return nullptr;
    }

    if (key < __root->__key_) {
        __root->__left_ = __eraseBST(__root->__left_, key, display_tree);
    } else if (key > __root->__key_) {
        __root->__right_ = __eraseBST(__root->__right_, key, display_tree);
    } else {
        if (!__root->__left_) {
            _NodePtr temp = __root->__right_;
            delete __root;
            __root = temp;
        } else if (!__root->__right_) {
            _NodePtr temp = __root->__left_;
            delete __root;
            __root = temp;
        } else {
            unsigned left_height = __height(__root->__left_);
            unsigned right_height = __height(__root->__right_);
            if (left_height >= right_height) {
                _NodePtr temp = __tree_max(__root->__left_);
                __root->__key_ = temp->__key_;
                __root->__left_ = __eraseBST(__root->__left_, temp->__key_, display_tree);
            } else {
                _NodePtr temp = __tree_min(__root->__right_);
                __root->__key_ = temp->__key_;
                __root->__right_ = __eraseBST(__root->__right_, temp->__key_, display_tree);
            }
        }
    }

    return __root;
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

template <class _Tp>
class BST {
public:
    typedef _Tp key_type;
    typedef std::size_t size_type;
    typedef Node<key_type>* pointer;
    typedef const Node<key_type>* const_pointer;

private:
    pointer __root_;
    bool display_tree;

public:
    BST() : __root_(nullptr), display_tree(true) {}

    size_type height() const {
        return __height(__root_);
    }

    size_type size() const {
        return __size(__root_);
    }

    void inorder() const {
        __inorder(__root_);
        std::cout << std::endl;
    }

    std::pair<const_pointer, bool> insert(const key_type& key) {
        std::pair<pointer, bool> result = __insertBST(__root_, key);
        if (!result.second) {
            std::cerr << "i " << key << ": The key already exists" << std::endl;
        }
        return std::pair<const_pointer, bool>(result.first, result.second);
    }

    const_pointer erase(const key_type& key) {
        bool tempbool = true;
        pointer __r = __eraseBST(__root_, key, tempbool);
        if (!tempbool) {
            std::cerr << "d " << key << ": The key does not exist" << std::endl;
            display_tree = false;
        } else {
            display_tree = (__root_ != nullptr);
        }

        return __root_;
    }

    void clear() {
        __clear(__root_);
    }

    void set_display_tree(bool value) {
        display_tree = value;
    }

    bool get_display_tree() const {
        return display_tree;
    }

    friend std::ostream& operator<<(std::ostream& os, const BST& tree) {
        if (tree.display_tree && tree.__root_) {
            os << tree.__root_;
        }
        return os;
    }
};

int main() {
    BST<int> tree;
    char command;
    int key;

    while (std::cin >> command >> key) {
        tree.set_display_tree(true);
        switch (command) {
            case 'i':
                if (!tree.insert(key).second) {
                    tree.set_display_tree(false);
                }
                if (tree.get_display_tree()) {
                    std::cout << tree << std::endl;
                }
                break;

            case 'd':
                if (tree.size() == 0) {
                    // 트리가 비어있을 때 삭제 시도 시 에러 메시지 출력
                    std::cerr << "d " << key << ": The key does not exist" << std::endl;
                } else {
                    if (tree.erase(key) == nullptr) {
                        tree.set_display_tree(false);
                    }
                    if (tree.get_display_tree()) {
                        std::cout << tree << std::endl;
                    }
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
