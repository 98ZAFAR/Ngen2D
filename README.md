# 🎮 Ngen2D - Native Engine 2D

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
- ✅ **2D Vector Mathematics**: Complete vector operations (addition, subtraction, scalar multiplication, dot product, cross product, normalization)
- ✅ **Rigid Body Dynamics**: Full physics simulation with force/torque accumulation, linear/angular velocity, and Euler integration
- ✅ **Rotation & Angular Dynamics**: Complete angular physics with orientation, angular velocity, torque, and moment of inertia
- ✅ **Physics World System**: Centralized physics simulation with body management, force generators, and sleep optimization
- ✅ **SDL2 Integration**: Window management, rendering pipeline, event handling with rotated rectangle and circle drawing
- ✅ **Demo System**: Interactive sandbox demo with mouse-based object spawning and rotation visualization
- ✅ **Modular Architecture**: Separated engine logic from platform-specific code
- ✅ **Advanced Collision Detection**: 
  - OBB (Oriented Bounding Box) collision using SAT
  - Circle vs Circle collision
  - OBB vs Circle hybrid collision
  - Proper contact point generation
- ✅ **Impulse-Based Collision Resolution**: Physically accurate collision response with angular components and restitution
- ✅ **Advanced Friction System**: Dynamic and static friction using Coulomb friction model with angular friction
- ✅ **Spatial Hash Optimization**: Broad-phase collision detection using spatial hashing for improved performance
- ✅ **Sleep System**: Automatic body sleeping for idle objects to reduce CPU usage

### In Development
- 🚧 Polygon shape primitives (arbitrary convex polygons)
- 🚧 Constraint solving (joints, springs, motors)
- 🚧 Continuous collision detection (CCD) for fast-moving objects

## 🏗️ Architecture

```
Ngen2D/
│
├── engine/              # Core physics engine (platform-agnostic)
│   ├── math/           # Mathematical primitives
│   │   ├── Vector2     # 2D vector with standard operations
│   │   └── MathUtils   # Utility functions (Clamp, etc.)
│   │
│   ├── physics/        # Physics simulation
│   │   ├── RigidBody   # Dynamic body with mass, forces, velocity, orientation, angular velocity, torque
│   │   ├── PhysicsWorld # Physics simulation manager with force generators and sleep system
│   │   └── SpatialHash  # Broad-phase collision optimization
│   │
│   ├── collision/      # Collision detection and resolution
│   │   ├── Collider     # Collider wrapper with material properties
│   │   ├── AABBCollider # AABB structure definition
│   │   ├── Collision    # OBB and Circle collision detection using SAT
│   │   ├── CollisionManifold # Collision data with normal, penetration, contact point
│   │   └── CollisionResolver # Impulse-based physics with angular components and friction
│   │
│   ├── shapes/         # Shape primitives
│   │   ├── Shape       # Base shape interface
│   │   ├── AABBShape   # Oriented bounding box (supports rotation)
│   │   └── CircleShape # Circle primitive
│   │
│   ├── forces/         # Force generators
│   │   ├── ForceGenerator # Base force generator interface
│   │   └── GravityForce   # Constant gravity force
│   │
│   └── core/           # Core utilities (Time, Config)
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
| **PhysicsWorld** | Manages physics bodies, simulation stepping, sleep system, and spatial hashing | RigidBody, SpatialHash |
| **SpatialHash** | Broad-phase collision detection for improved performance | RigidBody |
| **Collision** | Multi-shape collision detection (AABB, Circle, hybrid) | RigidBody, Shapes, CollisionManifold |
| **CollisionResolver** | Impulse-based physics with restitution and Coulomb friction | RigidBody, CollisionManifold |
| **Collider** | Collision wrapper with shape and material properties (friction, restitution) | Shape |
| **Shape** | Abstract shape interface with AABB and Circle implementations | - |
| **SDLApp** | Manages window, renderer, event loop, and drawing (rectangles & circles) | SDL2 |
| **Sandbox** | Interactive demo scene with mouse spawning and mixed shapes | PhysicsWorld, RigidBody, Collider |
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
```

**Windows (vcpkg):**
```bash
vcpkg install sdl2:x64-windows
```

**Windows (Manual):**
1. Download SDL2 development libraries from [libsdl.org](https://github.com/libsdl-org/SDL/releases)
2. Extract to `C:\SDL2`
3. SDL2.dll will be needed alongside the executable

## 🔨 Building

### Linux/macOS

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
./PhysicsDemo.exe
```

## 🎯 Usage

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
}

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
world.Step(deltaTime);  // Update all bodies 
```

### Running the Demo

The project includes an interactive physics demo with full rotation support. When you run the executable:
- **Click anywhere** to spawn circular objects with initial horizontal velocity
- Objects automatically interact with physics (gravity, collisions, friction, rotation)
- Pre-spawned objects include rotatable boxes and circles
- Watch realistic bouncing, rolling, spinning, and sleeping behavior
- Rotation is visualized with red indicator lines showing object orientation

### Creating Physics Objects

```cpp
#include "engine/physics/PhysicsWorld.h"
#include "engine/physics/RigidBody.h"
#include "engine/shapes/CircleShape.h"
#include "engine/shapes/AABBShape.h"
#include "engine/collision/Collider.h"

PhysicsWorld world;

// Create a dynamic circle
RigidBody* ball = new RigidBody(1.0f);  // 1kg mass
ball->position = Vector2(400, 300);
ball->velocity = Vector2(200.0f, 0.0f);
ball->angularVelocity = 2.0f;  // Initial rotation (rad/s)
ball->collider = new Collider(new CircleShape(25.0f));  // 25px radius
ball->collider->restitution = 0.8f;      // Bounciness (0-1)
ball->collider->dynamicFriction = 0.2f;  // Friction coefficient
ball->SetInverseInertia(ball->collider->shape->GetType());  // Calculate moment of inertia

world.AddBody(ball);

// Create a static ground (infinite mass)
RigidBody* ground = new RigidBody(0.0f);  // 0 mass = infinite mass (immovable)
ground->position = Vector2(400, 550);
ground->size = Vector2(800, 50);
ground->orientation = 0.1f;  // Slightly tilted platform
ground->collider = new Collider(new AABBShape(ground->size / 2));
ground->collider->restitution = 0.5f;
ground->collider->dynamicFriction = 0.3f;

world.AddBody(ground);

// In your game loop (60 FPS)
world.Step(1.0f / 60.0f);  // Updates all bodies, handles collisions with rotation
```

### Material Properties

```cpp
// Bounciness (restitution)
collider->restitution = 0.0f;  // No bounce (inelastic)
collider->restitution = 0.5f;  // Medium bounce
collider->restitution = 1.0f;  // Perfect bounce (elastic)

// Friction (affects both linear and angular motion)
collider->staticFriction = 0.4f;   // Starting friction
collider->dynamicFriction = 0.2f;  // Sliding/rolling friction
```

### Angular Dynamics

```cpp
// Set initial rotation
body->orientation = 1.57f;  // 90 degrees in radians

// Apply angular velocity (rad/s)
body->angularVelocity = 3.0f;  // Spin at 3 rad/s

// Apply torque (for rotational forces)
body->ApplyTorque(500.0f);

// Apply force at a point (generates both linear force and torque)
Vector2 force(100, 0);
Vector2 point(body->position.x + 20, body->position.y);
body->ApplyForceAtPoint(force, point);

// Configure rotational inertia (must call after setting collider)
body->SetInverseInertia(body->collider->shape->GetType());

// Angular damping (0.96 = 4% energy loss per frame)
body->angularDamping = 0.96f;  // Controls rotational slowdown
```

## 🛤️ Roadmap

### Phase 1: Core Physics
- [x] Vector2 mathematics
- [x] Rigid body dynamics
- [x] Basic SDL2 integration
- [x] Fix normalize() memory leak

### Phase 2: Collision Handling ✅ (Completed)
- [x] AABB (Axis-Aligned Bounding Box) collision detection
- [x] Circle collision detection
- [x] AABB vs Circle hybrid collision
- [x] Mass-based collision resolution (position correction)
- [x] Impulse-based collision response (velocity changes)
- [x] Collision manifold generation

### Phase 3: Shapes & Rendering ✅ (Completed)
- [x] Circle primitive with radius
- [x] AABB (Box) primitive with half-size
- [x] Shape interface with type identification
- [x] Circle (Bresenham's Midpoint) rendering
- [x] SDL2 rectangle rendering

### Phase 4: Rotation & Advanced Physics ✅ (Completed)
- [x] Full angular dynamics (orientation, angular velocity, torque, inertia)
- [x] Oriented Bounding Box (OBB) collision using SAT
- [x] Angular impulse resolution with proper inertia calculations
- [x] Angular friction and damping
- [x] Rotation visualization with orientation indicators
- [x] Contact point generation from penetrating vertices
- [x] Material properties (friction, restitution)
- [x] Spatial hashing for broad-phase collision (O(N) performance)
- [x] Sleep system for idle bodies

### Phase 5: Advanced Features ⚙️ (Current)
- [ ] Constraint solving (joints, springs, motors)
- [ ] Continuous collision detection (CCD) for fast-moving objects
- [ ] Convex polygon support with arbitrary vertices
- [ ] Advanced spatial partitioning (QuadTree/BVH)
- [ ] One-way platforms and collision filtering

### Phase 6: Optimization & Polish
- [ ] SIMD vector operations for performance
- [ ] Multi-threading support for parallel collision detection
- [ ] Profiling tools and performance metrics
- [ ] Extensive unit tests and benchmarks
- [ ] Debug visualization modes

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

## 📞 Contact

**Author**: Zafar  
**Project**: [Ngen2D](https://github.com/98ZAFAR/Ngen2D)

---

*Built with ❤️ for learning game physics*