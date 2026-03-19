#pragma once

#include "../screen_manager.h"
#include "bn_vector.h"
#include "bn_sprite_ptr.h"

class WinScreen : public ScreenBase
{
public:
    WinScreen();

    void enter() override;
    void exit() override;
    void update() override;
    Screen next_screen() const override;

private:
    Screen _next_screen = Screen::NONE;
    bn::vector<bn::sprite_ptr, 32> _sprites;
};