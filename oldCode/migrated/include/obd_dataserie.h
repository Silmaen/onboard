/**
 * \author Silmaen
 * \date 09/07/2021
 */

#pragma once
#include <algorithm>
#include <array>
#include <cmath>
#include <numeric>

namespace obd::data {

/**
 * @brief Simple class to manage looping data series
 * @tparam T Value's type stored.
 * @tparam size the size of the data pool
 */
template<typename T, uint8_t size>
class Series {
public:
    /**
     * @brief add data to the array
     * @param value
     */
    void appendData(T value) {
        data[index++] = value;
        if (index >= size) {
            index = 0;
            full  = true;
        }
    }

    /**
     * @brief Get the mean value of the series
     * @return The mean value
     */
    [[nodiscard]] T mean() const {
        return std::accumulate(data.begin(), trueEnd(), 0.0) / getLength();
    }

    /**
     * @brief Get the squared mean value of the series
     * @return The squared mean value
     */
    [[nodiscard]] T meanSQ() const {
        return std::accumulate(data.begin(), trueEnd(), 0.0, [](T accumulator, const T& val) { return accumulator + val * val; }) / getLength();
    }

    /**
     * @brief Get the variance of the series
     * @return The variance
     */
    [[nodiscard]] T variance() const {
        T m = mean();
        return std::accumulate(data.begin(), trueEnd(), 0.0, [&m](T accumulator, const T& val) { return accumulator + (val - m) * (val - m); }) / getLength();
    }

    /**
     * @brief Get the standard deviation of the series
     * @return The standard deviation
     */
    [[nodiscard]] T standardDeviation() const {
        return std::sqrt(variance());
    }

    /**
     * @brief Get the minimum element
     * @return The minimum
     */
    [[nodiscard]] T min() const {
        return *std::min_element(data.begin(), trueEnd());
    }

    /**
     * @brief Get the minimum element
     * @return The minimum
     */
    [[nodiscard]] T max() const {
        return *std::max_element(data.begin(), trueEnd());
    }

    /**
     * @brief Get the stored data number
     * @return Stored data number
     */
    [[nodiscard]] int getLength() const {
        return full ? size : index;
    }

    /**
     * @brief Get the next item's index
     * @return Next item's index
     */
    [[nodiscard]] int getIndex() const {
        return index;
    }
private:
    /**
     * @brief Get an iterator to the end of the data
     * @return Iterator to the end of the data
     */
    auto trueEnd() const {
        if (full)
            return data.end();
        return data.begin() + index;
    }
    /// The list of data
    std::array<T, size> data;
    /// Current index
    uint8_t index = 0;
    /// If the series reach the full set
    bool full = false;
};

}// namespace obd::data
