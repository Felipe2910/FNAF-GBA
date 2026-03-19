#include "rng.h"

RNG::RNG(uint32_t seed) : _state(seed) {}

uint32_t RNG::next() {
    _state = _state * 1664525 + 1013904223;
    return _state;
}

uint32_t RNG::range(uint32_t min, uint32_t max) {
    return min + (next() % (max - min + 1));
}