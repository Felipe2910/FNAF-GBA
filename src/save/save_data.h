#pragma once

#include <stdint.h>

struct SaveData {
    uint8_t max_night = 0;        // 0–6
    uint8_t stars = 0;            // 0–3
    uint8_t flags = 0;            // bits
    uint8_t checksum = 0;         // validación simple
};

// Flags
constexpr uint8_t FLAG_HAS_SAVE       = 1 << 0;
constexpr uint8_t FLAG_CUSTOM_UNLOCK  = 1 << 1;

// API pública
void save_init();
bool save_exists();

SaveData load_save();
void write_save(const SaveData& data);

SaveData make_new_save();