# 🎮 Ngen2D - 2D Physics Engine

[![C++17](https://img.shields.io/badge/C++-17-blue.svg)](https://isocpp.org/)
[![CMake](https://img.shields.io/badge/CMake-3.28+-green.svg)](https://cmake.org/)
[![SDL2](https://img.shields.io/badge/SDL2-2.0+-orange.svg)](https://www.libsdl.org/)
[![License](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

A lightweight, modular 2D physics engine written in modern C++ with SDL2 rendering. Designed for learning game physics fundamentals and rapid prototyping.

## 📋 Table of Contents

- [Features](#-features)
- [Architecture](#-architecture)
- [Prerequisites](#-prerequisites)
- [Building](#-building)
- [Project Structure](#-project-structure)
- [Usage](#-usage)
- [Roadmap](#-roadmap)
- [Contributing](#-contributing)

## ✨ Features

### Current Implementation
- ✅ **2D Vector Mathematics**: Complete vector operations (addition, subtraction, scalar multiplication, dot product, normalization)
- ✅ **Rigid Body Dynamics**: Position-based physics with force accumulation and Euler integration
- ✅ **Physics World System**: Centralized physics simulation with body management
- ✅ **SDL2 Integration**: Window management, rendering pipeline, and event handling with rectangle drawing
- ✅ **Demo System**: Working sandbox demo with physics visualization
- ✅ **Modular Architecture**: Separated engine logic from platform-specific code

### In Development
- 🚧 Collision detection (AABB, Circle, SAT)
- 🚧 Shape primitives (Circle, Box, Polygon)
- 🚧 Constraint solving
- 🚧 Spatial partitioning (Quadtree)

## 🏗️ Architecture

```
Ngen2D/
│
├── engine/              # Core physics engine (platform-agnostic)
│   ├── math/           # Mathematical primitives
│   │   ├── Vector2     # 2D vector with standard operations
│   │   └── ...         # [Future: Matrix, Transform, etc.]
│   │
│   ├── physics/        # Physics simulation
│   │   ├── RigidBody   # Dynamic body with mass and forces
│   │   └── PhysicsWorld # Physics simulation manager
│   │
│   ├── shapes/         # Shape primitives [Coming soon]
│   └── core/           # [Future: Object lifecycle, Memory management]
│
├── platform/           # Platform-specific rendering/windowing
│   └── SDLApp          # SDL2 window and renderer wrapper
│
├── demo/               # Example scenes and tests
│   └── Sandbox         # Working physics demonstration
│
└── main.cpp            # Application entry point
```

### Component Responsibilities

| Component | Purpose | Dependencies |
|-----------|---------|--------------|
| **PhysicsWorld** | Manages all physics bodies and simulation stepping | RigidBody |
| **SDLApp** | Manages window, renderer, event loop, and drawing | SDL2 |
| **Sandbox** | Demo scene showcasing physics simulation | PhysicsWorld, RigidBody |
| **PhysicsDemo** | Entry point that wires everything together | engine, platform, demo

## 📦 Prerequisites

- **C++17 compatible compiler** (GCC 7+, Clang 5+, MSVC 2017+)
- **CMake 3.16+**
- **SDL2 development libraries**

### Installing SDL2

**Ubuntu/Debian:**
```bash
sudo apt-get install libsdl2-dev
```

**macOS (Homebrew):**
```bash
brew install sdl2
```:**
1. Download SDL2 development libraries from [libsdl.org](https://github.com/libsdl-org/SDL/releases)
2. Extract to `C:\SDL2`
3. SDL2.dll will be needed alongside the executablebash
vcpkg install sdl2:x64-windows
```

## Linux/macOS

```bash
# Clone the repository
git clone https://github.com/yourusername/Ngen2D.git
cd Ngen2D

# Create build directory
mkdir build && cd build

# Configure and build
cmake -DCMAKE_BUILD_TYPE=Release ..
make

# Run the demo
./PhysicsDemo
```

### Windows (MinGW)

```powershell
# Navigate to project
cd Ngen2D

# Create build directory
mkdir build
cd build

# Configure with MinGW
cmake .. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH="C:/SDL2"

# Build
mingw32-make

# Copy SDL2.dll
copy C:\SDL2\lib\x86\SDL2.dll .

# Run the demo
├── .gitignore                  # Git ignore rules
│
├── engine/                     # Core physics engine
│   ├── CMakeLists.txt         # Engine library definition
│   ├── math/
│   │   ├── Vector2.h          # Vector2 interface
│   │   └── Vector2.cpp        # Vector2 implementation
│   ├── physics/
│   │   ├── RigidBody.h        # Rigid body interface
│   │   ├── RigidBody.cpp      # Physics integration logic
│   │   ├── PhysicsWorld.h     # Physics world manager interface
│   │   └── PhysicsWorld.cpp   # Physics world implementation
│   ├── shapes/                # Shape primitives (empty for now)
│   └── core/                  # Core systems (empty for now)
│
├── platform/                   # Platform abstraction
│   ├── CMakeLists.txt         # Platform library definition
│   ├── SDLApp.h               # SDL application interface
│   └── SDLApp.cpp             # SDL implementation with rendering
│
├── Current Demo

The project includes a working physics demo in the `Sandbox` class. When you run `PhysicsDemo.exe`, you'll see:
- A physics simulation with a box
- Real-time rendering using SDL2
- Basic physics world management

### Example Code

```cpp
#include "platform/SDLApp.h"
#include "demo/Sandbox.h"

int main(int argc, char* argv[]) {
    SDLApp app;
    Sandbox sandbox;

    if(!app.Init())
        return -1;

    // Main game loop
    while(app.IsRunning()) {
        app.HandleEvents();      // Process input
        sandbox.Update();        // Update physics
        
        app.Clear();             // Clear screen
        RigidBody* box = sandbox.GetBox();
        app.DrawRect(box->position.x, box->position.y, 50, 50);  // Draw box
        app.Render();            // Present frame
    }

    app.Shutdown();
    return 0;
}ompleted)
- [x] Vector2 mathematics
- [x] Rigid body dynamics
- [x] Physics World system
- [x] Basic SDL2 integration with rendering
- [x] Working demo application
- [x] Windows build support
```cpp
#include "engine/physics/PhysicsWorld.h"
#include "engine/physics/RigidBody.h"

// Create a physics world
PhysicsWorld world;

// Create a physics body
RigidBody ball(1.0f);  // 1kg mass
ball.position = Vector2(400, 300);

// Add to world
world.AddBody(&ball);

// In update loop
Vector2 gravity(0, 9.8f);
ball.ApplyForce(gravity * ball.mass);
world.Step(deltaTime);  // Update all bodies   └── SDLApp.cpp             # SDL implementation
│
└── build/                      # Generated build artifacts (git-ignored)
```

## 🎯 Usage

### Basic Example (Coming Soon)

```cpp
#include "engine/physics/RigidBody.h"
#include "platform/SDLApp.h"

int main() {
    SDLApp app;
    app.Init();
    
    // Create a physics body
    RigidBody ball(1.0f);  // 1kg mass
    ball.position = Vector2(400, 300);
    
    // Apply gravity
    Vector2 gravity(0, 9.8f);
    
    // Game loop
    while (app.IsRunning()) {
        ball.ApplyForce(gravity * ball.mass);
        ball.Integrate(0.016f);  // ~60 FPS
        
        // Render ball at ball.position
        app.Run();
    }
    
    app.Shutdown();
    return 0;
}
```

## 🛤️ Roadmap

### Phase 1: Core Physics ✅ (Current)
- [x] Vector2 mathematics
- [x] Rigid body dynamics
- [x] Basic SDL2 integration
- [x] Fix normalize() memory leak

### Phase 2: Collision System 🚧
- [ ] AABB (Axis-Aligned Bounding Box)
- [ ] Circle collision
- [ ] SAT (Separating Axis Theorem) for polygons
- [ ] Collision response (impulse-based)

### Phase 3: Shapes & Rendering
- [ ] Circle primitive
- [ ] Box primitive
- [ ] Convex polygon
- [ ] Debug rendering (wireframe)

### Phase 4: Advanced Features
- [ ] Constraint solving (joints, springs)
- [ ] Spatial partitioning (Quadtree/Grid)
- [ ] Continuous collision detection
- [ ] Material properties (friction, restitution)

### Phase 5: Optimization & Polish
- [ ] SIMD vector operations
- [ ] Multi-threading support
- [ ] Profiling tools
- [ ] Extensive unit tests

## 🤝 Contributing

Contributions are welcome! Areas that need help:
- Implementing collision detection algorithms
- Adding shape primitives
- Creating demo scenes
- Writing unit tests
- Documentation improvements

## 📄 License

This project is licensed under the MIT License - see the LICENSE file for details.

## 🙏 Acknowledgments

- Inspired by Box2D and Chipmunk2D
- SDL2 for cross-platform rendering
- Game Physics Engine Development by Ian Millington

## 📞 Contact

**Author**: Zafar  
**Project**: [Ngen2D](https://github.com/yourusername/Ngen2D)

---

*Built with ❤️ for learning game physics*