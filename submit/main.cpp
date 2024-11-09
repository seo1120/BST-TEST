#include <iostream>

// BST 노드 구조체 정의
struct Node {
    int key;
    Node* left;
    Node* right;

    Node(int k) : key(k), left(nullptr), right(nullptr) {}
};

// BST 클래스
class BST {
private:
    Node* root;

    // 새로운 노드를 생성하는 함수 (조건에 따라 정의)
    Node* getNodeBST(int key) {
        return new Node(key);
    }

    // 서브트리의 높이를 재귀적으로 계산하는 함수
    int height(Node* T) {
        if (T == nullptr) return 0;
        int leftHeight = height(T->left);
        int rightHeight = height(T->right);
        return 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
    }

    // 서브트리의 크기를 재귀적으로 계산하는 함수
    int size(Node* T) {
        if (T == nullptr) return 0;
        return 1 + size(T->left) + size(T->right);
    }

    // 왼쪽 서브트리에서 가장 큰 노드 찾기
    Node* maxNode(Node* T) {
        while (T && T->right) {
            T = T->right;
        }
        return T;
    }

    // 오른쪽 서브트리에서 가장 작은 노드 찾기
    Node* minNode(Node* T) {
        while (T && T->left) {
            T = T->left;
        }
        return T;
    }

    // 노드를 삽입하는 함수 (조건에 맞춰 구현)
    bool insertBST(Node*& T, int newKey) {
        if (T == nullptr) {
            T = getNodeBST(newKey);
            return true;
        }
        if (newKey < T->key) {
            return insertBST(T->left, newKey);
        } else if (newKey > T->key) {
            return insertBST(T->right, newKey);
        } else {
            std::cerr << "i " << newKey << ": The key already exists" << std::endl;
            return false;
        }
    }

    // 노드를 삭제하는 함수 (조건에 맞춰 구현)
    bool eraseBST(Node*& T, int deleteKey) {
        if (T == nullptr) {
            std::cerr << "d " << deleteKey << ": The key does not exist" << std::endl;
            return false;
        }

        if (deleteKey < T->key) {
            return eraseBST(T->left, deleteKey);
        } else if (deleteKey > T->key) {
            return eraseBST(T->right, deleteKey);
        } else {
            // 자식 노드가 없는 경우
            if (T->left == nullptr && T->right == nullptr) {
                delete T;
                T = nullptr;
            }
            // 하나의 자식 노드가 있는 경우
            else if (T->left == nullptr) {
                Node* temp = T;
                T = T->right;
                delete temp;
            } else if (T->right == nullptr) {
                Node* temp = T;
                T = T->left;
                delete temp;
            }
            // 두 개의 자식 노드가 있는 경우
            else {
                int leftHeight = height(T->left);
                int rightHeight = height(T->right);
                if (leftHeight > rightHeight || (leftHeight == rightHeight && size(T->left) >= size(T->right))) {
                    Node* maxInLeft = maxNode(T->left);
                    T->key = maxInLeft->key;
                    eraseBST(T->left, maxInLeft->key);
                } else {
                    Node* minInRight = minNode(T->right);
                    T->key = minInRight->key;
                    eraseBST(T->right, minInRight->key);
                }
            }
            return true;
        }
    }

    // 트리의 inorder 순회를 구현 (조건에 따라)
    void inorder(Node* T) {
        if (T != nullptr) {
            std::cout << "<";
            if (T->left) {
                inorder(T->left);
            }
            std::cout << " " << T->key << " ";
            if (T->right) {
                inorder(T->right);
            }
            std::cout << ">";
        }
    }

    // 트리를 삭제하는 함수
    void clear(Node*& T) {
        if (T != nullptr) {
            clear(T->left);
            clear(T->right);
            delete T;
            T = nullptr;
        }
    }

public:
    BST() : root(nullptr) {}

    // 삽입 함수 (외부 인터페이스)
    void insert(int key) {
        if (insertBST(root, key)) {
            std::cout << "<";
            inorder(root);
            std::cout << ">" << std::endl;
        }
    }

    // 삭제 함수 (외부 인터페이스)
    void erase(int key) {
        if (eraseBST(root, key)) {
            if (root != nullptr) {
                std::cout << "<";
                inorder(root);
                std::cout << ">" << std::endl;
            } else {
                std::cout << std::endl;  // 트리가 비어 있을 경우 빈 줄 출력
            }
        }
    }

    // 트리 전체를 삭제하는 함수
    void clear() {
        clear(root);
    }
};

int main() {
    BST tree;
    char command;
    int key;

    while (std::cin >> command >> key) {
        if (command == 'i') {
            tree.insert(key);
        } else if (command == 'd') {
            tree.erase(key);
        } else {
            std::cerr << "Invalid command: " << command << std::endl;
            return 1;
        }
    }

    tree.clear();
    return 0;
}
