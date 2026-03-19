#pragma once

#include "bn_sprite_ptr.h"
#include "bn_vector.h"
#include "bn_optional.h"
#include "bn_fixed.h"

class StaticNoise
{
public:
    StaticNoise();
    
    // Mostrar el efecto (con o sin transparencia)
    void show(bool with_transparency = true, bn::fixed alpha = 0.5);
    
    // Ocultar el efecto
    void hide();
    
    // Actualizar animación (ciclar entre frames)
    void update();
    
    // Cambiar transparencia en tiempo real
    void set_transparency(bn::fixed alpha);
    
    // Activar/desactivar transparencia
    void enable_transparency(bool enabled);
    
    // Verificar si está visible
    bool is_visible() const { return _visible; }
    
    // Cambiar velocidad de animación (frames por cambio)
    void set_speed(int frames_per_change) { _frames_per_change = frames_per_change; }

private:
    void create_grid();
    void change_frame(int frame_index);
    
private:
    static constexpr int FRAME_COUNT = 8;
    static constexpr int GRID_COLS = 8;  // 240/32 = 7.5, usamos 8
    static constexpr int GRID_ROWS = 5;  // 160/32 = 5
    
    bn::vector<bn::sprite_ptr, 64> _noise_sprites; // 8x5 = 40 sprites
    bool _visible = false;
    int _current_frame = 0;
    int _frame_counter = 0;
    int _frames_per_change = 2;
    bn::fixed _current_alpha = 0.5;
    bool _transparency_enabled = true;
};