# 🏍️ Bike Race Game

A competitive 3D two-player bike racing game built with C++ and Raylib, featuring physics-based gameplay, progressive difficulty, and unlockable upgrades.

## 🎮 Features

- **Two-Player Local Multiplayer** - Race head-to-head with split-screen action
- **Physics-Based Movement** - Realistic acceleration, friction, and collision
- **Multiple Tracks** - 3+ tracks with increasing difficulty
- **Obstacle Course** - Navigate barriers, ramps, and dynamic hazards
- **Upgrade System** - Unlock speed boosts, better handling, and cosmetic upgrades
- **Progressive Difficulty** - From beginner-friendly to expert challenges
- **Lap System** - Checkpoint-based lap counting and race timing
- **Immersive 3D Graphics** - Modern 3D rendering with Raylib

## 🛠️ Technologies

- **Language**: C++17
- **Graphics**: Raylib 5.0
- **Build System**: CMake 3.20+
- **Version Control**: Git

## 📋 Requirements

- C++17 compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)
- CMake 3.20 or higher
- OpenGL 3.3+ support
- Git (for cloning)

### Platform-Specific Requirements

**Linux:**
```bash
sudo apt-get install build-essential cmake git
sudo apt-get install libasound2-dev mesa-common-dev libx11-dev libxrandr-dev libxi-dev xorg-dev libgl1-mesa-dev libglu1-mesa-dev
```

**macOS:**
```bash
brew install cmake
```

**Windows:**
- Visual Studio 2017+ or MinGW-w64
- CMake (download from cmake.org)

## 🚀 Building & Running

### Clone the Repository
```bash
git clone <repository-url>
cd Bike-race-game
```

### Build the Project
```bash
mkdir build
cd build
cmake ..
make -j$(nproc)
```

### Run the Game
```bash
./bin/BikeRaceGame
```

Or on Windows:
```bash
.\bin\BikeRaceGame.exe
```

## 🎯 Controls

### Player 1
- **W** - Accelerate
- **S** - Brake/Reverse
- **A** - Turn Left
- **D** - Turn Right
- **Left Shift** - Nitro Boost (when unlocked)

### Player 2
- **↑** - Accelerate
- **↓** - Brake/Reverse
- **←** - Turn Left
- **→** - Turn Right
- **Right Shift** - Nitro Boost (when unlocked)

### General
- **ESC** - Pause/Menu
- **F11** - Toggle Fullscreen

## 📁 Project Structure

```
Bike-race-game/
├── CMakeLists.txt          # Build configuration
├── README.md               # This file
├── src/                    # Source files
│   ├── main.cpp           # Entry point
│   ├── core/              # Game engine core
│   ├── entities/          # Player, Bike, etc.
│   ├── physics/           # Physics engine
│   ├── level/             # Track, obstacles
│   ├── ui/                # User interface
│   ├── systems/           # Game systems
│   └── utils/             # Utilities
├── include/                # Header files
├── assets/                 # Game assets
│   ├── models/            # 3D models
│   ├── textures/          # Textures
│   ├── audio/             # Sound & music
│   └── data/              # Game data
└── external/               # External libraries
```

## 🎓 Development Phases

1. ✅ Project Setup & Architecture
2. 🚧 Core Game Engine
3. ⏳ Player & Bike System
4. ⏳ Physics System
5. ⏳ Track & Environment
6. ⏳ UI System
7. ⏳ Gameplay Features
8. ⏳ Visual & Audio Polish
9. ⏳ Save System & Progression
10. ⏳ Testing & Optimization

## 🤝 Contributing

This is an educational project demonstrating Object-Oriented Programming principles in C++. Contributions and suggestions are welcome!

## 📝 License

This project is created for educational purposes.

## 🎯 Future Enhancements

- AI opponents
- Online multiplayer
- Weather effects (rain, fog)
- Power-ups and boost zones
- Replay system
- Global leaderboards
- VR support

## 📧 Contact

For questions or feedback, please open an issue on the repository.

---

**Made with ❤️ using C++ and Raylib**
