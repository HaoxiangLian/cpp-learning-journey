// INTENTIONALLY INVALID: this file must fail during compilation.

class UniqueSession {
public:
    UniqueSession() = default;
    UniqueSession(const UniqueSession&) = delete;
};

int main() {
    UniqueSession first;
    UniqueSession second{first};  // error: copy constructor is deleted
}
