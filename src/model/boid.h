#pragma once

#include "vec2.h" // Include the 2D vector structure

/**
 * @brief Represents a single Boid agent in the simulation.
 * * Each boid has a state defined by its position, velocity, and the 
 * acceleration calculated from the three rules (Cohesion, Separation, Alignment).
 */
class Boid {
public:
    // State vectors
    Vec2 position;
    Vec2 velocity;
    Vec2 acceleration;

    // --- Constructors ---

    /**
     * @brief Default constructor initializes boid at origin with zero velocity.
     */
    Boid() : position({0.0f, 0.0f}), velocity({0.0f, 0.0f}), acceleration({0.0f, 0.0f}) {}

    /**
     * @brief Constructor that initializes boid with specific position and velocity.
     */
    Boid(Vec2 pos, Vec2 vel) 
        : position(pos), 
          velocity(vel), 
          acceleration({0.0f, 0.0f}) {}
          
    // In a full implementation, you might add methods here for:
    // 1. Limiting speed/acceleration.
    // 2. Applying boundary conditions.
    // 3. Calculating the angle for rendering.
    
    // For now, the core logic will be managed by the separate Flock class.
};