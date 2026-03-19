#pragma once

#include <stdint.h>

class Clock {
public:
    Clock();

    void reset();
    void update();

    uint8_t hour() const;
    bool is_finished() const;

private:
    uint32_t _frame_counter;
    uint8_t _hour;
};