#include <iostream>

// Node 클래스
class Node {
public:
    int key;        // 키 값
    Node* left;     // 왼쪽 자식 노드
    Node* right;    // 오른쪽 자식 노드

    Node(int key) : key(key), left(nullptr), right(nullptr) {}

    // 트리 구조의 형식에 맞게 출력하는 연산자 오버로드
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
        clear(root);
    }

    // 삽입 함수
    bool insertBST(int key) {
        Node* newNode = new Node(key);
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

    bool is_empty() const {
        return root == nullptr;
    }

    // 트리의 중위 순회 형식을 맞추어 출력
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
        bool display_tree = true;

        switch (command) {
            case 'i':
                if (!tree.insertBST(key)) {
                    std::cerr << "i " << key << ": The key already exists" << std::endl;
                    display_tree = false;
                }
                break;
            case 'd':
                if (!tree.eraseBST(key)) {
                    std::cerr << "d " << key << ": The key does not exist" << std::endl;
                    display_tree = false;
                } else if (tree.is_empty()) {
                    display_tree = false;
                    std::cout << std::endl; // 마지막 노드 삭제 후 개행만 출력
                }
                break;
            default:
                std::cerr << "Invalid command: " << command << std::endl;
                return 1;
        }

        // 에러가 아니고 트리가 비어 있지 않으면 트리 출력
        if (display_tree && !tree.is_empty()) {
            std::cout << tree << std::endl;
        }
    }

    return 0;
}
