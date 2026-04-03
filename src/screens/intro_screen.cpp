#include "intro_screen.h"

#include "bn_keypad.h"
#include "bn_music.h"
#include "bn_regular_bg_items_felip_games_studio.h"
#include "bn_music_items.h"

void IntroScreen::enter()
{
    _timer = 0;
    _next = Screen::NONE;

    _bg = bn::regular_bg_items::felip_games_studio.create_bg(0, 0);

    bn::music_items::felip_games_studio.play(0.8f, false);
}

void IntroScreen::update()
{
    _timer++;

    // Permitir saltar intro
    if (bn::keypad::a_pressed() || bn::keypad::start_pressed())
    {
        _next = Screen::TRAILER;
        return;
    }
    // Duración ~3 segundos
    if (_timer > 180)
    {
        _next = Screen::TRAILER;
    }
}

void IntroScreen::exit()
{
    _bg = bn::nullopt;
    bn::music::stop();
}

Screen IntroScreen::next_screen() const
{
    return _next;
}