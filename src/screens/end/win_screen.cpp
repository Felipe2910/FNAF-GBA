#include "win_screen.h"
#include "bn_keypad.h"
#include "bn_sprite_text_generator.h"
#include "common_variable_8x16_sprite_font.h"

WinScreen::WinScreen() {}

void WinScreen::enter()
{
    _next_screen = Screen::NONE;
    _sprites.clear();
    
    bn::sprite_text_generator gen(common::variable_8x16_sprite_font);
    gen.generate(-50, -10, "6 AM", _sprites);
    gen.generate(-80, 10, "Press A to continue", _sprites);
}

void WinScreen::exit()
{
    _sprites.clear();
}

void WinScreen::update()
{
    if (bn::keypad::a_pressed())
    {
        _next_screen = Screen::MAIN_MENU;
    }
}

Screen WinScreen::next_screen() const
{
    return _next_screen;
}