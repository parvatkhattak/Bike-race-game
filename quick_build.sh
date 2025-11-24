#!/bin/bash
# Quick build script - downloads Raylib release (faster than git clone)

set -e

echo "======================================"
echo "  Bike Race Game - Quick Build"
echo "======================================"
echo ""

cd /home/parvat-khattak/Downloads/Bike-race-game

# Clean previous builds
echo "Cleaning previous build..."
rm -rf build external/raylib
mkdir -p build external

# Download Raylib 5.0 release (much faster than git clone)
echo "Downloading Raylib 5.0..."
cd external
wget -q --show-progress https://github.com/raysan5/raylib/archive/refs/tags/5.0.tar.gz
tar -xzf 5.0.tar.gz
mv raylib-5.0 raylib
rm 5.0.tar.gz

# Build Raylib
echo ""
echo "Building Raylib (this takes 3-5 minutes)..."
cd raylib/src
make -j$(nproc) PLATFORM=PLATFORM_DESKTOP

# Build the game
echo ""
echo "Building Bike Race Game..."
cd /home/parvat-khattak/Downloads/Bike-race-game/build

# Simple CMake configuration
cat > CMakeLists.txt << 'EOF'
cmake_minimum_required(VERSION 3.20)
project(BikeRaceGame CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Raylib
set(RAYLIB_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/../external/raylib/src)
set(RAYLIB_LIB ${CMAKE_SOURCE_DIR}/../external/raylib/src/libraylib.a)

# Source files
file(GLOB_RECURSE SOURCES "${CMAKE_SOURCE_DIR}/../src/*.cpp")

# Create executable
add_executable(BikeRaceGame ${SOURCES})

# Include and link
target_include_directories(BikeRaceGame PRIVATE ${CMAKE_SOURCE_DIR}/../include ${RAYLIB_INCLUDE_DIR})
target_link_libraries(BikeRaceGame ${RAYLIB_LIB} m pthread dl GL X11)

# Output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
EOF

cmake .
make -j$(nproc)

echo ""
echo "======================================"
echo "  BUILD COMPLETE!"
echo "======================================"
echo ""
echo "Running the game..."
echo ""

# Run the game
mkdir -p bin/assets
./bin/BikeRaceGame
