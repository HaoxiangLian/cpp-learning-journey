#include <iostream>
#include <memory>

struct Node {
    explicit Node(int node_id) : id{node_id} {
        std::cout << "construct " << id << '\n';
    }

    ~Node() {
        std::cout << "destroy " << id << '\n';
    }

    int id;
    std::shared_ptr<Node> next;
};

int main() {
    auto first = std::make_shared<Node>(1);
    auto second = std::make_shared<Node>(2);
    first->next = second;
    second->next = first;
    std::cout << "first strong owners: " << first.use_count() << '\n';
    std::cout << "second strong owners: " << second.use_count() << '\n';
}
