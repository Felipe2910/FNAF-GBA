#pragma once
#include <stdint.h>
#include "../core/game_context.h"

class Animatronic {
public:
    void init(uint8_t level);
    void update(GameContext& ctx);

    bool at_left_door() const;
    bool at_right_door() const;

private:
    uint8_t _ai_level = 0;
    uint8_t _position = 0;  // 0 = stage, 1 = hall, 2 = door
    uint16_t _move_timer = 0;
};