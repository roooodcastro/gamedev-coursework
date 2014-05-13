# Computer Games Development Coursework

#### Simple 3D game developed for a university coursework

### TODO List:

* Fix the mouse positioning for rotated interface items
* Implement joystick events
* Refactor ship's input handling to allow control via both keyboard and joystick
* Create a configuration screen
* Implement upgrades or powerups
* Refactor all interface positioning and scaling to allow multiple screen resolutions


### Done:

* SDL Integration with OpenGL
* Separate framerate control for rendering and logic
* State (Level) driven game flow
* 2D User Interface class
* Interface items: Image, Panel, Button, Text
* Basic profiling: FPS, entities, resources counter and mouse position
* Wavefront object file (.obj) loader
* Keyboard and mouse handling
* Basic shader pipeline
* Multiple sources lighting model
* Generic resource manager
* Resources being managed: Models, Textures, Sound
* Physics engine with Verlet integration
* Collision model using World Partitioning broadphase detector
* Sound effects loader and player, supports .wav files
* Music loader and player, supports most mainstream audio file formats
* Configuration manager to save and load game configurations from the disk
* Track loading from fixed templates saved on disk
* Ship smooth movement with WASD keys
* Splash, Menu, Leaderboard and Help screens
* Pause Menu and Game Over menu
* Complete game