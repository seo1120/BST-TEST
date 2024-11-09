#include <iostream>

struct Node {
    int key;
    Node* left;
    Node* right;

    Node(int k) : key(k), left(nullptr), right(nullptr) {}
};

class BST {
private:
    Node* root;

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
            // 이미 존재할 경우
            std::cout << "i " << newKey << ": The key already exists" << std::endl;
            return false;
        }
    }

    // 오른쪽 서브트리에서 가장 작은 노드
    Node* minInRight(Node* node) {
        while (node && node->left) {
            node = node->left;
        }
        return node;
    }

    // 왼쪽 서브트리에서 가장 큰 노드
    Node* maxInLeft(Node* node) {
        while (node && node->right) {
            node = node->right;
        }
        return node;
    }

    bool deleteBST(Node*& node, int deleteKey, bool& success) {
        if (node == nullptr) {
            // 없는 경우 계속 타고 들어가다보며 없을 시 
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
                int leftHeight = Height(node->left);
                int rightHeight = Height(node->right);
                Node* temp = nullptr;

                if (leftHeight == rightHeight) {
                    if (Size(node->left) >= Size(node->right)) {
                        temp = maxInLeft(node->left);
                        node->key = temp->key;
                        deleteBST(node->left, temp->key, success);
                    } else {
                        temp = minInRight(node->right);
                        node->key = temp->key;
                        deleteBST(node->right, temp->key, success);
                    }
                } else if (leftHeight < rightHeight) {
                    temp = minInRight(node->right);
                    node->key = temp->key;
                    deleteBST(node->right, temp->key, success);
                } else {
                    temp = maxInLeft(node->left);
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
        // 왼쪽
        if (node->left != nullptr) {
            os << "<";
            printTree(node->left, os);
            os << ">";
        }

        os << " " << node->key << " ";

        // 오른쪽
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

    // 마지막에 제거 시 0개인지 확인
    Node* getRoot() const {
        return root;
    }

    void clear() {
        clear(root);
    }

private:
    // 전부 다 지움
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
                        std::cout << std::endl; // 개행
                    }
                }
                break;
            }
        }
    }
    tree.clear();
    return 0;
}
