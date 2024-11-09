int main() {
    BST<int> tree;
    char command;
    int key;
    bool first_output = true;

    while (std::cin >> command >> key) {
        tree.set_display_tree(true);
        switch (command) {
            case 'i':
                if (!tree.insert(key).second) {
                    tree.set_display_tree(false);
                }
                if (tree.get_display_tree()) {
                    if (!first_output) std::cout << std::endl;
                    std::cout << tree;
                    first_output = false;
                }
                break;

            case 'd':
                if (tree.size() == 0) {
                    // 트리가 비어있을 때 삭제 시도 시 에러 메시지 출력만 하고 빈 트리 출력 생략
                    std::cerr << "d " << key << ": The key does not exist" << std::endl;
                } else {
                    if (tree.erase(key) == nullptr) {
                        tree.set_display_tree(false);
                    }
                    if (tree.get_display_tree()) {
                        if (!first_output) std::cout << std::endl;
                        std::cout << tree;
                        first_output = false;
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
