class UniqueHandle {
public:
    explicit UniqueHandle(int id)
        : id_{id} {}

    UniqueHandle(const UniqueHandle&) = delete;
    UniqueHandle& operator=(const UniqueHandle&) = delete;

private:
    int id_;
};

int main() {
    UniqueHandle first{7};
    UniqueHandle second{first};
}
