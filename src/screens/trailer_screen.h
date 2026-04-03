#pragma once

#include "bn_optional.h"
#include "bn_regular_bg_ptr.h"
#include "screen_manager.h"

class TrailerScreen : public ScreenBase
{
public:
    void enter() override;
    void update() override;
    void exit() override;

    Screen next_screen() const override;

private:
    bn::optional<bn::regular_bg_ptr> _bg;

    int _frame = 0;
    int _timer = 0;

    Screen _next = Screen::NONE;

    static constexpr int FPS = 10;
    static constexpr int FRAME_DELAY = 60 / FPS;
    static constexpr int TOTAL_FRAMES = 345; // Ajustar según el número de frames disponibles
};