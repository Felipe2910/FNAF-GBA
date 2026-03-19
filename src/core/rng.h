#pragma once

#include <stdint.h>

class RNG {
public:
    RNG(uint32_t seed = 1);

    uint32_t next();
    uint32_t range(uint32_t min, uint32_t max);

private:
    uint32_t _state;
};