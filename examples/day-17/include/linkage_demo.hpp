#ifndef CPP_LEARNING_DAY17_LINKAGE_DEMO_HPP
#define CPP_LEARNING_DAY17_LINKAGE_DEMO_HPP

namespace robot {

using ModeCode = int;

extern ModeCode active_mode;

ModeCode current_mode();
int left_local_value();
int right_local_value();

inline ModeCode double_mode(ModeCode value) {
    return value * 2;
}

inline constexpr int max_axes{6};

}

#endif
