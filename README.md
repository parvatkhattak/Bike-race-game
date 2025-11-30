# 🎮 3D Bike Race Game

[![C++](https://img.shields.io/badge/C++-17-blue.svg)](https://isocpp.org/)
[![Raylib](https://img.shields.io/badge/Raylib-5.0-red.svg)](https://www.raylib.com/)
[![License](https://img.shields.io/badge/License-Educational-green.svg)]()
[![Build](https://img.shields.io/badge/Build-CMake-orange.svg)](https://cmake.org/)

A competitive 3D bike racing game built with **C++17** and **Raylib**, featuring an AI opponent, physics-based gameplay, progressive difficulty, and unlockable upgrades.

![Game Status](https://img.shields.io/badge/Status-Playable-success)
![Progress](https://img.shields.io/badge/Progress-Complete-brightgreen)

---

## 🎯 Features

✅ **AI Opponent** - Race against an intelligent CPU opponent with adaptive driving  
✅ **Physics-Based Movement** - Realistic acceleration, friction, and collision  
✅ **3 Racing Tracks** - Beginner, Intermediate, and Advanced difficulty levels  
✅ **Progressive Challenge** - Increasingly complex tracks with dynamic obstacles  
✅ **Checkpoint System** - Lap counting with sequential checkpoint validation  
✅ **Upgrade System** - Unlock speed boosts, better handling, and nitro  
✅ **Complete UI** - Menus, HUD with speedometer, lap timer, and position tracking  
✅ **Gamepad Support** - Full keyboard and gamepad controls  

---

## 🚀 Quick Start

### Single Command to Run
```bash
bash quick_build.sh
```

This command will:
- Download and build Raylib 5.0
- Compile the game
- Launch the game automatically

**First time setup?** Clone the repository first:
```bash
git clone https://github.com/parvatkhattak/Bike-race-game.git
cd Bike-race-game
bash quick_build.sh
```

### If Already Built - Just Run the Game
```bash
cd build
./bin/BikeRaceGame
```

See **[QUICKSTART.md](QUICKSTART.md)** for detailed instructions.

---

## 🎮 Controls

### Player (Red Bike)
- **W** - Accelerate
- **S** - Brake/Reverse
- **A** - Turn Left
- **D** - Turn Right
- **Left Shift** - Nitro Boost

### AI Opponent (Blue Bike)
- Controlled automatically by the game AI
- Follows checkpoints and adjusts speed dynamically
- Uses nitro strategically on straightaways

### General
- **ESC** - Pause/Menu
- **ENTER** - Confirm
- **Arrow Keys** - Navigate Menus

---

## 🏗️ Architecture

### Core Systems
- **GameEngine** - Main game loop, state management, subsystem coordination
- **PhysicsEngine** - Gravity, friction, drag, collision detection/response
- **InputManager** - Keyboard & gamepad handling with AI input integration
- **CameraManager** - Follow camera with smooth interpolation
- **UIManager** - Complete menu system and in-game HUD
- **LevelManager** - Race lifecycle, checkpoint tracking, position calculation
- **AudioManager** - Music streaming and sound effects (ready for assets)

### Entities
- **Bike** - Physics-based vehicle with upgrade system
- **Player** - Race tracking, statistics, input processing, and AI control logic
- **Track** - Procedural track generation with 3 difficulty levels
- **Checkpoint** - Lap counting with sphere-based detection
- **Obstacle** - 3 solid types: Static Barriers, Moving Platforms, Ramps

---

## 📦 Project Structure

```
Bike-race-game/
├── CMakeLists.txt              # Build configuration
├── README.md                   # This file
├── QUICKSTART.md               # Installation guide
├── src/                        # Source code (~2000+ lines)
│   ├── main.cpp
│   ├── core/                   # Game engine, input, camera
│   ├── entities/               # Player, bike
│   ├── physics/                # Physics simulation
│   ├── level/                  # Tracks, checkpoints, obstacles
│   ├── ui/                     # User interface
│   └── systems/                # Level manager, audio
├── include/                    # Header files
└── assets/                     # Game assets (models, audio, textures)
```

---

## 🛠️ Build Requirements

- **C++17** compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)
- **CMake 3.20+**
- **Raylib 5.0** (auto-downloaded by CMake)
- **OpenGL 3.3+** support

### Platform-Specific Dependencies

**Ubuntu/Debian:**
```bash
sudo apt install cmake build-essential libasound2-dev libx11-dev libgl1-mesa-dev
```

**macOS:**
```bash
brew install cmake
```

**Windows:**
- Visual Studio 2019+ or MinGW-w64
- CMake from [cmake.org](https://cmake.org/download/)

---

## 🎓 Technical Highlights

### Object-Oriented Design
- **Design Patterns**: Singleton (GameEngine, Logger), Composition, Polymorphism
- **Memory Management**: `std::unique_ptr` for automatic resource cleanup
- **Modern C++**: Lambda functions, range-based loops, STL containers

### Game Development Concepts
- **Fixed Timestep Game Loop** (60 FPS)
- **State Machine** for game states
- **Component-Based Architecture**
- **Event-Driven UI** with callbacks
- **Physics Simulation**: Euler integration, impulse-based collision

### Code Quality
- **~2000+ lines** of organized C++ code
- **15+ classes** following SOLID principles
- **Modular subsystems** with clear interfaces
- **Comprehensive documentation**

---

## 🎯 Game Modes

### 1. Beginner Circuit
- **Difficulty**: Easy
- **Layout**: Simple oval track
- **Checkpoints**: 5
- **Obstacles**: Few static barriers
- **Perfect for**: Learning controls

### 2. Intermediate Track
- **Difficulty**: Medium
- **Layout**: Complex hexagon
- **Checkpoints**: 6
- **Obstacles**: Moving platforms, speed boost zones
- **Perfect for**: Improving skills

### 3. Advanced Track
- **Difficulty**: Hard
- **Layout**: Figure-8 with sharp turns
- **Checkpoints**: 7
- **Obstacles**: Many hazards, slow zones
- **Perfect for**: Expert racers

---

## 📈 Progression System

- 🏆 **Complete races** to unlock harder tracks
- ⭐ **Earn points** based on position (100 for 1st, 50 for 2nd)
- 🚀 **Track best lap times**
- 🔓 **Unlock upgrades**: Speed, Acceleration, Handling, Nitro

---

## 🔮 Future Enhancements

- [x] AI opponents
- [ ] Multiple AI difficulty levels
- [ ] Online multiplayer
- [ ] Custom 3D bike models
- [ ] Particle effects (dust, sparks)
- [ ] Weather effects (rain, fog)
- [ ] Power-ups and boost zones
- [ ] Replay system
- [ ] Save/load progress
- [ ] Global leaderboards

---

## 🤝 Contributing

This project is an educational demonstration of OOP principles in C++. Contributions and suggestions are welcome!

1. Fork the repository
2. Create a feature branch
3. Commit your changes
4. Push to the branch
5. Open a Pull Request

---

## 📝 License

This project is created for **educational purposes** demonstrating:
- C++ Object-Oriented Programming
- Game engine architecture
- 3D graphics programming with Raylib
- Physics simulation
- Game design principles

---

## 🙏 Acknowledgments

- **[Raylib](https://www.raylib.com/)** - Amazing game development library
- **C++ Community** - For excellent documentation and resources
- **Game Development Patterns** - Inspired by industry best practices

---

## 📧 Contact

**Developer**: Parvat Khattak  
**Repository**: [github.com/parvatkhattak/Bike-race-game](https://github.com/parvatkhattak/Bike-race-game)  
**Issues**: [Report bugs or request features](https://github.com/parvatkhattak/Bike-race-game/issues)

---

<div align="center">

**Made with ❤️ using C++ and Raylib**

⭐ Star this repo if you found it helpful!

</div>
