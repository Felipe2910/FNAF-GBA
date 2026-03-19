#include "gameover_screen.h"
#include "bn_keypad.h"
#include "bn_sprite_text_generator.h"
#include "common_variable_8x16_sprite_font.h"

#include "bn_regular_bg_items_gameover.h"

GameOverScreen::GameOverScreen() {}

void GameOverScreen::enter()
{
    _next_screen = Screen::NONE;
    _sprites.clear();

    _bg = bn::regular_bg_items::gameover.create_bg(0, 0);
    
    bn::sprite_text_generator gen(common::variable_8x16_sprite_font);
    gen.generate(50, 70, "GAME OVER", _sprites);
    gen.generate(-60, 50, "Press A to continue", _sprites);
}

void GameOverScreen::exit()
{
    _sprites.clear();
    _bg = bn::nullopt;
}

void GameOverScreen::update()
{
    if (bn::keypad::a_pressed())
    {
        _next_screen = Screen::MAIN_MENU;
    }
}

Screen GameOverScreen::next_screen() const
{
    return _next_screen;
}