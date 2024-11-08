#include <iostream>
#include <stack>

// Node 클래스
class Node {
public:
    int key;        // 키 값
    Node* left;     // 왼쪽 자식 노드
    Node* right;    // 오른쪽 자식 노드

    Node(int key) : key(key), left(nullptr), right(nullptr) {}

    friend std::ostream& operator<<(std::ostream& os, const Node* node) {
        if (node) {
            os << "<";
            if (node->left) os << node->left;
            os << " " << node->key << " ";
            if (node->right) os << node->right;
            os << ">";
        } else {
            os << "<>";
        }
        return os;
    }
};

// getNodeBST 함수: 새 노드를 생성하는 함수
Node* getNodeBST(int key) {
    return new Node(key);
}

// BST 클래스
class BST {
private:
    Node* root;

    // 트리의 노드를 재귀적으로 삭제하는 함수
    void clear(Node* node) {
        if (node) {
            clear(node->left);
            clear(node->right);
            delete node;
        }
    }

public:
    // 생성자
    BST() : root(nullptr) {}

    // 소멸자
    ~BST() {
        clear();
    }

    // 삽입 함수
    bool insertBST(int key) {
        Node* newNode = getNodeBST(key);
        if (!root) {
            root = newNode;
            return true;
        }

        Node* current = root;
        Node* parent = nullptr;

        while (current) {
            parent = current;
            if (key == current->key) {
                delete newNode; // 이미 존재하는 키 값이면 새 노드 삭제
                return false;
            } else if (key < current->key) {
                current = current->left;
            } else {
                current = current->right;
            }
        }

        if (key < parent->key) {
            parent->left = newNode;
        } else {
            parent->right = newNode;
        }
        return true;
    }

    // 삭제 함수
    bool eraseBST(int key) {
        Node* current = root;
        Node* parent = nullptr;

        // 삭제할 노드를 찾는다.
        while (current && current->key != key) {
            parent = current;
            if (key < current->key) {
                current = current->left;
            } else {
                current = current->right;
            }
        }

        if (!current) return false; // 삭제할 키가 존재하지 않음

        // 자식이 두 개인 경우
        if (current->left && current->right) {
            Node* successor = current->right;
            Node* successorParent = current;

            while (successor->left) {
                successorParent = successor;
                successor = successor->left;
            }

            current->key = successor->key;
            current = successor;
            parent = successorParent;
        }

        // 자식이 없거나 하나인 경우
        Node* child = (current->left) ? current->left : current->right;

        if (!parent) {
            root = child;
        } else if (parent->left == current) {
            parent->left = child;
        } else {
            parent->right = child;
        }

        delete current;
        return true;
    }

    // 트리를 중위 순회하여 출력하는 함수
    void inorder() const {
        inorder(root);
        std::cout << std::endl;
    }

    // 중위 순회 함수
    void inorder(Node* node) const {
        if (node) {
            inorder(node->left);
            std::cout << node->key << " ";
            inorder(node->right);
        }
    }

    // 트리를 삭제하는 함수
    void clear() {
        clear(root);
        root = nullptr;
    }

    bool is_empty() const {
        return root == nullptr;
    }

    friend std::ostream& operator<<(std::ostream& os, const BST& tree) {
        os << tree.root;
        return os;
    }
};

int main() {
    BST tree;
    char command;
    int key;

    while (std::cin >> command >> key) {
        bool display_tree = true; // Flag to control whether to display tree or not
        bool was_empty = tree.is_empty();

        switch (command) {
            case 'i':
                if (!tree.insertBST(key)) {
                    std::cerr << "i " << key << ": The key already exists" << std::endl;
                    display_tree = false; // Suppress tree display on duplicate insert
                }
                break;
            case 'd':
                if (!tree.eraseBST(key)) {
                    std::cerr << "d " << key << ": The key does not exist";
                    display_tree = false; // Suppress tree display on non-existing delete
                } else if (tree.is_empty()) {
                    display_tree = false; // Suppress tree display if tree is now empty
                    std::cout << std::endl; // Only add newline if the tree is now empty after deletion
                }
                break;
            default:
                std::cerr << "Invalid command: " << command << std::endl;
                return 1;
        }
        
        // If tree should be displayed, output tree structure
        if (display_tree && !tree.is_empty()) {
            std::cout << tree << std::endl;
        }
    }

    return 0;
}
