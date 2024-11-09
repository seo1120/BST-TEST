#include <iostream>

// BST 노드 구조체 정의
struct Node {
    int key;
    Node* left;
    Node* right;

    Node(int k) : key(k), left(nullptr), right(nullptr) {}
};

class BST {
private:
    Node* root;

    // 두 개의 정수 중 더 큰 값을 반환하는 함수
    int max(int a, int b) {
        return (a > b) ? a : b;
    }

    int Height(Node* node) {
        if (node == nullptr) return 0;
        return 1 + max(Height(node->left), Height(node->right));
    }

    int Size(Node* node) {
        if (node == nullptr) return 0;
        return 1 + Size(node->left) + Size(node->right);
    }

    // insertBST 함수 구현 (같은 값이 있으면 오류 출력)
    bool insertBST(Node*& T, int newKey) {
        if (T == nullptr) {
            T = new Node(newKey);
            return true;
        }

        if (newKey < T->key) {
            return insertBST(T->left, newKey);
        } else if (newKey > T->key) {
            return insertBST(T->right, newKey);
        } else {
            std::cout << "i " << newKey << ": The key already exists" << std::endl;
            return false;
        }
    }

    // 오른쪽 서브트리에서 가장 작은 노드 찾기
    Node* findMinInRightSubtree(Node* node) {
        while (node && node->left) {
            node = node->left;
        }
        return node;
    }

    // 왼쪽 서브트리에서 가장 큰 노드 찾기
    Node* findMaxInLeftSubtree(Node* node) {
        while (node && node->right) {
            node = node->right;
        }
        return node;
    }

    // deleteBST 함수 구현 (자식 노드를 올바르게 삭제하고, 메모리 누수 방지)
    bool deleteBST(Node*& node, int deleteKey, bool& success) {
        if (node == nullptr) {
            std::cout << "d " << deleteKey << ": The key does not exist" << std::endl;
            success = false;
            return false;
        }

        if (deleteKey < node->key) {
            return deleteBST(node->left, deleteKey, success);
        } else if (deleteKey > node->key) {
            return deleteBST(node->right, deleteKey, success);
        } else {
            if (node->left == nullptr && node->right == nullptr) {
                delete node;
                node = nullptr;
            } else if (node->left == nullptr) {
                Node* temp = node;
                node = node->right;
                delete temp;
            } else if (node->right == nullptr) {
                Node* temp = node;
                node = node->left;
                delete temp;
            } else {
                int leftheight = Height(node->left);
                int rightheight = Height(node->right);
                Node* temp = nullptr;

                if (leftheight == rightheight) {
                    if (Size(node->left) >= Size(node->right)) {
                        temp = findMaxInLeftSubtree(node->left);
                        node->key = temp->key;
                        deleteBST(node->left, temp->key, success);
                    } else {
                        temp = findMinInRightSubtree(node->right);
                        node->key = temp->key;
                        deleteBST(node->right, temp->key, success);
                    }
                } else if (leftheight < rightheight) {
                    temp = findMinInRightSubtree(node->right);
                    node->key = temp->key;
                    deleteBST(node->right, temp->key, success);
                } else {
                    temp = findMaxInLeftSubtree(node->left);
                    node->key = temp->key;
                    deleteBST(node->left, temp->key, success);
                }
            }
        }

        success = true;
        return true;
    }

    void printTree(Node* node, std::ostream& os) {
        if (node == nullptr) return;

        if (node->left != nullptr) {
            os << "<";
            printTree(node->left, os);
            os << ">";
        }

        os << " " << node->key << " ";

        if (node->right != nullptr) {
            os << "<";
            printTree(node->right, os);
            os << ">";
        }
    }

public:
    BST() : root(nullptr) {}

    bool insert(int key) {
        return insertBST(root, key);
    }

    bool deleteKey(int key) {
        bool success = false;
        deleteBST(root, key, success);
        return success;
    }

    friend std::ostream& operator<<(std::ostream& os, BST& tree) {
        tree.printTree(tree.root, os);
        return os;
    }

    Node* getRoot() const {
        return root;
    }

    void clear() {
        clear(root);
    }

private:
    void clear(Node*& node) {
        if (node == nullptr) return;
        clear(node->left);
        clear(node->right);
        delete node;
        node = nullptr;
    }
};

int main() {
    BST tree;
    char command;
    int key;

    while (std::cin >> command >> key) {
        switch (command) {
            case 'i': {
                if (tree.insert(key)) {
                    std::cout << "<";
                    std::cout << tree;
                    std::cout << ">" << std::endl;
                }
                break;
            }

            case 'd': {
                if (tree.deleteKey(key)) {
                    if (tree.getRoot() != nullptr) {
                        std::cout << "<";
                        std::cout << tree;
                        std::cout << ">" << std::endl;
                    } else {
                        std::cout << std::endl;
                    }
                }
                break;
            }
        }
    }
    tree.clear();
    return 0;
}
