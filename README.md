# �️ Bike Race Game

<div align="center">

![C++](https://img.shields.io/badge/C++-17-blue.svg)
![Raylib](https://img.shields.io/badge/Raylib-5.0-red.svg)
![License](https://img.shields.io/badge/License-Educational-green.svg)
![Platform](https://img.shields.io/badge/Platform-Linux%20%7C%20macOS%20%7C%20Windows-lightgrey.svg)

**A high-performance 3D motorcycle racing game built with C++ and Raylib**

*Featuring AI opponents, bike selection, difficulty scaling, and competitive racing!*

</div>

---

## 🎮 Features

### Core Gameplay
- **🏁 Competitive Racing**: Race against 4 AI opponents on 3 different tracks
- **🏍️ Bike Selection**: Choose between Red or Blue bike at race start
- **🎯 Arrow Key Controls**: Your selected bike is always controlled by arrow keys
- **⚡ Nitro Boost System**: Strategic speed boosts for overtaking
- **� 3 Difficulty Levels**: Easy, Medium, and Hard tracks with scaling challenge

### AI & Difficulty
- **🤖 Intelligent AI**: CPU opponents race competitively toward checkpoints
- **📈 Difficulty Scaling**: 
  - **Level 1 (Easy)**: 90% AI power, 3 obstacles, gentler competition
  - **Level 2 (Medium)**: 105% AI power, 6 obstacles, balanced challenge
  - **Level 3 (Hard)**: 120% AI power, 12 obstacles, maximum challenge
- **🎲 Randomized Starts**: Starting positions shuffled each race for fairness

### Physics & Controls
- **🔄 Smooth Physics**: Custom physics engine with gravity, friction, and drag
- **💥 Dynamic Collisions**: Bike-to-bike and bike-to-obstacle interactions
- **🎛️ Responsive Controls**: Optimized turn rate, acceleration, and braking
- **🚀 Reduced Collision Sensitivity**: Stable, non-bouncy collisions

### Visual & UI
- **🎨 Dark Gradient UI**: Professional, consistent interface throughout
- **📊 In-Game HUD**: Real-time speed, position, lap times, and nitro meter
- **🏆 Results Screen**: Detailed race results and points awarded
- **🎯 Clear Checkpoints**: Visual checkpoint markers to guide racing

---

## 🚀 Quick Start

### Prerequisites

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

### Build & Run

#### First Time Setup (Recommended)
```bash
# Clone the repository
git clone https://github.com/parvatkhattak/Bike-race-game.git
cd Bike-race-game

# Quick build script (builds and runs)
chmod +x quick_build.sh
./quick_build.sh
```

#### Manual Build
```bash
# Create build directory
mkdir -p build && cd build

# Configure and build
cmake ..
cmake --build .

# Run the game
./BikeRaceGame
```

#### If Already Built - Just Run the Game
```bash
cd Bike-race-game
./build/BikeRaceGame
```

---

## 🎮 Controls

### Menu Navigation
- **↑↓ Arrow Keys** - Navigate menus
- **ENTER** - Confirm selection
- **ESC** - Pause/Back to menu

### Racing Controls
- **↑ Arrow** - Accelerate
- **↓ Arrow** - Brake/Reverse
- **← Arrow** - Turn Left
- **→ Arrow** - Turn Right
- **Left Shift** - Nitro Boost
- **ESC** - Pause Race

> **Note:** Your selected bike (red or blue) is always controlled by arrow keys!

### AI Opponents
- Controlled automatically by intelligent AI
- Follow checkpoints and race competitively
- Use nitro strategically
- Difficulty scales with track level

---

## 🏗️ Architecture

### Core Systems
- **GameEngine** - Main loop, state management, subsystem coordination
- **PhysicsEngine** - Gravity, friction, drag, collision detection/response
- **InputManager** - Keyboard & gamepad handling
- **CameraManager** - Follow camera with smooth interpolation
- **UIManager** - Menu system, bike selection, and in-game HUD
- **LevelManager** - Race lifecycle, checkpoint tracking, position calculation
- **AudioManager** - Music streaming and sound effects (ready for assets)

### Entities
- **Bike** - Physics-based vehicle with custom controls
- **Player** - Race tracking, statistics, and AI control logic
- **Track** - Procedural track generation with 3 difficulty levels
- **Checkpoint** - Lap counting with sphere-based detection
- **Obstacle** - Static barriers, moving platforms, and ramps

---

## 📦 Project Structure

```
Bike-race-game/
├── CMakeLists.txt              # Build configuration
├── README.md                   # This file
├── QUICKSTART.md               # Quick installation guide
├── quick_build.sh              # One-command build script
├── src/                        # Source code (~2500+ lines)
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

## 🎯 Game Modes

### 1. Beginner Circuit (Level 1)
- **Difficulty**: Easy
- **AI Strength**: 90% power
- **Obstacles**: 3 (2 barriers, 1 ramp)
- **Perfect for**: Learning controls and mechanics

### 2. Intermediate Track (Level 2)
- **Difficulty**: Medium
- **AI Strength**: 105% power
- **Obstacles**: 6 (4 barriers, 1 moving platform, 1 ramp)
- **Perfect for**: Competitive racing

### 3. Advanced Track (Level 3)
- **Difficulty**: Hard
- **AI Strength**: 120% power
- **Obstacles**: 12 (10 barriers, 1 moving platform, 1 ramp)
- **Perfect for**: Expert racers seeking maximum challenge

---

## 🎓 Technical Highlights

### Object-Oriented Design
- **Design Patterns**: Singleton (GameEngine, Logger), Composition
- **Memory Management**: `std::unique_ptr` for automatic resource cleanup
- **Modern C++**: Lambda functions, range-based loops, STL containers

### Game Development Concepts
- **Fixed Timestep Game Loop** (60 FPS)
- **State Machine** for game states (Menu, Race, Game Over)
- **Component-Based Architecture**
- **Event-Driven UI** with callbacks
- **Physics Simulation**: Euler integration, impulse-based collision

### AI Implementation
- **Simplified Racing AI**: Full throttle with gentle steering corrections
- **Checkpoint Targeting**: AI tracks progress and targets next checkpoint
- **Difficulty Scaling**: AI parameters adjust based on level (1-3)
- **Strategic Nitro**: Frequency-based boost usage

### Code Quality
- **~2500+ lines** of organized C++ code
- **15+ classes** following SOLID principles
- **Modular subsystems** with clear interfaces
- **Comprehensive documentation**
- **Clean .gitignore** (excludes build artifacts)

---

## 📈 Recent Updates

### Latest Features (v1.0)
- ✅ **Bike Selection**: Choose red or blue bike before each race
- ✅ **AI Difficulty Scaling**: Easy/Medium/Hard AI based on track level
- ✅ **Obstacle Scaling**: 3x, 6x, 12x obstacles for progressive difficulty
- ✅ **Improved AI Navigation**: CPUs race competitively toward finish
- ✅ **Dark Gradient UI**: Professional interface across all screens
- ✅ **Randomized Starts**: Fair starting positions each race
- ✅ **Explicit Direction Init**: All bikes face forward at race start
- ✅ **Reduced Collision Sensitivity**: Stable, realistic physics

---

## 🔮 Future Enhancements

- [x] AI opponents with difficulty scaling
- [x] Bike selection system
- [x] Dynamic obstacle scaling
- [ ] Online multiplayer
- [ ] Custom 3D bike models
- [ ] Particle effects (dust, sparks, exhaust)
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
- AI implementation
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
