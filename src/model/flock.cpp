#include "flock.h"
#include <cmath>

// --- Helper for Random Number Generation (Used in constructor) ---
float random_float(std::default_random_engine& engine, std::uniform_real_distribution<float>& dist, float min, float max) {
    dist.param(std::uniform_real_distribution<float>::param_type(min, max));
    return dist(engine);
}

// --- Constructor (Initialization) ---
Flock::Flock(int num_boids, int width, int height) {
    // Seed the random engine
    std::random_device rd;
    engine.seed(rd());

    // Initialize boids at random positions with small random velocities
    for (int i = 0; i < num_boids; ++i) {
        Vec2 pos = {random_float(engine, rand_dist, 0.0f, (float)width), 
                    random_float(engine, rand_dist, 0.0f, (float)height)};
        
        // Initial velocity should be small
        Vec2 vel = {random_float(engine, rand_dist, -MAX_SPEED, MAX_SPEED) * 0.1f, 
                    random_float(engine, rand_dist, -MAX_SPEED, MAX_SPEED) * 0.1f};
        
        boids.emplace_back(pos, vel);
    }
}

// --- 1. Rule Implementations (Stubs) ---

// Rule 1: Cohesion (Move towards average position)
Vec2 Flock::rule_cohesion(const Boid& b) const {
    Vec2 center_of_mass = {0.0f, 0.0f};
    int count = 0;

    for (const Boid& other : this->boids) {
        if (&other != &b) { // Exclude itself
            center_of_mass += other.position;
            count++;
        }
    }
    
    if (count > 0) {
        center_of_mass /= (float)count;
        // The acceleration vector is towards the center of mass: (c - B.position)
        return center_of_mass - b.position;
    }
    return {0.0f, 0.0f};
}

Vec2 Flock::rule_separation(const Boid& b) const {
    Vec2 steering_vector = {0.0f, 0.0f};
    int count = 0;

    // Iterate over all other boids in the flock
    for (const Boid& other : boids) {
        if (&other == &b) {
            continue; // Skip the boid itself
        }

        // 1. Calculate the squared distance between the two boids
        // Use squared distance for efficiency (avoiding sqrt)
        float dist_sq = distance_sq(b.position, other.position);

        // 2. Check if the distance is within the repulsion radius
        if (dist_sq < SEPARATION_WEIGHT * SEPARATION_WEIGHT) {
            // Found a "close boid"

            // 3. Calculate the repulsion vector (B.position - B'.position)
            // This vector points directly FROM the neighbor TO the current boid.
            Vec2 difference = b.position - other.position;

            // 4. Weight the repulsion inversely by distance
            // Closer boids should exert a stronger repulsive force.
            // We use the magnitude_sq for weighting to keep the calculation fast.
            // (1.0f / dist_sq) makes the force much stronger when dist is small.
            if (dist_sq > 0.0f) {
                 // Weight = 1 / (distance^2)
                difference = difference / dist_sq;
                steering_vector += difference;
                count++;
            }
        }
    }

    // 5. Calculate the average steering force (Optional: just returning the sum works too)
    // If the count is high, we usually divide by count to average the forces, 
    // or just return the accumulated sum. Returning the sum often leads to better separation.
    
    // The accumulated steering_vector represents the total repulsive force/acceleration.
    return steering_vector;
}
// Rule 3: Alignment (Match average velocity)
Vec2 Flock::rule_alignment(const Boid& b) const {
    // TODO: Calculate average velocity v of all neighbors (initially, all boids)
    Vec2 average_velocity = {0.0f, 0.0f};
    int count = 0;

    for (const Boid& other : this->boids) {
        if (&other != &b) { // Exclude itself
            average_velocity += other.velocity;
            count++;
        }
    }

    if (count > 0) {
        average_velocity /= (float)count;
        // The acceleration vector is towards the average velocity: (v - B.speed)
        return average_velocity - b.velocity;
    }
    return {0.0f, 0.0f};
}

// --- 2. Utility Implementations ---

void Flock::limit_velocity(Boid& b) {
    if (b.velocity.magnitude() > MAX_SPEED) {
        b.velocity = b.velocity.normalize() * MAX_SPEED;
    }
}

void Flock::wrap_position(Boid& b, int width, int height) {
    // If the boid leaves the screen, make it reappear on the opposite side
    if (b.position.x < 0) b.position.x += width;
    if (b.position.x > width) b.position.x -= width;
    if (b.position.y < 0) b.position.y += height;
    if (b.position.y > height) b.position.y -= height;
}


// --- 3. Core Update Loop ---
void Flock::update(float dt, int width, int height) {
    // We iterate over the boids using an index to handle the modification
    // or by creating a temporary copy of the old positions/velocities 
    // to ensure rules are based on the state at the beginning of the frame.
    // For simplicity, we iterate and modify in place (using a range-based loop is easiest).
    
    for (Boid& b : this->boids) {
        
        // 1. Calculate Rule Accelerations (Forces)
        Vec2 a_cohesion = rule_cohesion(b);
        Vec2 a_separation = rule_separation(b);
        Vec2 a_alignment = rule_alignment(b);
        
        // 2. Apply Weights and Sum (F = ma, where F is the sum of weighted rule accelerations)
        Vec2 total_acceleration = 
            a_cohesion * COHESION_WEIGHT + 
            a_separation * SEPARATION_WEIGHT + 
            a_alignment * ALIGNMENT_WEIGHT;

        // 3. Limit Acceleration (Force)
        if (total_acceleration.magnitude() > MAX_FORCE) {
            total_acceleration = total_acceleration.normalize() * MAX_FORCE;
        }

        // 4. Update Boid State
        b.acceleration = total_acceleration;
        
        // Update velocity: v = v + dt * a 
        b.velocity += b.acceleration * dt;

        // Limit Velocity (Speed)
        limit_velocity(b);

        // Update position: p = p + dt * v
        b.position += b.velocity * dt;

        // Apply boundary conditions (wrap around screen)
        wrap_position(b, width, height);
    }
}