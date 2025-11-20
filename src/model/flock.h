#include <vector>
#include <random>
#include "boid.h" 
#include "vec2.h"

/**
 * @brief Manages the entire collection of boids and the core simulation logic.
 */
class Flock {
private:
    const float SEPARATION_DISTANCE = 20.0f;
    std::vector<Boid> boids;
    
    // Constants for Rule Weights (from the assignment)
    // These constants will be used to weight the influence of each rule.
    const float COHESION_WEIGHT = 0.01f;
    const float SEPARATION_WEIGHT = 0.5f;
    const float ALIGNMENT_WEIGHT = 0.2f;
    const float MAX_SPEED = 5.0f;     // Example Max Speed (adjust as needed)
    const float MAX_FORCE = 0.5f;     // Example Max Acceleration/Force Limit

    // Variables for randomness
    std::default_random_engine engine;
    std::uniform_real_distribution<float> rand_dist;

    // Helper functions for the three Boids rules
    Vec2 rule_cohesion(const Boid& b) const;
    Vec2 rule_separation(const Boid& b) const;
    Vec2 rule_alignment(const Boid& b) const;

    // Utility functions for limits and boundaries
    void limit_velocity(Boid& b);
    void wrap_position(Boid& b, int width, int height);

public:
    /**
     * @brief Constructor: Initializes the flock with a set number of boids 
     * and seeds the random number generator.
     */
    Flock(int num_boids, int width, int height);

    /**
     * @brief The core update loop: Calculates rules and updates position/velocity 
     * for all boids over a time step (dt).
     */
    void update(float dt, int width, int height);

    /**
     * @brief Accessor to retrieve the boids vector for rendering.
     */
    const std::vector<Boid>& get_boids() const { return this->boids; }
};