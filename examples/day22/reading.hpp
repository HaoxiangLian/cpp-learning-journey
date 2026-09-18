#ifndef CPP_LEARNING_DAY22_READING_HPP
#define CPP_LEARNING_DAY22_READING_HPP

struct Reading {
    int sensor_id;
    double value;
};

bool is_valid(const Reading& reading);

#endif
