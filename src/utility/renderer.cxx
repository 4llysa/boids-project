#include "renderer.h"
#include <cmath>

namespace Renderer {

    void draw_oriented_boid(SDL_Renderer* renderer, const Boid& b, float size) {
        
        // Use a small constant to check if the boid is moving. 
        // If not, drawing the triangle based on a zero vector is problematic.
        if (b.velocity.magnitude_sq() < 0.01f) {
            // Draw a dot if stationary
            SDL_SetRenderDrawColor(renderer, 0, 0, 255, SDL_ALPHA_OPAQUE); 
            SDL_Rect r = { (int)(b.position.x - 2), (int)(b.position.y - 2), 4, 4 };
            SDL_RenderFillRect(renderer, &r);
            return;
        }

        // Set color for the boid (e.g., Blue)
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, SDL_ALPHA_OPAQUE);

        // Calculate the angle of the velocity vector (in radians)
        // atan2(y, x) gives the angle relative to the positive x-axis.
        float angle = std::atan2(b.velocity.y, b.velocity.x);

        // Define a constant for PI (ensure it's defined globally or locally if needed)
        float const PI = 3.1415927f; 

        // --- Calculate Rotated Vertices ---
        // Vertices are calculated relative to b.position, rotated by 'angle'.

        // Vertex 0 (Tip): Forward
        float x0 = b.position.x + size * std::cos(angle);
        float y0 = b.position.y + size * std::sin(angle);

        // Vertex 1 (Back-right corner)
        // Offset angle by 135 degrees (3*PI/4) or similar value for a good wing spread
        float x1 = b.position.x + size * 0.7f * std::cos(angle - 3.0f * PI / 4.0f);
        float y1 = b.position.y + size * 0.7f * std::sin(angle - 3.0f * PI / 4.0f);

        // Vertex 2 (Back-left corner)
        // Offset angle by -135 degrees (3*PI/4)
        float x2 = b.position.x + size * 0.7f * std::cos(angle + 3.0f * PI / 4.0f);
        float y2 = b.position.y + size * 0.7f * std::sin(angle + 3.0f * PI / 4.0f);
        
        // --- Draw the Triangle Outline ---
        
        // Line 1: Tip to Back-right
        SDL_RenderDrawLine(renderer, (int)x0, (int)y0, (int)x1, (int)y1);
        
        // Line 2: Back-right to Back-left (Base)
        SDL_RenderDrawLine(renderer, (int)x1, (int)y1, (int)x2, (int)y2);
        
        // Line 3: Back-left to Tip
        SDL_RenderDrawLine(renderer, (int)x2, (int)y2, (int)x0, (int)y0);
    }
}