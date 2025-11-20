#pragma once

#include <SDL2/SDL.h>
#include "model/boid.h" // Requires Boid structure definition
#include "model/vec2.h" // Requires Vec2 structure definition

namespace Renderer {
    /**
     * @brief Renders an oriented triangle representing a boid based on its velocity.
     * * This function is the primary drawing routine for individual boids.
     * * @param renderer The active SDL_Renderer pointer (usually 'g.renderer' from main).
     * @param b The Boid object to draw.
     * @param size The physical size (half-length) of the boid triangle.
     */
    void draw_oriented_boid(SDL_Renderer* renderer, const Boid& b, float size);
    
    /**
     * @brief Draws the target/nest the boids are moving towards.
     * @param renderer The active SDL_Renderer.
     * @param pos The position of the target (e.g., a Vec2 from the Flock class).
     */
    void draw_target(SDL_Renderer* renderer, const Vec2& pos);

    /**
     * @brief Draws lines or outlines to represent the boundaries of the simulation space.
     * @param renderer The active SDL_Renderer.
     * @param width The screen width.
     * @param height The screen height.
     */
    void draw_boundaries(SDL_Renderer* renderer, int width, int height);
}