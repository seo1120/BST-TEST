#include <iostream>

// BST 노드 구조체 정의
struct BSTNode {
    int key;
    BSTNode* left;
    BSTNode* right;

    BSTNode(int k) : key(k), left(nullptr), right(nullptr) {}
};

// 이진 검색 트리 클래스
class BST {
private:
    BSTNode* root;

    // 두 개의 정수 중 더 큰 값을 반환하는 함수
    int max(int a, int b) {
        return (a > b) ? a : b;
    }

    // 높이를 재귀적으로 계산하는 헬퍼 함수
    int getHeight(BSTNode* node) {
        if (node == nullptr) return 0;
        return 1 + max(getHeight(node->left), getHeight(node->right));
    }

    // 서브트리 크기를 재귀적으로 계산하는 헬퍼 함수
    int getSize(BSTNode* node) {
        if (node == nullptr) return 0;
        return 1 + getSize(node->left) + getSize(node->right);
    }

    // insertBST 함수 구현 (같은 값이 있으면 오류 출력)
    bool insertBST(BSTNode*& T, int newKey) {
        if (T == nullptr) {
            T = new BSTNode(newKey);
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
    BSTNode* findMinInRightSubtree(BSTNode* node) {
        while (node && node->left) {
            node = node->left;
        }
        return node;
    }

    // 왼쪽 서브트리에서 가장 큰 노드 찾기
    BSTNode* findMaxInLeftSubtree(BSTNode* node) {
        while (node && node->right) {
            node = node->right;
        }
        return node;
    }

    // deleteBST 함수 구현 (자식 노드를 올바르게 삭제하고, 메모리 누수 방지)
    bool deleteBST(BSTNode*& node, int deleteKey, bool& success) {
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
                BSTNode* temp = node;
                node = node->right;
                delete temp;
            } else if (node->right == nullptr) {
                BSTNode* temp = node;
                node = node->left;
                delete temp;
            } else {
                int leftheight = getHeight(node->left);
                int rightheight = getHeight(node->right);
                BSTNode* temp = nullptr;

                if (leftheight == rightheight) {
                    if (getSize(node->left) >= getSize(node->right)) {
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

    // 트리 출력 (중위 순회) - 트리의 괄호 형식 출력
    void printTree(BSTNode* node, std::ostream& os) {
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

    BSTNode* getRoot() const {
        return root;
    }

    void clear() {
        clear(root);
    }

private:
    void clear(BSTNode*& node) {
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

            default:
                std::cerr << "Invalid command: " << command << std::endl;
                return 1;
        }
    }
    tree.clear();
    return 0;
}
