# Details

Date : 2025-03-08 13:50:00

Directory c:\\Users\\twinkboy42\\Projects\\bounce-tales

Total : 66 files,  3869 codes, 168 comments, 659 blanks, all 4696 lines

[Summary](results.md) / Details / [Diff Summary](diff.md) / [Diff Details](diff-details.md)

## Files
| filename | language | code | comment | blank | total |
| :--- | :--- | ---: | ---: | ---: | ---: |
| [CMakeLists.txt](/CMakeLists.txt) | CMake | 48 | 0 | 11 | 59 |
| [assets/textures/jump\_pad.json](/assets/textures/jump_pad.json) | JSON | 7 | 0 | 0 | 7 |
| [assets/textures/particles.json](/assets/textures/particles.json) | JSON | 25 | 0 | 0 | 25 |
| [assets/textures/redy\_emotions.json](/assets/textures/redy_emotions.json) | JSON | 7 | 0 | 0 | 7 |
| [extra/levels/debug.json](/extra/levels/debug.json) | JSON | 69 | 0 | 0 | 69 |
| [extra/settings.ini](/extra/settings.ini) | Ini | 0 | 0 | 1 | 1 |
| [scripts/curve.py](/scripts/curve.py) | Python | 146 | 32 | 26 | 204 |
| [scripts/space.py](/scripts/space.py) | Python | 24 | 4 | 9 | 37 |
| [src/components/collidable.cpp](/src/components/collidable.cpp) | C++ | 33 | 0 | 6 | 39 |
| [src/components/physical.cpp](/src/components/physical.cpp) | C++ | 57 | 16 | 14 | 87 |
| [src/components/sound\_player.cpp](/src/components/sound_player.cpp) | C++ | 51 | 0 | 7 | 58 |
| [src/core/asset\_manager.cpp](/src/core/asset_manager.cpp) | C++ | 133 | 2 | 17 | 152 |
| [src/core/game.cpp](/src/core/game.cpp) | C++ | 87 | 1 | 11 | 99 |
| [src/core/music\_player.cpp](/src/core/music_player.cpp) | C++ | 44 | 0 | 9 | 53 |
| [src/core/object\_manager.cpp](/src/core/object_manager.cpp) | C++ | 219 | 28 | 27 | 274 |
| [src/core/settings.cpp](/src/core/settings.cpp) | C++ | 112 | 0 | 16 | 128 |
| [src/drawables/animation.cpp](/src/drawables/animation.cpp) | C++ | 22 | 0 | 3 | 25 |
| [src/drawables/curved\_shape.cpp](/src/drawables/curved_shape.cpp) | C++ | 41 | 0 | 6 | 47 |
| [src/drawables/primitive\_sprite.cpp](/src/drawables/primitive_sprite.cpp) | C++ | 55 | 0 | 10 | 65 |
| [src/extra/collision\_calculator.cpp](/src/extra/collision_calculator.cpp) | C++ | 180 | 4 | 33 | 217 |
| [src/extra/collision\_math.cpp](/src/extra/collision_math.cpp) | C++ | 171 | 0 | 24 | 195 |
| [src/extra/earcut.cpp](/src/extra/earcut.cpp) | C++ | 57 | 0 | 4 | 61 |
| [src/extra/randomizer.cpp](/src/extra/randomizer.cpp) | C++ | 13 | 0 | 4 | 17 |
| [src/extra/timer.cpp](/src/extra/timer.cpp) | C++ | 40 | 0 | 8 | 48 |
| [src/include/animation.hpp](/src/include/animation.hpp) | C++ | 22 | 0 | 6 | 28 |
| [src/include/asset\_manager.hpp](/src/include/asset_manager.hpp) | C++ | 31 | 0 | 9 | 40 |
| [src/include/background.hpp](/src/include/background.hpp) | C++ | 12 | 0 | 3 | 15 |
| [src/include/background\_object.hpp](/src/include/background_object.hpp) | C++ | 16 | 0 | 4 | 20 |
| [src/include/camera.hpp](/src/include/camera.hpp) | C++ | 16 | 0 | 5 | 21 |
| [src/include/collidable.hpp](/src/include/collidable.hpp) | C++ | 42 | 0 | 11 | 53 |
| [src/include/collision\_calculator.hpp](/src/include/collision_calculator.hpp) | C++ | 19 | 0 | 3 | 22 |
| [src/include/collision\_math.hpp](/src/include/collision_math.hpp) | C++ | 41 | 0 | 9 | 50 |
| [src/include/curved\_shape.hpp](/src/include/curved_shape.hpp) | C++ | 17 | 0 | 5 | 22 |
| [src/include/earcut.hpp](/src/include/earcut.hpp) | C++ | 741 | 66 | 145 | 952 |
| [src/include/game.hpp](/src/include/game.hpp) | C++ | 36 | 0 | 9 | 45 |
| [src/include/game\_object.hpp](/src/include/game_object.hpp) | C++ | 77 | 1 | 13 | 91 |
| [src/include/ground.hpp](/src/include/ground.hpp) | C++ | 12 | 0 | 4 | 16 |
| [src/include/gui\_manager.hpp](/src/include/gui_manager.hpp) | C++ | 29 | 1 | 5 | 35 |
| [src/include/jump\_pad.hpp](/src/include/jump_pad.hpp) | C++ | 23 | 0 | 7 | 30 |
| [src/include/music\_player.hpp](/src/include/music_player.hpp) | C++ | 19 | 0 | 8 | 27 |
| [src/include/object\_manager.hpp](/src/include/object_manager.hpp) | C++ | 34 | 0 | 7 | 41 |
| [src/include/particle.hpp](/src/include/particle.hpp) | C++ | 22 | 0 | 6 | 28 |
| [src/include/physical.hpp](/src/include/physical.hpp) | C++ | 46 | 0 | 8 | 54 |
| [src/include/platform.hpp](/src/include/platform.hpp) | C++ | 16 | 0 | 5 | 21 |
| [src/include/player.hpp](/src/include/player.hpp) | C++ | 22 | 1 | 7 | 30 |
| [src/include/primitive\_sprite.hpp](/src/include/primitive_sprite.hpp) | C++ | 21 | 0 | 10 | 31 |
| [src/include/randomizer.hpp](/src/include/randomizer.hpp) | C++ | 6 | 0 | 1 | 7 |
| [src/include/settings.hpp](/src/include/settings.hpp) | C++ | 29 | 0 | 8 | 37 |
| [src/include/simple\_object.hpp](/src/include/simple_object.hpp) | C++ | 13 | 0 | 3 | 16 |
| [src/include/sound\_player.hpp](/src/include/sound_player.hpp) | C++ | 27 | 0 | 6 | 33 |
| [src/include/spikes.hpp](/src/include/spikes.hpp) | C++ | 12 | 0 | 5 | 17 |
| [src/include/timer.hpp](/src/include/timer.hpp) | C++ | 21 | 0 | 9 | 30 |
| [src/include/transformable.hpp](/src/include/transformable.hpp) | C++ | 25 | 0 | 2 | 27 |
| [src/include/trigger\_object.hpp](/src/include/trigger_object.hpp) | C++ | 14 | 0 | 5 | 19 |
| [src/objects/background.cpp](/src/objects/background.cpp) | C++ | 57 | 4 | 8 | 69 |
| [src/objects/background\_object.cpp](/src/objects/background_object.cpp) | C++ | 33 | 0 | 4 | 37 |
| [src/objects/camera.cpp](/src/objects/camera.cpp) | C++ | 22 | 0 | 5 | 27 |
| [src/objects/game\_object.cpp](/src/objects/game_object.cpp) | C++ | 261 | 3 | 25 | 289 |
| [src/objects/ground.cpp](/src/objects/ground.cpp) | C++ | 17 | 0 | 3 | 20 |
| [src/objects/jump\_pad.cpp](/src/objects/jump_pad.cpp) | C++ | 76 | 1 | 7 | 84 |
| [src/objects/particle.cpp](/src/objects/particle.cpp) | C++ | 105 | 0 | 12 | 117 |
| [src/objects/platform.cpp](/src/objects/platform.cpp) | C++ | 37 | 2 | 6 | 45 |
| [src/objects/player.cpp](/src/objects/player.cpp) | C++ | 101 | 2 | 5 | 108 |
| [src/objects/simple\_object.cpp](/src/objects/simple_object.cpp) | C++ | 31 | 0 | 5 | 36 |
| [src/objects/spikes.cpp](/src/objects/spikes.cpp) | C++ | 20 | 0 | 4 | 24 |
| [src/objects/trigger\_object.cpp](/src/objects/trigger_object.cpp) | C++ | 7 | 0 | 1 | 8 |

[Summary](results.md) / Details / [Diff Summary](diff.md) / [Diff Details](diff-details.md)