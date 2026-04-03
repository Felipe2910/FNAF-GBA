# Five Nights at Freddy's GBA

<p align="center">
  <img src="FNAF_GBA_BOX_ART_MX.png" alt="FNAF GBA Box Art México" width="30%">
  <img src="FNAF_GBA_BOX_ART_US.png" alt="FNAF GBA Box Art USA" width="30%">
  <img src="FNAF_GBA_BOX_ART_JP.png" alt="FNAF GBA Box Art Japón" width="30%">
</p>

> Demake de *Five Nights at Freddy's* para Game Boy Advance, desarrollado en C++ con [Butano](https://github.com/GValiente/butano).  
> Proyecto de **Felip Games Studio** — en desarrollo activo.

---

## Descripción

Port/demake fiel al original de Scott Cawthon para hardware GBA real (240×160, 60 FPS fijos). Reproduce la mecánica central del juego: gestionar energía, puertas y luces mientras los cuatro animatrónicos avanzan por el mapa de cámaras intentando llegar a tu oficina.

El juego es jugable de principio a fin en las 6 noches normales y la noche personalizada. Los valores de IA, drenaje de energía e intervalos de movimiento se tomaron directamente del código JavaScript de referencia y se convirtieron a frames a 60 FPS sin usar punto flotante.

---

## Estado del proyecto

| Módulo | Estado |
|--------|--------|
| Animatrónicos: Bonnie, Chica, Foxy, Freddy | ✅ Completo |
| Sistema de energía (drenaje por noche + multiplicadores) | ✅ Completo |
| Noches 1–6 con tabla de IA verificada | ✅ Completo |
| Noche personalizada (niveles 0–20 por animatrónico) | ✅ Completo |
| Oficina con scroll horizontal suavizado | ✅ Completo |
| Efecto de ruido estático (8 frames, grid 8×5 sprites) | ✅ Completo |
| Menú principal con fondos cíclicos y flicker aleatorio | ✅ Completo |
| Pantalla de intro (logo Felip Games Studio + música) | ✅ Completo |
| Pantalla de trailer animada (346 frames) | ✅ Completo |
| Pantalla de periódico (entre intro y noche 1) | ✅ Completo |
| Pantallas Win / Game Over | ✅ Completo |
| HUD (energía, barra de uso, hora, noche) | ✅ Completo |
| Guardado SRAM con checksum XOR | ✅ Completo |
| Transición Fade (negro ↔ pantalla) | ✅ Completo |
| Audio: música ambient, intro, menú (módulos .it / MaxMod) | ✅ Completo |
| Sistema de cámaras (navegación e imagen por cámara) | 🚧 WIP — estructura definida, sin implementación |
| Puertas y luces renderizadas en la oficina | 🚧 WIP — stubs declarados |
| Golden Freddy | 🚧 WIP — archivo placeholder |
| Audio Manager (abstracción sobre MaxMod) | 🚧 WIP — archivo vacío |
| Cola de eventos (`event_queue.h`) | 🚧 WIP — archivo vacío |
| Sistema de progresión y logros (`progression.h`) | 🚧 WIP — archivo vacío |

---

## Controles

| Botón | Acción |
|-------|--------|
| ← / → | Mirar a la izquierda / derecha de la oficina |
| **A** | Abrir cámaras |
| **B** | Cerrar cámaras / volver a vista de oficina |
| **L** | Cerrar / abrir puerta izquierda |
| **R** | Cerrar / abrir puerta derecha |
| **↑** (en vista lateral) | Encender / apagar luz del pasillo |
| ← / → (en cámaras) | Cambiar cámara activa |
| **A / Start** (intro) | Saltar intro |

---

## Estructura del proyecto

```
FNAF/
├── Makefile                          # Build estándar de Butano
├── README.md
├── FNAF_GBA_BOX_ART_*.png            # Arte de caja (MX, US, JP)
├── FNAF_cartridge.png
│
├── src/
│   ├── main.cpp                      # Entry point: init, screen manager, save, game loop
│   ├── game_state.h                  # Enum GameState (legacy, reemplazado por Screen)
│   │
│   ├── core/
│   │   ├── clock.h / .cpp            # Reloj: 12 AM → 6 AM (3600 frames por hora)
│   │   ├── power.h / .cpp            # Energía: drenaje por noche × multiplicador de uso
│   │   ├── rng.h / .cpp              # LCG simple (seed configurable por instancia)
│   │   ├── game_context.h            # Snapshot del estado compartido por todos los sistemas
│   │   ├── game_settings.h           # Niveles AI por animatrónico (0–20)
│   │   ├── event_queue.h             # 🚧 WIP
│   │   └── progression.h             # 🚧 WIP
│   │
│   ├── animatronics/
│   │   ├── animatronic.h / .cpp      # Clase base genérica (prototipo simplificado)
│   │   ├── bonnie.h / .cpp           # Ruta izquierda, attack timer, RNG propio
│   │   ├── chica.h / .cpp            # Ruta derecha, misma estructura que Bonnie
│   │   ├── foxy.h / .cpp             # Fases de cortina + sprint + daño a energía
│   │   ├── freddy.h / .cpp           # Ruta derecha + modo Power Out
│   │   └── golden_freddy.h           # 🚧 WIP
│   │
│   ├── game/
│   │   └── game.h / .cpp             # Loop: input, reloj, energía, animatrónicos, HUD
│   │
│   ├── save/
│   │   └── save_data.h / .cpp        # SRAM GBA (0x0E000000), checksum XOR
│   │
│   ├── effects/
│   │   └── static_noise.h / .cpp     # Ruido estático (8 frames, grid 40 sprites 32×32)
│   │
│   ├── audio/
│   │   └── audio_manager.h / .cpp    # 🚧 WIP
│   │
│   └── screens/
│       ├── screen_manager.h / .cpp   # Máquina de estados (register / change / update)
│       ├── intro_screen.h / .cpp     # Logo + música, skip con A/Start (~3 s)
│       ├── trailer_screen.h / .cpp   # Animación de 346 frames del trailer
│       ├── newspaper_screen.h / .cpp # Periódico previo a la noche 1
│       │
│       ├── menu/
│       │   ├── main_menu.h / .cpp    # Continue / New Game / Night 6 / Custom + efectos
│       │   └── custom_menu.h / .cpp  # Sliders 0–20 por animatrónico
│       │
│       ├── office/
│       │   ├── office_screen.h / .cpp # BG panorámico con scroll suavizado (±40 px)
│       │   ├── doors.h               # 🚧 WIP — stub
│       │   └── lights.h              # 🚧 WIP — stub
│       │
│       ├── cameras/
│       │   ├── camera_screen.h / .cpp # 🚧 WIP — interfaz definida
│       │   └── camera_map.h           # 🚧 WIP — vacío
│       │
│       ├── end/
│       │   ├── gameover_screen.h / .cpp  # Game Over → menú principal
│       │   └── win_screen.h / .cpp       # 6 AM → menú principal
│       │
│       └── transitions/
│           └── fade.h / .cpp         # Fade usando bn::bg/sprite_palettes
│
├── graphics/                         # BMPs + JSONs de metadata para Butano/grit
│   ├── menu1–4.bmp                   # Fondos del menú principal (ciclo con flicker)
│   ├── office.bmp                    # Fondo panorámico de la oficina
│   ├── gameover.bmp, newspaper.bmp, felip_games_studio.bmp
│   ├── trailer_0000–0345.bmp         # 346 frames del trailer animado
│   ├── white_noise1–8.bmp            # Frames del efecto de ruido estático
│   ├── star_8x8.bmp, star_16x16.bmp, usage_bar.bmp
│   └── fuentes/                      # fixed 8×8/8×16/32×64, variable 8×8/8×16/16×16
│                                     # (azul, rojo, amarillo)
│
└── audio/                            # Módulos Impulse Tracker (.it) para MaxMod
    ├── felip_games_studio.it         # Jingle de intro del estudio
    ├── fnaf_intro.it                 # Música de la noche personalizada
    ├── darkness.it                   # Menú principal
    ├── ambience2.it                  # Ambiente de la oficina
    └── gracefully_into_the_abyss.it  # Preparada, sin asignar
```

---

## Mecánicas implementadas

### Animatrónicos

Cada animatrónico tiene clase propia, RNG de seed único y temporizadores independientes. Los valores reproducen los del original.

**Bonnie** — ruta izquierda: Show Stage → Backstage → Dining Room → West Hall → Supply Closet → Left Door.  
Intervalo de movimiento: 298 frames (~4.97 s). Si llega a la puerta abierta, tiene entre 5 y 15 s para atacar (según nivel de IA) antes de causar Game Over, dando margen al jugador para cerrar.

**Chica** — ruta derecha: Show Stage → Dining Room → Restrooms → Kitchen *(solo audio)* → East Hall → East Corner → Right Door.  
Intervalo: 299 frames. Misma lógica de attack timer que Bonnie.

**Foxy** — sistema de fases de cortina en Pirate Cove: `CURTAIN_CLOSED → PEEKING_1 → PEEKING_2 → CURTAIN_OPEN`. Avanza de fase cada 720 frames (12 s) si el jugador **no** lo está mirando. Al llegar a CURTAIN_OPEN hace un sprint de 90 frames hasta la puerta izquierda. Si la puerta está cerrada al llegar, drena `5 + ai_level/2` % de energía y regresa a Cove. Si está abierta → Game Over.

**Freddy** — ruta derecha con la misma secuencia que Chica, pero **solo se mueve si no está siendo observado** en la cámara donde se encuentra. Intervalo: 181 frames (~3.02 s), el más rápido de todos. Modo **Power Out**: cuando la energía llega a 0, sortea un delay aleatorio de 600–1200 frames (10–20 s) y luego causa Game Over independientemente de las puertas.

### Sistema de energía

Cada noche tiene una tasa de drenaje base diferente, multiplicada por el número de elementos activos (puertas cerradas, luces encendidas, cámaras abiertas). Todo implementado en enteros para evitar punto flotante en GBA.

**Drenaje base por noche** (frames entre cada tick de −1%):

| Noche | Frames/tick | Segundos equivalentes |
|-------|-------------|----------------------|
| 1 | 578 | ~9.6 s |
| 2 | 480 | 8.0 s |
| 3 | 432 | 7.2 s |
| 4–6 | 384 | 6.4 s |

**Multiplicadores por items activos:**

| Items activos | Multiplicador |
|---------------|---------------|
| 0 | ×1.0 |
| 1 | ×1.6 |
| 2 | ×2.8 |
| 3 | ×4.6 |
| 4+ | ×8.0 |

### Tabla de IA por noche

| Noche | Freddy | Bonnie | Chica | Foxy |
|-------|--------|--------|-------|------|
| 1 | 0 | 0 | 0 | 0 |
| 2 | 0 | 3 | 1 | 1 |
| 3 | 1 | 0 | 5 | 2 |
| 4 | 1 | 2 | 4 | 6 |
| 5 | 3 | 5 | 7 | 5 |
| 6 | 4 | 10 | 12 | 16 |

### Guardado (SRAM)

Escritura directa byte a byte en SRAM GBA (`0x0E000000`). Estructura de 4 bytes: `max_night` (0–6), `stars` (0–3), `flags` (`FLAG_HAS_SAVE`, `FLAG_CUSTOM_UNLOCK`) y `checksum` (XOR de los tres campos anteriores). Al iniciar, si el checksum no coincide, se considera corrupto y se sobreescribe con un save limpio.

---

## Dependencias y compilación

- **Framework:** [Butano](https://github.com/GValiente/butano) — headers `bn_*`
- **Toolchain:** devkitARM (`arm-none-eabi-g++`) — parte de [devkitPro](https://devkitpro.org/)
- **Audio:** MaxMod (integrado en Butano), módulos `.it` de Impulse Tracker
- **Build:** Makefile estándar de Butano

```bash
# Requiere devkitPro instalado y $DEVKITPRO apuntando a la instalación
export DEVKITPRO=/opt/devkitpro   # ajusta según tu ruta

make

# Salida: FNAF.gba
```

El `.gba` generado se puede ejecutar en [mGBA](https://mgba.io/) (recomendado para desarrollo) o en hardware real con un flashcart.

> **Nota:** Los archivos `graphics/*.bmp` y `audio/*.it` **no están en el repositorio** por tamaño. Consulta al equipo para obtenerlos.

---

## Próximos pasos (WIP prioritario)

1. **Sistema de cámaras** — implementar `CameraScreen` y `camera_map.h`: renderizar el fondo de cada cámara y mostrar la posición de los animatrónicos según `ctx.bonnie.camera`, `ctx.chica.camera`, etc. Cam 1C (Pirate Cove) debe leer `foxy.phase()` para mostrar el estado de la cortina.

2. **Puertas y luces renderizadas** — completar `Doors` y `Lights`: superponer el sprite de puerta cerrada y la imagen de luz con/sin animatrónico usando `ctx.bonnie.at_left_door`, `ctx.chica.at_right_door` y el estado de `left_light_on` / `right_light_on`.

3. **Audio Manager** — capa de abstracción sobre MaxMod para transiciones de música (Toreador March durante Power Out) y efectos de sonido puntuales (jumpscare, puerta, luz).

4. **Golden Freddy** — aparición en Cam 1A bajo condiciones especiales en Noche 6 o Custom: si el jugador baja el monitor sin cerrar la vista, jumpscare instantáneo.

5. **Sistema de progresión** — registrar estrellas al completar noches y desbloquear la Custom Night escribiendo `FLAG_CUSTOM_UNLOCK` en el save.

6. **Cola de eventos** — desacoplar la comunicación entre animatrónicos, juego y audio mediante `event_queue.h` en lugar de lecturas directas del contexto.

---

## Créditos

Desarrollado por **Felip Games Studio**.

*Five Nights at Freddy's* es propiedad de Scott Cawthon / Steel Wool Studios. Este es un proyecto de fan sin fines comerciales, hecho con fines educativos y de entretenimiento personal.
