/**
 * @author Silmaen
 * @date 14/07/2021
 */

#pragma once
#include <vector>

namespace obd::video {

/**
 * @brief Direction of Action of the 5key par
 */
enum struct NavDirection {
    Enter = 0x01, ///< Middle click: Enter
    Left  = 0x02, ///< Left click
    Right = 0x03, ///< Right click
    Up    = 0x04, ///< Up click
    Down  = 0x05, ///< Down click
};

/**
 * @brief Description of a menu item
 */
struct NavItem {
    uint8_t idSet       = 255; ///< Id of the NavItem if Enter is clicked
    uint8_t idLeftRight = 255; ///< Id of the NavItem if either left of right is clicked
    uint8_t idDown      = 255; ///< Id of the NavItem if Down is clicked
    uint8_t idUp        = 255; ///< Id of the NavItem if Up is clicked
};

/**
 * @brief List of the menus
 */
static const std::vector<NavItem> NavItems = {
        {1, 1, 42, 77},
        {1, 1, 2, 41},
        {2, 2, 3, 1},
        {3, 3, 4, 2},
        {4, 4, 5, 3},
        {5, 5, 6, 4},
        {6, 6, 7, 5},
        {7, 7, 8, 6},
        {8, 8, 9, 7},
        {9, 9, 10, 8},
        {10, 10, 11, 9},
        {11, 11, 12, 10},
        {12, 12, 13, 11},
        {13, 13, 14, 12},
        {14, 14, 15, 13},
        {15, 15, 16, 14},
        {16, 16, 17, 15},
        {17, 17, 18, 16},
        {18, 18, 19, 17},
        {19, 19, 20, 18},
        {20, 20, 21, 19},
        {21, 21, 22, 20},
        {22, 22, 23, 21},
        {23, 23, 24, 22},
        {24, 24, 25, 23},
        {25, 25, 26, 24},
        {26, 26, 27, 25},
        {27, 27, 28, 26},
        {28, 28, 29, 27},
        {29, 29, 30, 28},
        {30, 30, 31, 29},
        {31, 31, 32, 30},
        {32, 32, 33, 31},
        {33, 33, 34, 32},
        {34, 34, 35, 33},
        {35, 35, 36, 34},
        {36, 36, 37, 35},
        {37, 37, 38, 36},
        {38, 38, 39, 37},
        {39, 39, 40, 38},
        {0, 0, 41, 39},
        {0, 0, 1, 40},
        {43, 43, 47, 0},
        {43, 43, 44, 46},
        {44, 44, 45, 43},
        {45, 45, 46, 44},
        {42, 42, 43, 45},
        {48, 48, 54, 42},
        {48, 48, 49, 53},
        {49, 49, 50, 48},
        {50, 50, 51, 49},
        {51, 51, 52, 50},
        {52, 52, 53, 51},
        {47, 47, 48, 52},
        {55, 55, 61, 47},
        {55, 55, 56, 60},
        {56, 56, 57, 55},
        {57, 57, 58, 56},
        {58, 58, 59, 57},
        {59, 59, 60, 58},
        {54, 54, 55, 59},
        {61, 61, 62, 54},
        {63, 63, 76, 61},
        {63, 63, 64, 75},
        {64, 64, 65, 63},
        {66, 66, 67, 64},
        {65, 66, 66, 66},
        {68, 68, 70, 65},
        {67, 69, 69, 69},
        {67, 68, 68, 68},
        {71, 71, 72, 67},
        {70, 71, 71, 71},
        {73, 73, 75, 70},
        {72, 74, 74, 74},
        {72, 73, 73, 73},
        {62, 62, 63, 72},
        {76, 76, 77, 62},
        {255, 255, 0, 76}};

/**
 * @brief Simple class to simulate the navigation through the Loris menus
 */
class LorisMenu  {
public:
    /**
     * @brief Reinitialize the Menu when entering it
     */
    void enterMenu() {
        currentNav = 0;
    }

    /**
     * @brief Navigation in the menu
     * @param direction The button clicked
     * @return If the navigation goes out of the menus, return true else return false
     */
    bool moveMenu(const NavDirection& direction) {
        if (currentNav >= NavItems.size())
            return true;
        switch (direction) {
        case NavDirection::Enter:
            currentNav = NavItems[currentNav].idSet;
            break;
        case NavDirection::Left:
        case NavDirection::Right:
            currentNav = NavItems[currentNav].idLeftRight;
            break;
        case NavDirection::Up:
            currentNav = NavItems[currentNav].idUp;
            break;
        case NavDirection::Down:
            currentNav = NavItems[currentNav].idDown;
            break;
        }
        return currentNav >= NavItems.size();
    }

private:
    /// The position in the navigation
    uint8_t currentNav = 0;
};

}//namespace obd::video
