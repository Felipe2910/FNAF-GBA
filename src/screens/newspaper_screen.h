#pragma once
#include "screen_manager.h"
#include "bn_optional.h"
#include "bn_regular_bg_ptr.h"

class NewspaperScreen : public ScreenBase
{
public:
    void enter() override;
    void exit() override;
    void update() override;
    Screen next_screen() const override;

private:
    int _timer = 0;
    Screen _next = Screen::NONE;
    bn::optional<bn::regular_bg_ptr> _bg;
};