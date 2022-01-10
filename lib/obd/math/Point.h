/**
 * @file Point.h
 * @author argawaen
 * @date 30/12/2021
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once
#include <cstdint>

/**
 * @brief Namespace gathering math functions
 */
namespace obd::math {

/**
 * @brief Struct for handling a point
 */
struct Point {
  /// First coordinate
  int16_t x;
  /// Second coordinate
  int16_t y;
};

/**
 * @brief compute the minimum between 2 values
 * @param a First value
 * @param b second value
 * @return The minimum
 */
inline constexpr int16_t min(int16_t a, int16_t b) { return (a > b) ? b : a; }
/**
 * @brief compute the maximum between 2 values
 * @param a First value
 * @param b second value
 * @return The maximum
 */
inline constexpr int16_t max(int16_t a, int16_t b) { return (a < b) ? b : a; }

/**
 * @brief Compute the clamped value
 * @param a Number to evaluate
 * @param l Lower bound
 * @param h Upper bound
 * @return The clamped value
 */
inline constexpr int16_t clamp(int16_t a, int16_t l, int16_t h) {
  return (a < l) ? l : ((a > h) ? h : a);
}

/**
 * @brief Compute the sum between two points.
 * @param a First point
 * @param b Second point
 * @return The sum
 */
inline constexpr Point operator+(const Point &a, const Point &b) {
  return {(int16_t)(a.x + b.x), (int16_t)(a.y + b.y)};
}

/**
 * @brief Compute the difference between two points.
 * @param a First point
 * @param b Second point
 * @return The sum
 */
inline constexpr Point operator-(const Point &a, const Point &b) {
  return {(int16_t)(a.x - b.x), (int16_t)(a.y - b.y)};
}

/**
 * @brief Inversion operator
 * @param f The numerator float
 * @param p The denominator Point
 * @return The resulting point
 */
inline constexpr Point operator/(float f, const Point &p) {
  if (p.x * p.y == 0)
    return {0, 0};
  return {(int16_t)(f / (float)p.x), (int16_t)(f / (float)p.y)};
}

/**
 * @brief Divide Points component by a number.
 * @param p The point
 * @param f The denominator
 * @return The divided point
 */
inline constexpr Point operator/(const Point &p, float f) {
  if (f == 0)
    return {0, 0};
  return {(int16_t)((float)p.x / f), (int16_t)((float)p.y / f)};
}

/**
 * @brief Divide a point by another term by term.
 * @param n Numerator point
 * @param d Denominator point
 * @return The division result.
 */
inline constexpr Point operator/(const Point &n, const Point &d) {
  if (d.x * d.y == 0)
    return {0, 0};
  return {(int16_t)(n.x / d.x), (int16_t)(n.y / d.y)};
}

/**
 * @brief Multiply a point by another term by term.
 * @param a First point
 * @param b Second point
 * @return The multiplication result.
 */
inline constexpr Point operator*(const Point &a, const Point &b) {
  return {(int16_t)(a.x * b.x), (int16_t)(a.y * b.y)};
}

/**
 * @brief Multiply a point by a number
 * @param f The number to multiply
 * @param p The point
 * @return The multiplication result.
 */
inline constexpr Point operator*(float f, const Point &p) {
  return {(int16_t)(f * (float)p.x), (int16_t)(f * (float)p.y)};
}
/**
 * @brief Multiply a point by a number
 * @param p The point
 * @param f The number to multiply
 * @return The multiplication result.
 */
inline constexpr Point operator*(const Point &p, float f) {
  return {(int16_t)((float)p.x * f), (int16_t)((float)p.y * f)};
}

/**
 * @brief Compute the minimum between 2 points term by term
 * @param a First value
 * @param b second value
 * @return The minimum
 */
inline constexpr Point min(const Point &a, const Point &b) {
  return {min(a.x, b.x), min(a.y, b.y)};
}

/**
 * @brief Compute the maximum between 2 points term by term
 * @param a First value
 * @param b second value
 * @return The maximum
 */
inline constexpr Point max(const Point &a, const Point &b) {
  return {max(a.x, b.x), max(a.y, b.y)};
}

/**
 * @brief Compute the clamped point term by term
 * @param p Point to evaluate
 * @param lower Lower bound
 * @param upper Upper bound
 * @return The clamped point
 */
inline constexpr Point clamp(const Point &p, const Point &lower,
                             const Point &upper) {
  return {clamp(p.x, lower.x, upper.x), clamp(p.y, lower.y, upper.y)};
}

} // namespace cpt::math
