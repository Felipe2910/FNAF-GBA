#include "save_data.h"
#include <stdint.h>

// SRAM GBA base address
static volatile uint8_t *const SRAM_BASE =
    reinterpret_cast<volatile uint8_t *>(0x0E000000);

// ---------------------------------
// COPIA SEGURA DESDE SRAM → RAM
// ---------------------------------
static SaveData read_sram()
{
    SaveData data;

    const volatile SaveData *sram =
        reinterpret_cast<const volatile SaveData *>(SRAM_BASE);

    data.max_night = sram->max_night;
    data.stars = sram->stars;
    data.flags = sram->flags;
    data.checksum = sram->checksum;

    return data;
}

// ---------------------------------
// COPIA SEGURA DESDE RAM → SRAM
// ---------------------------------
static void write_sram(const SaveData &data)
{
    volatile SaveData *sram =
        reinterpret_cast<volatile SaveData *>(SRAM_BASE);

    sram->max_night = data.max_night;
    sram->stars = data.stars;
    sram->flags = data.flags;
    sram->checksum = data.checksum;
}

// ---------------------------------
// CHECKSUM SIMPLE (XOR)
// ---------------------------------
static uint8_t compute_checksum(const SaveData &data)
{
    return data.max_night ^ data.stars ^ data.flags;
}

// ---------------------------------
// INIT
// ---------------------------------
void save_init()
{
    SaveData data = read_sram();

    if (data.checksum != compute_checksum(data))
    {
        SaveData clean = make_new_save();
        write_sram(clean);
    }
}

// ---------------------------------
// EXISTE SAVE?
// ---------------------------------
bool save_exists()
{
    SaveData data = read_sram();

    // Primero verificar checksum
    if (data.checksum != compute_checksum(data))
        return false;

    // Luego validar flag
    if (!(data.flags & FLAG_HAS_SAVE))
        return false;

    // Finalmente validar rangos
    if (data.max_night > 6)
        return false;

    if (data.stars > 3)
        return false;

    return true;
}

// ---------------------------------
// LOAD
// ---------------------------------
SaveData load_save()
{
    if (!save_exists())
        return make_new_save();

    return read_sram();
}

// ---------------------------------
// WRITE
// ---------------------------------
void write_save(const SaveData &data)
{
    SaveData temp = data;
    temp.flags |= FLAG_HAS_SAVE;
    temp.checksum = compute_checksum(temp);

    write_sram(temp);
}

// ---------------------------------
// NUEVO SAVE
// ---------------------------------
SaveData make_new_save()
{
    SaveData data{};
    data.max_night = 0;
    data.stars = 0;
    data.flags = FLAG_HAS_SAVE;
    data.checksum = compute_checksum(data);

    return data;
}