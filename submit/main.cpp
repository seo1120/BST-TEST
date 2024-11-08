#include <iostream>
#include <stack>

// Node 클래스 정의
template <class T>
class Node {
public:
    T key;
    Node* left;
    Node* right;
    unsigned height;
    unsigned size;

    // 생성자
    Node(T k) : key(k), left(nullptr), right(nullptr), height(1), size(1) {}
};

// 헬퍼 함수들
template <class T>
Node<T>* getNodeBST(T key) {
    return new Node<T>(key);
}

template <class T>
unsigned height(Node<T>* node) {
    return node ? node->height : 0;
}

template <class T>
unsigned size(Node<T>* node) {
    return node ? node->size : 0;
}

template <class T>
Node<T>* minNode(Node<T>* node) {
    while (node && node->left) {
        node = node->left;
    }
    return node;
}

// 이진 검색 트리에 키 삽입
template <class T>
bool insertBST(Node<T>*& root, T key) {
    Node<T>* p = root;
    Node<T>* q = nullptr;
    std::stack<Node<T>*> nodeStack;

    // 삽입 위치를 찾으며 부모 노드 저장
    while (p) {
        if (key == p->key) {
            std::cerr << "i " << key << ": The key already exists" << std::endl;
            return false;
        }
        q = p;
        nodeStack.push(q);
        p = (key < p->key) ? p->left : p->right;
    }

    // 새 노드 생성
    Node<T>* newNode = getNodeBST(key);
    if (!q)
        root = newNode;
    else if (key < q->key)
        q->left = newNode;
    else
        q->right = newNode;

    // 높이와 크기 업데이트
    while (!nodeStack.empty()) {
        q = nodeStack.top();
        nodeStack.pop();
        q->height = 1 + std::max(height(q->left), height(q->right));
        q->size = 1 + size(q->left) + size(q->right);
    }
    return true;
}

// 이진 검색 트리에서 키 삭제
template <class T>
bool eraseBST(Node<T>*& root, T key) {
    Node<T>* p = root;
    Node<T>* q = nullptr;
    std::stack<Node<T>*> nodeStack;

    // 삭제할 노드 찾기
    while (p && p->key != key) {
        q = p;
        nodeStack.push(q);
        p = (key < p->key) ? p->left : p->right;
    }

    if (!p) {
        std::cerr << "d " << key << ": The key does not exist" << std::endl;
        return false; // 삭제할 키가 없음
    }

    // 두 자식을 가진 경우 오른쪽 서브트리의 최소 노드로 대체
    if (p->left && p->right) {
        nodeStack.push(p);
        Node<T>* successor = minNode(p->right);
        p->key = successor->key;
        q = p;
        p = successor;
    }

    // 자식이 하나이거나 없는 경우
    Node<T>* child = (p->left) ? p->left : p->right;
    if (q) {
        if (q->left == p) q->left = child;
        else q->right = child;
    } else {
        root = child;
    }

    delete p;

    // 높이와 크기 업데이트
    while (!nodeStack.empty()) {
        q = nodeStack.top();
        nodeStack.pop();
        q->height = 1 + std::max(height(q->left), height(q->right));
        q->size = 1 + size(q->left) + size(q->right);
    }
    return true;
}

// inorder 순회 함수
template <class T>
void inorder(Node<T>* root) {
    if (!root) return;
    
    std::cout << "<";
    if (root->left) inorder(root->left);
    std::cout << " " << root->key << " ";
    if (root->right) inorder(root->right);
    std::cout << ">";
}

// 트리 삭제 함수
template <class T>
void clear(Node<T>*& root) {
    if (root) {
        clear(root->left);
        clear(root->right);
        delete root;
        root = nullptr;
    }
}

// 메인 함수 테스트
int main() {
    Node<int>* root = nullptr;
    char command;
    int key;

    while (std::cin >> command >> key) {
        switch (command) {
            case 'i':
                if (insertBST(root, key)) {
                    if (root) {
                        inorder(root);
                    }
                    std::cout << std::endl;
                }
                break;
            case 'd':
                if (eraseBST(root, key)) {
                    if (root) {
                        inorder(root);
                        std::cout << std::endl;
                    } else {
                        std::cout << std::endl; // 트리가 비었을 때 개행만 출력
                    }
                }
                break;
            default:
                std::cerr << "Invalid command: " << command << std::endl;
                return 1;
        }
    }

    clear(root); // 메모리 해제
    return 0;
}
