#pragma once

#include <stdint.h>

#include "bn_string.h"
#include "bn_vector.h"
#include "bn_sprite_ptr.h"

#include "../screens/screen_manager.h"
#include "../screens/office/office_screen.h"

#include "../core/game_context.h"
#include "../core/game_settings.h"
#include "../core/clock.h"
#include "../core/power.h"
#include "../core/rng.h"

#include "../animatronics/animatronic.h"
#include "../animatronics/bonnie.h"
#include "../animatronics/chica.h"
#include "../animatronics/foxy.h"
#include "../animatronics/freddy.h"

class Game : public ScreenBase
{
public:
    Game();

    // Implementación de ScreenBase
    void enter() override;
    void exit() override;
    void update() override;
    Screen next_screen() const override;

    // Métodos específicos del juego
    void start_normal(uint8_t night);
    void start_custom(const GameSettings &settings);

    const GameContext &context() const;

private:
    void handle_input();
    void update_clock();
    void update_power();
    void update_animatronics();
    void check_win_lose();

private:
    GameContext _ctx;
    GameSettings _settings;

    Clock _clock;
    Power _power{1};   // se reinicia en enter() con la noche correcta

    Bonnie  _bonnie;
    Chica   _chica;
    Foxy    _foxy;
    Freddy  _freddy;

    Screen _next_screen = Screen::NONE;

    OfficeScreen _office;

    uint8_t _pending_night = 0;
    bool _pending_custom = false;

    bn::vector<bn::sprite_ptr, 64> _sprites;  // 64: margen seguro para HUD + barras
};