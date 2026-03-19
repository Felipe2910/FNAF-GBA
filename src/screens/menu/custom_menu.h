#pragma once

#include <stdint.h>
#include "bn_vector.h"
#include "bn_sprite_ptr.h"
#include "bn_music_item.h"
#include "../screen_manager.h"
#include "../../core/game_settings.h"

enum class CustomAction
{
    NONE,
    START,
    BACK
};

class CustomMenu : public ScreenBase
{
public:
    CustomMenu();

    // Implementación de ScreenBase
    void enter() override;
    void exit() override;
    void update() override;
    Screen next_screen() const override;

    CustomAction action() const;
    const GameSettings& settings() const;

private:
    bool handle_input();
    void draw();
    void draw_values();

private:
    bool _needs_redraw = true;
    int _selected = 0;
    CustomAction _action = CustomAction::NONE;
    Screen _next_screen = Screen::NONE;

    GameSettings _settings;

    bn::vector<bn::sprite_ptr, 96> _sprites;
};