#include "clock.h"

// Aproximado: 1 hora cada 60 segundos (60 FPS)
static constexpr uint32_t FRAMES_PER_HOUR = 60 * 60;

Clock::Clock() {
    reset();
}

void Clock::reset() {
    _frame_counter = 0;
    _hour = 0; // 12 AM
}

void Clock::update() {
    _frame_counter++;

    if(_frame_counter >= FRAMES_PER_HOUR) {
        _frame_counter = 0;
        if(_hour < 6)
            _hour++;
    }
}

uint8_t Clock::hour() const {
    return _hour;
}

bool Clock::is_finished() const {
    return _hour >= 6;
}