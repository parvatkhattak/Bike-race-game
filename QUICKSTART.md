# 🚀 Quick Start Guide - Bike Race Game

## Option 1: Automated Installation (RECOMMENDED)

Run this single command to install dependencies, build, and run the game:

```bash
cd /home/parvat-khattak/Downloads/Bike-race-game
chmod +x install_and_run.sh
./install_and_run.sh
```

You'll be prompted for your sudo password to install dependencies.

---

## Option 2: Manual Installation

If you prefer to install step-by-step:

### Step 1: Install Dependencies
```bash
# Install CMake and build tools
sudo apt update
sudo apt install -y cmake build-essential

# Install Raylib dependencies
sudo apt install -y libasound2-dev mesa-common-dev libx11-dev \
                    libxrandr-dev libxi-dev xorg-dev \
                    libgl1-mesa-dev libglu1-mesa-dev
```

### Step 2: Build the Game
```bash
cd /home/parvat-khattak/Downloads/Bike-race-game

# Clean any old builds
rm -rf build/*

# Configure and build
mkdir -p build && cd build
cmake ..
make -j$(nproc)
```

### Step 3: Run the Game
```bash
./bin/BikeRaceGame
```

---

## 🎮 Controls

### Player 1 (Red Bike)
- **W** - Accelerate
- **S** - Brake
- **A** - Turn Left
- **D** - Turn Right
- **Left Shift** - Nitro Boost

### Player 2 (Blue Bike)
- **↑** - Accelerate
- **↓** - Brake
- **←** - Turn Left
- **→** - Turn Right
- **Right Shift** - Nitro Boost

### Menu
- **ENTER** - Select
- **Arrow Keys** - Navigate
- **ESC** - Pause

---

## 🏁 How to Play

1. **Main Menu** → Press ENTER on "START GAME"
2. **Bike Selection** → Press ENTER to confirm
3. **Level Selection** → Choose your track (use ↑↓, press ENTER)
4. **Race!** → Wait for countdown, then race through checkpoints
5. **Win** → Complete all laps first!

---

## ⚠️ Troubleshooting

### "cmake: command not found"
You need to install CMake first:
```bash
sudo apt install cmake
```

### "No such file or directory: BikeRaceGame"
The game wasn't built successfully. Try:
```bash
cd /home/parvat-khattak/Downloads/Bike-race-game
rm -rf build
./install_and_run.sh
```

### "Unable to open display"
If running over SSH, you need X11 forwarding or run locally.

### Black screen or crash
Make sure you have OpenGL support:
```bash
glxinfo | grep "OpenGL version"
```

---

## 📝 What You Built

This is a complete 3D racing game with:
- ✅ Full physics engine (gravity, friction, collision)
- ✅ 3 racing tracks with progressive difficulty
- ✅ Split-screen 2-player multiplayer
- ✅ Lap counting and position tracking
- ✅ Upgrade system and level progression
- ✅ Complete UI with menus and HUD

**~2,000 lines of C++ code, 15+ classes, full OOP architecture!**

Enjoy your game! 🏍️💨
