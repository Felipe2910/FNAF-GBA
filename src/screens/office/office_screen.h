#pragma once
#include "bn_regular_bg_ptr.h"
#include "../../core/game_context.h"

class OfficeScreen
{
public:
    void enter();
    void update(const GameContext& ctx);
    void exit();

private:
    int _bg_pos = 0;
    int _bg_target = 0;
    int _bg_speed = 2;
    bn::optional<bn::regular_bg_ptr> _bg;
};
