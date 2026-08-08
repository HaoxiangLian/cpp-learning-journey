#include "linkage_demo.hpp"

namespace robot {
static int local_value{20};

int right_local_value() {
    return local_value;
}
}
