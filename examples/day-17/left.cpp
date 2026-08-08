#include "linkage_demo.hpp"

namespace robot {
namespace {
int local_value{10};
}

int left_local_value() {
    return local_value;
}
}
