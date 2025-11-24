#!/bin/bash
# Quick Installation Script for Bike Race Game
# Run this script to install dependencies and build the game

echo "================================================"
echo "  Bike Race Game - Dependency Installation"
echo "================================================"
echo ""
echo "This script will install the required dependencies."
echo "You will need to enter your sudo password."
echo ""
read -p "Press Enter to continue..."

echo ""
echo "Step 1: Installing CMake and build tools..."
sudo apt update
sudo apt install -y cmake build-essential

echo ""
echo "Step 2: Installing Raylib dependencies..."
sudo apt install -y libasound2-dev mesa-common-dev libx11-dev \
                    libxrandr-dev libxi-dev xorg-dev \
                    libgl1-mesa-dev libglu1-mesa-dev

echo ""
echo "================================================"
echo "  Installation Complete!"
echo "================================================"
echo ""
echo "Now building the game..."
echo ""

# Clean old build
cd /home/parvat-khattak/Downloads/Bike-race-game
rm -rf build/*

# Create and enter build directory
mkdir -p build
cd build

# Configure with CMake
echo "Configuring project (downloading Raylib)..."
cmake ..

# Build
echo ""
echo "Compiling game..."
make -j$(nproc)

echo ""
echo "================================================"
echo "  BUILD SUCCESSFUL!"
echo "================================================"
echo ""
echo "Starting the game..."
echo ""

# Run the game
./bin/BikeRaceGame
