#pragma once
#include <unordered_map>
#include <vector>
#include <cmath>
#include <algorithm>
#include "RigidBody.h"
#include "../collision/AABBCollider.h"
#include "../shapes/AABBShape.h"
#include "../shapes/CircleShape.h"

// Simple spatial hash for broad-phase collision detection
class SpatialHash {
public:
    SpatialHash(float cellSize = 100.0f) : cellSize(cellSize) {}

    void Clear() {
        grid.clear();
    }

    void Insert(RigidBody* body, int index) {
        AABB bounds = GetBodyAABB(body);
        
        int minX = static_cast<int>(std::floor(bounds.min.x / cellSize));
        int minY = static_cast<int>(std::floor(bounds.min.y / cellSize));
        int maxX = static_cast<int>(std::floor(bounds.max.x / cellSize));
        int maxY = static_cast<int>(std::floor(bounds.max.y / cellSize));

        for (int y = minY; y <= maxY; y++) {
            for (int x = minX; x <= maxX; x++) {
                long long key = GetKey(x, y);
                grid[key].push_back(index);
            }
        }
    }

    std::vector<std::pair<int, int>> GetPotentialCollisions(const std::vector<RigidBody*>& bodies) {
        std::vector<std::pair<int, int>> pairs;
        
        for (auto& [key, indices] : grid) {
            for (size_t i = 0; i < indices.size(); i++) {
                for (size_t j = i + 1; j < indices.size(); j++) {
                    int idxA = indices[i];
                    int idxB = indices[j];
                    
                    // Ensure we don't duplicate pairs
                    if (idxA < idxB) {
                        pairs.emplace_back(idxA, idxB);
                    }
                }
            }
        }
        
        // Remove duplicates (bodies in multiple cells)
        std::sort(pairs.begin(), pairs.end());
        pairs.erase(std::unique(pairs.begin(), pairs.end()), pairs.end());
        
        return pairs;
    }

private:
    float cellSize;
    std::unordered_map<long long, std::vector<int>> grid;

    long long GetKey(int x, int y) const {
        return (static_cast<long long>(x) << 32) | (static_cast<long long>(y) & 0xFFFFFFFF);
    }

    AABB GetBodyAABB(RigidBody* body) {
        AABB aabb;
        
        if (body->collider && body->collider->shape) {
            if (body->collider->shape->GetType() == ShapeType::AABB) {
                auto* shape = static_cast<AABBShape*>(body->collider->shape);
                aabb.min = body->position - shape->halfsize;
                aabb.max = body->position + shape->halfsize;
            } else if (body->collider->shape->GetType() == ShapeType::Circle) {
                auto* shape = static_cast<CircleShape*>(body->collider->shape);
                Vector2 radius(shape->radius, shape->radius);
                aabb.min = body->position - radius;
                aabb.max = body->position + radius;
            }
        } else {
            // Fallback to body.size
            Vector2 halfSize = body->size * 0.5f;
            aabb.min = body->position - halfSize;
            aabb.max = body->position + halfSize;
        }
        
        return aabb;
    }
};
