#include <iostream>
#include <stack>

// Node 클래스
class Node {
public:
    int key;        // 키 값
    Node* left;     // 왼쪽 자식 노드
    Node* right;    // 오른쪽 자식 노드

    Node(int key) : key(key), left(nullptr), right(nullptr) {}
};

// getNodeBST 함수: 새 노드를 생성하는 함수
Node* getNodeBST(int key) {
    return new Node(key);
}

// BST 클래스
class BST {
private:
    Node* root;

    // 트리의 높이를 계산하는 함수
    int height(Node* node) {
        if (!node) return 0;
        return 1 + std::max(height(node->left), height(node->right));
    }

    // 트리의 크기를 계산하는 함수
    int size(Node* node) {
        if (!node) return 0;
        return 1 + size(node->left) + size(node->right);
    }

    // 트리에서 최소 노드를 찾는 함수
    Node* minNode(Node* node) {
        while (node && node->left) {
            node = node->left;
        }
        return node;
    }

    // 트리에서 최대 노드를 찾는 함수
    Node* maxNode(Node* node) {
        while (node && node->right) {
            node = node->right;
        }
        return node;
    }

    // 트리를 중위 순회하여 출력하는 함수
    void inorder(Node* node) const {
        if (node) {
            inorder(node->left);
            std::cout << node->key << " ";
            inorder(node->right);
        }
    }

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
            Node* successor = minNode(current->right);
            int successorKey = successor->key;
            eraseBST(successorKey);
            current->key = successorKey;
        }
        // 자식이 없거나 하나인 경우
        else {
            Node* child = (current->left) ? current->left : current->right;

            if (!parent) {
                root = child;
            } else if (parent->left == current) {
                parent->left = child;
            } else {
                parent->right = child;
            }
            delete current;
        }
        return true;
    }

    // 트리의 높이를 반환하는 함수
    int height() {
        return height(root);
    }

    // 트리의 크기를 반환하는 함수
    int size() {
        return size(root);
    }

    // 트리에서 최소 노드를 반환하는 함수
    Node* minNode() {
        return minNode(root);
    }

    // 트리에서 최대 노드를 반환하는 함수
    Node* maxNode() {
        return maxNode(root);
    }

    // 트리를 중위 순회하여 출력하는 함수
    void inorder() const {
        inorder(root);
        std::cout << std::endl;
    }

    // 트리를 삭제하는 함수
    void clear() {
        clear(root);
        root = nullptr;
    }
};

int main() {
    BST tree;

    // 삽입 테스트
    tree.insertBST(25);
    tree.insertBST(15);
    tree.insertBST(50);
    tree.insertBST(10);
    tree.insertBST(22);

    std::cout << "Inorder traversal: ";
    tree.inorder(); // Expected output: 10 15 22 25 50

    // 삭제 테스트
    tree.eraseBST(15);
    std::cout << "After deleting 15: ";
    tree.inorder(); // Expected output: 10 22 25 50

    // 높이와 크기 테스트
    std::cout << "Tree height: " << tree.height() << std::endl;
    std::cout << "Tree size: " << tree.size() << std::endl;

    // 최소/최대 노드 테스트
    std::cout << "Min node: " << tree.minNode()->key << std::endl;
    std::cout << "Max node: " << tree.maxNode()->key << std::endl;

    // 트리 삭제 테스트
    tree.clear();
    std::cout << "Tree cleared. Inorder traversal: ";
    tree.inorder(); // Expected output: (empty)

    return 0;
}
