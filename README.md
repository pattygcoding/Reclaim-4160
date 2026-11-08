# Reclaim-4160

A 2D side-scrolling game about a pandemic-era grocery run: grab the last pack of
toilet paper and make it to the checkout before the store (and your health) runs out.

Built in **C++ with SDL2** as a class project for **CPSC 4160 – 2D Game Engine
Construction**. It is a prototype built on a hand-rolled game engine (no external
engine like Unity/Godot) — everything from the game loop to collision and animation
lives in this repository.

> **Status: work-in-progress prototype.** The gameplay described below reflects the
> current build. Some systems are complete, some are stubbed. See
> [What it's supposed to be](#what-its-supposed-to-be) for the honest state of things.

---

## The idea

You play a shopper in a store aisle during a supply shortage. Your goal is to
**collect the pack of toilet paper** and **reach the checkout counter** without being
worn down by the "yellow" enemies roaming the aisle. Hand sanitizer pickups along the
way heal you, and shipping boxes / shopping carts can be climbed or pushed to get
around.

The repository is named **Reclaim-4160** — "reclaim" as in reclaiming the toilet paper.
The legacy window title still reads *"Astrorun"* from an earlier iteration.

---

## How to play

Reach the pack of toilet paper (TP), then touch the **checkout counter** while holding
it to **win**. If your **health hits 0** you get a **Game Over** screen.

### Controls

| Action | Key |
| --- | --- |
| Move left / right | `A` / `D` |
| Jump | `Space` |
| Drop down (fall off a ledge you're standing on) | `S` |
| Pause / back out of a screen | `Esc` |
| Navigate menus | `W` / `S` |
| Confirm menu option | `Space` |

### Game screens

- **Title screen** – *Start*, *How to Play*, *Quit*
- **How to Play** – on-screen instructions page
- **In-game HUD** – live health label + value (rendered with SDL2_ttf)
- **Pause menu** – *Unpause*, *Return to Home*
- **Game Over** – *Try Again*, *Return to Home*
- **You Win** – *Play Again*, *Return to Home*

---

## Features (current build)

- **Game loop** – fixed ~60 FPS loop with FPS limiting and an optional terminal FPS counter.
- **Scrolling camera** – the store background is 2000 px wide; the camera follows the
  player across a 680×480 window.
- **Animated player** – sprite-sheet animation driven by a state machine
  (`IDLE`, `MOVE_LEFT`, `MOVE_RIGHT`, `JUMP`, `FALL`), plus a separate "damaged" sprite
  shown briefly when hit.
- **Enemy AI (finite state machine)** – enemies idle, aggro when the player is in view,
  and chase left/right, dealing damage on contact.
- **Pickups** – collectible toilet paper and hand-sanitizer health items
  (+health on pickup, with a particle sparkle FX and a pickup sound).
- **Physics / platforms** – gravity, jumping, standing on top of objects, and drop-through.
- **Pushable objects** – shopping carts that respond to being pushed.
- **UI & menus** – title, pause, game-over, and win screens built from TTF text and a
  selection-cursor sprite.
- **Audio** – separate music tracks for menu / gameplay / game over / victory, and SFX.

---

## What it's supposed to be

For now this is a **compact, self-contained demonstration of a 2D game engine** written
from scratch, wrapped in a small "survive the aisle and get the toilet paper" game. It
shows off the core pieces you'd expect from a course-built engine: an object hierarchy
(`GameObject` → `Player`, `Enemy`, `PushableObj`, `HealthObj`, `ToiletPaper`, …),
sprite animation, box collisions, camera/background scrolling, particle effects, text
rendering, and state-driven menus.

It is **not** a finished, packaged game yet. Notably:

- **Art, audio, and font assets are not included in the repository** — only source code
  and a leftover `pi.bmp` are committed. The game will not run until the asset folders
  listed in [Required assets](#required-assets) are provided.
- Balance, level content, and enemy behavior are prototype-level and may change.
- `GameEngine.hpp` / `GameObject.hpp` are leftovers from an earlier assignment and are
  not part of the active build (the active headers are `GameEngine.h` / `GameObject.h`).

---

## Requirements

- A C++ compiler (`g++`) with **C++14** support
- [SDL2](https://www.libsdl.org/) development libraries:
  - `SDL2`
  - `SDL2_image` (PNG loading)
  - `SDL2_ttf` (TrueType fonts)
  - `SDL2_mixer` (music / SFX)

The provided `Makefile` uses `sdl2-config`. It is written for **macOS / Linux**.
On Windows, build from **MSYS2/MinGW** (or WSL) with the SDL2 packages installed.

---

## Build & run

Using the included Makefile:

```sh
# compile
make clean && make

# run the game
make run          # or ./astrorun
```

The build produces the executable **`astrorun`**.

---

## Required assets

The code loads assets at runtime from `img/`, `sounds/`, and `fonts/` folders that are
**not committed**. To run the game, add files with these names:

**`img/`**
`background.png`, `player.png`, `player_damaged.png`, `enemy.png`,
`shoppingcart.png`, `boxes.png`, `box.png`, `checkout.png`, `tp.png`,
`sanitizer.png`, `healthsparkle.png`, `selector.png`, `title.png`, `paused.png`,
`gameover.png`, `youwin.png`, `instructions.png`

**`sounds/`**
`Toilet_Paper_Waltz_Final.wav`, `TPP_3rd_Draft_Final.wav`, `Game_Over.wav`,
`Victory.wav`, `Health_Get.wav`

**`fonts/`**
`arial.ttf`

---

## Project layout

```
main.cpp            Entry point + game loop (FPS limiter)
GameEngine.*        Singleton engine: window, renderer, events, update, render,
                    camera, collisions, menus, music, win/game-over logic
GameObject.*        Base object: sprite, box collider, movement, render, collisions
Player.*            Player state machine, jumping/gravity, health, damaged sprite
Enemy.*             Enemy FSM (idle / move / aggro) and contact damage
PushableObj.*       Shopping carts that can be pushed
HealthObj.*         Sanitizer pickups (+health) with particle FX + SFX
ToiletPaper.*       The objective item (collect -> win condition)
Particle / ParticleEmitter.*   Particle sparkle effects
Sprite.*            Texture loading, source/screen rects, animation frames
Text.*              SDL2_ttf text rendering
MenuOptions.*       Menu option list + selection cursor
Background.*        Scrolling background / camera math
Makefile            Build rules for the SDL2 project (output: astrorun)
```

---

## Credits

Originally built for **CPSC 4160 – 2D Game Engine Construction**.

- **Team (CPSC 4160, Assignment 5):** Nikita Tran, Cameron Larson, Jason Lyle, Quinn Hubbarth
- **Engine / gameplay code:** Patrick Goodwin (Dr. Victor Zordan's section)
- **Art:** hand-drawn by Nikita Tran; hand-sanitizer sprite from
  [clipartpng.com](https://clipartpng.com/?3267,green-hand-sanitizer-png-clipart)
- **Music & SFX:** Cameron Larson
