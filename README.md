# 🎮 Ngen2D - 2D Physics Engine

[![C++17](https://img.shields.io/badge/C++-17-blue.svg)](https://isocpp.org/)
[![CMake](https://img.shields.io/badge/CMake-3.16+-green.svg)](https://cmake.org/)
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
- ✅ **SDL2 Integration**: Window management, rendering pipeline, and event handling
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
│   │   └── ...         # [Future: World, Collider, Constraints]
│   │
│   └── core/           # [Future: Object lifecycle, Memory management]
│
├── platform/           # Platform-specific rendering/windowing
│   └── SDLApp          # SDL2 window and renderer wrapper
│
├── demo/               # [Future: Example scenes and tests]
│
└── main.cpp            # Application entry point
```

### Component Responsibilities

| Component | Purpose | Dependencies |
|-----------|---------|--------------|
| **Vector2** | 2D math operations for physics calculations | None (stdlib only) |
| **RigidBody** | Stores physical properties and integrates motion | Vector2 |
| **SDLApp** | Manages window, renderer, and event loop | SDL2 |
| **PhysicsDemo** | Entry point that wires everything together | engine, platform |

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
```

**Windows (vcpkg):**
```bash
vcpkg install sdl2:x64-windows
```

## 🔨 Building

### Quick Start

```bash
# Clone the repository
git clone https://github.com/yourusername/Ngen2D.git
cd Ngen2D

# Create build directory
mkdir build && cd build

# Configure and build
cmake ..
cmake --build .

# Run the demo
./PhysicsDemo          # Linux/macOS
.\PhysicsDemo.exe      # Windows
```

### CMake Options

```bash
# Debug build
cmake -DCMAKE_BUILD_TYPE=Debug ..

# Release build with optimizations
cmake -DCMAKE_BUILD_TYPE=Release ..
```

## 📁 Project Structure

```
Ngen2D/
├── CMakeLists.txt              # Root build configuration
├── main.cpp                    # Application entry point
├── README.md                   # This file
│
├── engine/                     # Core physics engine
│   ├── CMakeLists.txt         # Engine library definition
│   ├── math/
│   │   ├── Vector2.h          # Vector2 interface
│   │   └── Vector2.cpp        # Vector2 implementation
│   └── physics/
│       ├── RigidBody.h        # Rigid body interface
│       └── RigidBody.cpp      # Physics integration logic
│
├── platform/                   # Platform abstraction
│   ├── CMakeLists.txt         # Platform library definition
│   ├── SDLApp.h               # SDL application interface
│   └── SDLApp.cpp             # SDL implementation
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
- [ ] Fix normalize() memory leak

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