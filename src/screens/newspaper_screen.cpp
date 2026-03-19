#include "newspaper_screen.h"

#include "bn_keypad.h"

#include "bn_regular_bg_items_newspaper.h"

void NewspaperScreen::enter()
{
    _timer = 0;
    _next = Screen::NONE;

    _bg = bn::regular_bg_items::newspaper.create_bg(0, 0);
}

void NewspaperScreen::update()
{
    _timer++;

    if (bn::keypad::a_pressed())
        _timer = 600; // saltar

    if (_timer > 600) // 10 segundos
        _next = Screen::GAME;
}

void NewspaperScreen::exit()
{
    _bg = bn::nullopt;
}

Screen NewspaperScreen::next_screen() const
{
    return _next;
}