#include "trailer_screen.h"

#include "bn_keypad.h"
#include "bn_music.h"
#include "bn_music_items.h"
#include "bn_regular_bg_ptr.h"

#include "trailer_frames.h"

void set_trailer_frame(int index, bn::optional<bn::regular_bg_ptr>& bg);

void TrailerScreen::enter()
{
    _frame = 0;
    _timer = 0;
    _next = Screen::NONE;

    // Crear BG inicial
    _bg = bn::regular_bg_items::trailer_0000.create_bg(0, 0);

    // (Opcional) música del trailer
    bn::music_items::fnaf_trailer.play(1.0f, false);
}

void TrailerScreen::update()
{
    _timer++;

    // Skip con botones
    if (bn::keypad::a_pressed() || bn::keypad::start_pressed())
    {
        _next = Screen::MAIN_MENU;
        return;
    }

    // Control FPS (10 FPS recomendado)
    if (_timer >= FRAME_DELAY)
    {
        _timer = 0;
        _frame++;

        if (_frame >= TOTAL_FRAMES)
        {
            _next = Screen::MAIN_MENU;
            return;
        }

        set_trailer_frame(_frame, _bg);
    }
}

void TrailerScreen::exit()
{
    _bg = bn::nullopt;
    bn::music::stop();
}

Screen TrailerScreen::next_screen() const
{
    return _next;
}