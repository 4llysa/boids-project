#pragma once

#include <cmath>
#include <iostream>

/**
 * @brief Represents a 2D vector for position, velocity, and acceleration.
 */
struct Vec2 {
    float x;
    float y;

    // --- Constructors ---

    Vec2() : x(0.0f), y(0.0f) {}
    Vec2(float x_val, float y_val) : x(x_val), y(y_val) {}

    // --- Vector Operations (Member Functions) ---

    /**
     * @brief Calculates the squared magnitude (length squared) of the vector.
     * Useful for distance comparisons as it avoids the expensive square root.
     */
    float magnitude_sq() const {
        return x * x + y * y;
    }

    /**
     * @brief Calculates the magnitude (length) of the vector.
     */
    float magnitude() const {
        return std::sqrt(magnitude_sq());
    }

    /**
     * @brief Returns a normalized (unit length) vector in the same direction.
     */
    Vec2 normalize() const {
        float mag = magnitude();
        if (mag > 1e-6) { // Avoid division by zero/near-zero
            return Vec2{x / mag, y / mag};
        }
        return Vec2{0.0f, 0.0f};
    }
    
    // --- Operator Overloading (Vector Arithmetic) ---

    // Vector Addition
    Vec2 operator+(const Vec2& other) const {
        return Vec2{x + other.x, y + other.y};
    }

    // Vector Subtraction
    Vec2 operator-(const Vec2& other) const {
        return Vec2{x - other.x, y - other.y};
    }

    // Compound Addition (e.g., v1 += v2)
    Vec2& operator+=(const Vec2& other) {
        x += other.x;
        y += other.y;
        return *this;
    }
    
    // Compound Subtraction (e.g., v1 -= v2)
    Vec2& operator-=(const Vec2& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }
};

// --- Operator Overloading (Scalar Arithmetic) ---

// Scalar Multiplication (Vector * float)
inline Vec2 operator*(const Vec2& v, float scalar) {
    return Vec2{v.x * scalar, v.y * scalar};
}

// Scalar Multiplication (float * Vector) - allows 5.0f * vector
inline Vec2 operator*(float scalar, const Vec2& v) {
    return Vec2{v.x * scalar, v.y * scalar};
}

// Scalar Division (Vector / float)
inline Vec2 operator/(const Vec2& v, float scalar) {
    // Note: In real-world code, you'd add a check for scalar == 0
    return Vec2{v.x / scalar, v.y / scalar};
}

// Compound Scalar Multiplication (e.g., v *= 5.0f)
inline Vec2& operator*=(Vec2& v, float scalar) {
    v.x *= scalar;
    v.y *= scalar;
    return v;
}

// Compound Scalar Division (e.g., v /= 5.0f)
inline Vec2& operator/=(Vec2& v, float scalar) {
    v.x /= scalar;
    v.y /= scalar;
    return v;
}

// --- Utility Functions ---

/**
 * @brief Calculates the dot product of two vectors.
 */
inline float dot(const Vec2& v1, const Vec2& v2) {
    return v1.x * v2.x + v1.y * v2.y;
}

/**
 * @brief Calculates the distance between the points represented by two vectors.
 */
inline float distance(const Vec2& v1, const Vec2& v2) {
    return (v1 - v2).magnitude();
}

/**
 * @brief Calculates the squared distance between the points represented by two vectors.
 * Useful for fast distance checks.
 */
inline float distance_sq(const Vec2& v1, const Vec2& v2) {
    return (v1 - v2).magnitude_sq();
}