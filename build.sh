#!/bin/bash

# Build script for Bike Race Game
# This script checks dependencies, builds the project, and runs it

set -e  # Exit on error

echo "========================================"
echo "  Bike Race Game - Build Script"
echo "========================================"
echo ""

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Check if CMake is installed
if ! command -v cmake &> /dev/null; then
    echo -e "${RED}❌ CMake is not installed!${NC}"
    echo ""
    echo "Please install CMake:"
    echo "  Ubuntu/Debian: sudo apt install cmake build-essential"
    echo "  macOS: brew install cmake"
    echo ""
    exit 1
fi

echo -e "${GREEN}✓ CMake found${NC}"

# Check for required libraries (Linux only)
if [[ "$OSTYPE" == "linux-gnu"* ]]; then
    echo "Checking for Raylib dependencies..."
    
    MISSING_DEPS=()
    
    # Check for X11
    if ! dpkg -l | grep -q libx11-dev; then
        MISSING_DEPS+=("libx11-dev")
    fi
    
    if [ ${#MISSING_DEPS[@]} -gt 0 ]; then
        echo -e "${YELLOW}⚠ Some dependencies may be missing${NC}"
        echo "You may need to install:"
        echo "  sudo apt install libasound2-dev mesa-common-dev libx11-dev libxrandr-dev libxi-dev xorg-dev libgl1-mesa-dev libglu1-mesa-dev"
        echo ""
    fi
fi

# Create build directory
echo ""
echo "Creating build directory..."
mkdir -p build
cd build

# Configure with CMake
echo ""
echo "Configuring project with CMake..."
echo "(Raylib will be automatically downloaded if not found)"
cmake .. || {
    echo -e "${RED}❌ CMake configuration failed!${NC}"
    exit 1
}

# Build
echo ""
echo "Compiling project..."
make -j$(nproc) || {
    echo -e "${RED}❌ Compilation failed!${NC}"
    exit 1
}

echo ""
echo -e "${GREEN}========================================"
echo "  ✓ Build Successful!"
echo "========================================${NC}"
echo ""
echo "To run the game:"
echo "  cd build"
echo "  ./bin/BikeRaceGame"
echo ""
echo "Or run directly with:"
echo "  ./bin/BikeRaceGame"
echo ""

# Ask if user wants to run the game
read -p "Run the game now? (y/n) " -n 1 -r
echo
if [[ $REPLY =~ ^[Yy]$ ]]; then
    echo ""
    echo "Starting Bike Race Game..."
    echo ""
    ./bin/BikeRaceGame
fi
