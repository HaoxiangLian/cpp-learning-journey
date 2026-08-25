// INTENTIONALLY INVALID: this file must fail during compilation.

class RetryLimit {
public:
    explicit RetryLimit(int count) : count_{count} {}

private:
    int count_;
};

void schedule(RetryLimit limit) {
    (void)limit;
}

int main() {
    schedule(3);  // error: explicit constructor cannot provide this implicit conversion
}
