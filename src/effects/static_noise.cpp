#include "static_noise.h"
#include "bn_sprite_palette_ptr.h"
#include "bn_blending.h"
#include "bn_log.h"

// Importar todos los sprite items de ruido
#include "bn_sprite_items_white_noise1.h"
#include "bn_sprite_items_white_noise2.h"
#include "bn_sprite_items_white_noise3.h"
#include "bn_sprite_items_white_noise4.h"
#include "bn_sprite_items_white_noise5.h"
#include "bn_sprite_items_white_noise6.h"
#include "bn_sprite_items_white_noise7.h"
#include "bn_sprite_items_white_noise8.h"

StaticNoise::StaticNoise() {}

void StaticNoise::show(bool with_transparency, bn::fixed alpha)
{
    _transparency_enabled = with_transparency;
    _current_alpha = alpha;
    _current_frame = 0;
    _frame_counter = 0;
    
    create_grid();
    
    _visible = true;
}

void StaticNoise::hide()
{
    _noise_sprites.clear();
    _visible = false;
    
    // Resetear blending
    if (_transparency_enabled)
    {
        bn::blending::set_transparency_alpha(0);
    }
}

void StaticNoise::update()
{
    if (!_visible)
        return;
    
    _frame_counter++;
    
    if (_frame_counter >= _frames_per_change)
    {
        _frame_counter = 0;
        _current_frame = (_current_frame + 1) % FRAME_COUNT;
        
        change_frame(_current_frame);
    }
}

void StaticNoise::create_grid()
{
    _noise_sprites.clear();
    
    // Calcular posiciones para cubrir toda la pantalla
    int start_x = -120 + 16;
    int start_y = -80 + 16;
    
    for (int row = 0; row < GRID_ROWS; row++)
    {
        for (int col = 0; col < GRID_COLS; col++)
        {
            int x = start_x + (col * 32);
            int y = start_y + (row * 32);
            
            bn::sprite_ptr sprite = bn::sprite_items::white_noise1.create_sprite(x, y);
            
            if (_transparency_enabled)
            {
                sprite.set_blending_enabled(true);
            }
            
            _noise_sprites.push_back(bn::move(sprite));
        }
    }
    
    if (_transparency_enabled)
    {
        bn::blending::set_transparency_alpha(_current_alpha);
    }
}

void StaticNoise::change_frame(int frame_index)
{
    // Obtener el sprite_item correspondiente
    const bn::sprite_item* current_item = nullptr;
    
    switch (frame_index)
    {
    case 0:
        current_item = &bn::sprite_items::white_noise1;
        break;
    case 1:
        current_item = &bn::sprite_items::white_noise2;
        break;
    case 2:
        current_item = &bn::sprite_items::white_noise3;
        break;
    case 3:
        current_item = &bn::sprite_items::white_noise4;
        break;
    case 4:
        current_item = &bn::sprite_items::white_noise5;
        break;
    case 5:
        current_item = &bn::sprite_items::white_noise6;
        break;
    case 6:
        current_item = &bn::sprite_items::white_noise7;
        break;
    case 7:
        current_item = &bn::sprite_items::white_noise8;
        break;
    default:
        current_item = &bn::sprite_items::white_noise1;
        break;
    }
    
    // Actualizar todos los sprites con el nuevo frame
    int sprite_count = _noise_sprites.size();
    for (int i = 0; i < sprite_count; i++)
    {
        _noise_sprites[i].set_item(*current_item);
    }
}

void StaticNoise::set_transparency(bn::fixed alpha)
{
    _current_alpha = alpha;
    
    if (_transparency_enabled)
    {
        bn::blending::set_transparency_alpha(_current_alpha);
    }
}

void StaticNoise::enable_transparency(bool enabled)
{
    _transparency_enabled = enabled;
    
    int sprite_count = _noise_sprites.size();
    for (int i = 0; i < sprite_count; i++)
    {
        _noise_sprites[i].set_blending_enabled(enabled);
    }
    
    if (enabled)
    {
        bn::blending::set_transparency_alpha(_current_alpha);
    }
    else
    {
        bn::blending::set_transparency_alpha(0);
    }
}