#include "freddy.h"

void Freddy::init(uint8_t ai_level)
{
    _ai_level = (ai_level > 20) ? 20 : ai_level;
    reset();
}

void Freddy::reset()
{
    _pos               = FreddyPos::SHOW_STAGE;
    _mode              = FreddyMode::NORMAL;
    _move_timer        = 0;
    _attack_timer      = 0;
    _powerout_timer    = 0;
    _powerout_threshold = 0;
}

void Freddy::trigger_power_out()
{
    _mode = FreddyMode::POWER_OUT;
    _powerout_timer = 0;

    // Sortear el delay aleatorio: MIN + (rng % rango)
    uint16_t range = POWEROUT_MAX - POWEROUT_MIN;
    _powerout_threshold = POWEROUT_MIN + (uint16_t)(_rng.next() % range);
}

void Freddy::update(GameContext& ctx)
{
    if (ctx.game_over || ctx.win)
        return;

    switch (_mode)
    {
        case FreddyMode::NORMAL:    update_normal(ctx);    break;
        case FreddyMode::POWER_OUT: update_power_out(ctx); break;
    }

    // Exponer al contexto
    ctx.freddy.camera       = FREDDY_CAM_ID[static_cast<uint8_t>(_pos)];
    ctx.freddy.at_right_door = (_pos == FreddyPos::RIGHT_DOOR);
}

// ---------------------------------------------------------------------------
void Freddy::update_normal(GameContext& ctx)
{
    if (_ai_level == 0)
        return;

    _move_timer++;
    if (_move_timer >= MOVE_INTERVAL)
    {
        _move_timer = 0;
        try_move(ctx);
    }

    if (_pos == FreddyPos::RIGHT_DOOR)
    {
        if (ctx.right_door_closed)
            _attack_timer = 0;
        else
            update_attack(ctx);
    }
}

// ---------------------------------------------------------------------------
void Freddy::try_move(GameContext& ctx)
{
    // Freddy SOLO se mueve si NO lo están mirando en la cámara actual.
    // Fuente: JS class Freddy — move() bloquea si this.watched es true.
    bool watched = (ctx.current_view == GameView::CAMERAS &&
                    ctx.freddy.camera != 0 &&
                    ctx.current_camera == ctx.freddy.camera);

    if (watched)
        return;

    uint8_t roll = (uint8_t)(_rng.next() % 20);
    if (roll < _ai_level)
    {
        if (_pos < FreddyPos::RIGHT_DOOR)
            _pos = static_cast<FreddyPos>(static_cast<uint8_t>(_pos) + 1);
        _attack_timer = 0;
    }
}

// ---------------------------------------------------------------------------
void Freddy::update_attack(GameContext& ctx)
{
    _attack_timer++;
    if (_attack_timer >= attack_threshold())
        ctx.game_over = true;
}

// ---------------------------------------------------------------------------
void Freddy::update_power_out(GameContext& ctx)
{
    // Con la luz apagada Freddy aparece en la oficina y espera su momento.
    // El Toreador March suena durante este tiempo (lo maneja Game/AudioManager).
    _powerout_timer++;
    if (_powerout_timer >= _powerout_threshold)
        ctx.game_over = true;
}

bool Freddy::at_right_door() const
{
    return _pos == FreddyPos::RIGHT_DOOR;
}