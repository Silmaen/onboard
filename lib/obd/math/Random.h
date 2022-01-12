/**
 * @file Random.h
 * @author argawaen
 * @date 03/01/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once
#ifdef ESP8266
#include <core_esp8266_features.h>
#define FULL_ALEA
#endif
#include <cstdint>

namespace obd::math {

/**
 * @brief A fast random number generator
 */
class Random {
public:
  /**
   * @brief Constructor
   * @param _seed Initial seed of the generator
   */
  explicit Random(uint64_t _seed = 0) {
    if (_seed == 0) {
#ifdef FULL_ALEA
      seed = micros64();
#else
      seed = 123456;
#endif
    } else {
      seed = _seed;
    }
  }
  /**
   * @brief Define the seed of the generator
   * @param s The new seed of the generator
   */
  inline void setSeed(const uint64_t &s) { seed = s; }
  /**
   * @brief Core random function
   * @return The new seed (new random number)
   */
  inline uint64_t rand() {
    // seed = (214013 * seed + 2531011);
    // return ((seed >> 16U) & 0x7FFFU);
    seed ^= seed << 13U;
    seed ^= seed >> 7U;
    seed ^= seed << 17U;
    return seed;
  }
  /**
   * @brief Get a random 64bit number in the given bound
   * @param bound The upper bound
   * @return Random number
   */
  inline uint64_t rand64(const uint64_t &bound) { return rand() % bound; }
  /**
   * @brief Get a random 32bit number in the given bound
   * @param bound The upper bound
   * @return Random number
   */
  inline uint32_t rand32(const uint32_t &bound) { return rand() % bound; }
  /**
   * @brief Get a random 16bit number in the given bound
   * @param bound The upper bound
   * @return Random number
   */
  inline uint16_t rand16(const uint16_t &bound) { return rand() % bound; }
  /**
   * @brief Get a random 8bit number in the given bound
   * @param bound The upper bound
   * @return Random number
   */
  inline uint8_t rand8(const uint8_t &bound) { return rand() % bound; }
  /**
   * @brief Get a random 64bit number in the given bound.
   * @param a Lower bound
   * @param b UpperBound
   * @return Random number
   */
  inline uint64_t rand64(const uint64_t &a, const uint64_t &b) {
    return (a + rand64(b - a + 1ULL));
  }
  /**
   * @brief Get a random 32bit number in the given bound.
   * @param a Lower bound
   * @param b UpperBound
   * @return Random number
   */
  inline uint32_t rand32(const uint32_t &a, const uint32_t &b) {
    return (static_cast<uint32_t>(a) + rand32(b - a + 1UL));
  }
  /**
   * @brief Get a random 16bit number in the given bound.
   * @param a Lower bound
   * @param b UpperBound
   * @return Random number
   */
  inline uint16_t rand16(const uint16_t &a, const uint16_t &b) {
    return (static_cast<uint16_t>(a + rand16(b - a + 1U)));
  }
  /**
   * @brief Get a random 8bit number in the given bound.
   * @param a Lower bound
   * @param b UpperBound
   * @return Random number
   */
  inline uint8_t rand8(const uint8_t &a, const uint8_t &b) {
    return (static_cast<uint8_t>(a + rand8(b - a + 1U)));
  }

private:
  /// Internal seed
  uint64_t seed;
};

} // namespace cpt::math
