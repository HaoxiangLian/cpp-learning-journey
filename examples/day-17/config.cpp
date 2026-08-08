#include "linkage_demo.hpp"

namespace robot {

ModeCode active_mode{2};

ModeCode current_mode() {
    static int calls{};
    ++calls;
    return active_mode + calls;
}

}
