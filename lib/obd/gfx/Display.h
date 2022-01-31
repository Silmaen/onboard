/**
 * @file Display.h
 * @author argawaen
 * @date 30/12/2021
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include "Color.h"
#include "core/driver/Node.h"
#include "math/Point.h"

namespace obd::gfx {

/**
 * @brief Predefined SPI clock speed
 */
enum struct SpiSpeed : uint32_t {
    SpiSlow   = 125000U,  ///< 125KHz
    SpiNormal = 4000000U, ///< 4MHz
    SpiFast   = 16000000U,///< 16MHz
    SpiCustom = 0,        ///< User defined... be careful!
};

/// The id of the
constexpr uint8_t ra8875_id = 0x75;

/**
 * @brief Class handling raw screen
 */
class Display : public core::driver::Node {
public:
    /**
     * @brief Constructor
     * @param parent The parent system
     * @param cs The Cable Select pin
     * @param rst The reset pin
     */
    explicit Display(std::shared_ptr<Messenger> parent = nullptr, uint8_t cs = 255, uint8_t rst = 255) :
        Node(parent),
        _cs{cs}, _rst{rst} {
    }

    /**
     * @brief Different display mode
     */
    enum struct Resolution {
        DM_480x80,  /*!<  480x80 Pixel Display */
        DM_480x128, /*!< 480x128 Pixel Display */
        DM_480x272, /*!< 480x272 Pixel Display */
        DM_800x480  /*!< 800x480 Pixel Display */
    };

    /**
   * @brief Initialize display
   * @param displayMode The display mode
   * @return False if something go wrong or display not
   * present
   */
    bool begin(const Resolution& displayMode = Resolution::DM_800x480);

    /**
   * @brief Define SPI speed
   * @param spd The predefined speed
   * @param custom_speed Custom speed if predefined is set to SpiCustom
   *
   * @note If the SPI lib of the device hasn't the transaction support,
   *       this function has no effect
   */
    void setSpiSpeed(const SpiSpeed& spd, uint32_t custom_speed = 4000000);

    /**
   * @brief Set display on or off
   * @param active The desired state of the screen.
   * @param sleep
   */
    void display(bool active, bool sleep = false);

    /**
   * @brief Get the screen's resolution
   * @return The resolution
   */
    [[nodiscard]] const math::Point& getResolution() const { return resolution; }

    /**
   * @brief Do a reset of the screen
   */
    void softReset();
    /**
   * @brief Do a hard reset of the screen (using reset pin)
   */
    void hardReset();

    /**
   * @brief Set the backlight
   * @param percent Percent of the luminosity
   */
    void backlight(uint8_t percent = 0);

    /**
   * @brief list od display modes
   */
    enum struct DisplayMode {
        Text,  ///< text mode
        Graphic///< graphical mode
    };

    /**
   * @brief Change the display mode
   * @param displayMode The new display mode
   */
    void mode(const DisplayMode& displayMode);

    /**
   * @brief Fill the screen with desired color
   * @param color component
   * @return True if execution is OK
   */
    [[nodiscard]] bool fillScreen(const Color& color) const;

    // ----- text functions -----
    /**
   * @brief Set the cursor blinking rate
   * @param rate the rate of blink
   *
   * Enable Cursor Visibility and Blink, here we set bits 6 and 5 in MWCR0[40h]
   * As well as the set the blink rate in BTCR[44h] the rate is 0 through max
   * 255 the lower the number the faster it blinks (00h is 1 frame time, FFh is
   * 256 Frames time. Blink Time (sec) = BTCR[44h]x(1/Frame_rate)
   */
    void textSetCursorBlink(uint8_t rate);
    /**
   * @brief Chang the position of the cursor
   * @param pos The new position of the cursor
   */
    void textSetCursor(const math::Point& pos);
    /**
   * @brief Define the new font color
   * @param color The color for the font
   * @param transparent If background is transparent
   * @param backColor The background color if not transparent
   */
    void textSetColor(const Color& color, bool transparent = true,
                      const Color& backColor = {0, 0, 0});
    /**
   * @brief Define the new text scale factor
   * @param scale The only values allowed are [0,1,2,3] respectively x1, x2, x4,
   * x4 zoom factor. (value higher than 3 will be treated as 3)
   */
    void textSetScale(uint8_t scale);
    /**
   * @brief Write the given text
   * @param str the text to write
   */
    void textWrite(const OString& str);

    // ----- status functions -----
    /**
   * @brief
   * Status register
   *   Bit 7 : Memory R/W Busy ?
   *   Bit 6 : BTE busy ?
   *   Bit 5 : Touch Panel touched ?
   *   Bit 4 : In Sleep Mode ?
   *   Bit 0 : Serial Flash/ROM busy ?
   * @return The status register
   */
    [[nodiscard]] uint8_t readStatus() const;

    /**
   * @brief Dump all status registers
   */
    void printStatusRegisters() const;

    /**
   * @brief Change the cursor position
   * @param pos The new position
   */
    void setPosition(const math::Point& pos) const;

    // ----- HW draw functions -----
    /**
   * @brief Function to paint a pixel
   * @param pos The position of the pixel
   * @param color The Color
   */
    void drawPixel(const math::Point& pos, const Color& color) const;

    /**
   * @brief Function to draw line
   * @param start The starting point
   * @param end The ending point
   * @param color The color
   * @return True if execution OK
   */
    [[nodiscard]] bool drawLine(const math::Point& start, const math::Point& end,
                                const Color& color) const;

    /**
   * @brief Function to draw triangle
   * @param point1 Coordinates of the first point
   * @param point2 Coordinates of the second point
   * @param point3 Coordinates of the third point
   * @param color The color
   * @param filled If filling curve
   * @return True if execution OK
   */
    [[nodiscard]] bool drawTriangle(const math::Point& point1, const math::Point& point2,
                                    const math::Point& point3, const Color& color,
                                    bool filled = false) const;

    /**
   * @brief Function to draw rectangle
   * @param topLeft The position of the top-left corner
   * @param bottomRight The position of the top-left corner
   * @param color The color
   * @param filled If filling curve
   * @return True if execution OK
   */
    [[nodiscard]] bool drawRectangle(const math::Point& topLeft,
                                     const math::Point& bottomRight,
                                     const Color& color,
                                     bool filled = false) const;

    /**
   * @brief Function to draw rectangle with rounded angle
   * @param topLeft The position of the top-left corner
   * @param bottomRight The position of the top-left corner
   * @param radius The radius of the corners
   * @param color The color
   * @param filled If filling curve
   * @return True if execution OK
   */
    [[nodiscard]] bool drawRoundRectangle(const math::Point& topLeft,
                                          const math::Point& bottomRight,
                                          uint16_t radius, const Color& color,
                                          bool filled = false) const;

    /**
   * @brief Function to draw circle
   * @param center The center of the circle
   * @param radius The radius of the circle
   * @param color The color
   * @param filled If filling curve
   * @return True if execution OK
   */
    [[nodiscard]] bool drawCircle(const math::Point& center, uint16_t radius,
                                  const Color& color, bool filled = false) const;

    /**
   * @brief Function to draw ellipse
   * @param center The center of the ellipse
   * @param longAxis The X axis
   * @param shortAxis The Y axis
   * @param color The color
   * @param filled If filling curve
   * @return True if execution OK
   */
    [[nodiscard]] bool drawEllipse(const math::Point& center, uint16_t longAxis,
                                   uint16_t shortAxis, const Color& color,
                                   bool filled = false) const;

    /**
   * @brief Part of the curve to draw
   */
    enum class CurvePart {
        BottomLeft  = 0x00,
        TopLeft     = 0x01,
        TopRight    = 0x02,
        BottomRight = 0x03
    };

    /**
   * @brief Function to draw curve (part of ellipse)
   * @param center The center of the ellipse
   * @param longAxis The X axis
   * @param shortAxis The Y axis
   * @param curvePart Which part of the Ellipse
   * @param color The color
   * @param filled If filling curve
   * @return True if execution OK
   */
    [[nodiscard]] bool drawCurve(const math::Point& center, uint16_t longAxis,
                                 uint16_t shortAxis, const CurvePart& curvePart,
                                 const Color& color, bool filled = false) const;

    // ----- Touch screen functions ---

    /// Types of touch screen modes
    enum struct TouchMode { Auto,
                            Manual,
                            off };

    /**
   * @brief Enable the touch screen mechanism
   * @param enable If enable or disable
   * @param mode Which touch mode
   */
    void touchEnable(bool enable, const TouchMode& mode = TouchMode::Auto);

    /**
   * @brief Check if the screen has been touched
   * @return True if touched
   */
    [[nodiscard]] bool touched();

    /**
   * @brief Read the touched position
   * @return The touched position
   */
    [[nodiscard]] math::Point touchRead();

    /**
   * @brief Clear the Touch screen interrupt engine
   */
    void clearTouch();

private:
    /// The Cable Select pin
    uint8_t _cs;
    /// The reset pin
    uint8_t _rst;
    /// The screen resolution
    math::Point resolution = {0, 0};
    /// The speed of the SPI clock in Hz
    uint32_t spi_speed = static_cast<uint8_t>(SpiSpeed::SpiNormal);
    /// Vertical offset
    uint8_t _voffset = 0;
    /// PWM Clock divider
    uint8_t _pwmClock = 0b1010;
    /// The mode of the touch screen
    TouchMode touchMode = TouchMode::Auto;

    /**
   * @brief Initialize the device
   */
    void initialize();

    /**
   * @brief Initialize the PLL
   */
    void PLLInit();

    /// The registers
    enum struct Registers {
        /// Register for getting the ID of the device: should be equal to 0x75
        RID = 0x00,
        /// Power and display control
        ///   Bit 7 : Display On ?
        ///   Bit 1 : Sleep Mode?
        ///   Bit 0 : Soft Reset
        PWRR = 0x01,
        MRWC = 0x02,
        /// Pixel Clock Setting
        ///   Bit 7 : PCLK Inversion: 0: rising, 1: falling
        ///   Bits 1-0 : PCLK Period 2^(val) * "system clock period"
        PCSR  = 0x04,
        SROC  = 0x05,
        SFCLR = 0x06,
        /// System Configuration Register
        ///   Bits 3-2: Color depth: 0b00: 8bpp (256 color), 0b1x 16bpp (65K color)
        ///   Bits 1-0: MCUIF Selection 0b00 8bit MCU, 0b1x 16bits MCU
        SYSR = 0x10,
        GPIR = 0x12,
        GPOR = 0x13,
        /// LCD Horizontal Display Width
        ///   Bits 6-0 Horizontal display width
        HDWR = 0x14,
        /// Horizontal non-display period fine Tuning Option
        ///   Bit 7 : DE Polarity, 0: high active, 1: low active
        ///   Bits 3-0 : Horizontal non-display period fine tuning
        HNDFTR = 0x15,
        HNDR   = 0x16,
        /// Horizontal Start Position
        ///   Bits 4-0 : HSYNC Start position
        HSTR = 0x17,
        /// HSYNC Pulse With
        ///   Bits 7 : HSYNC Polarity, 0: high active, 1: low active
        ///   Bits 4-0 : HSYNC pulse width
        HPWR = 0x18,
        /// LCD Vertical Display Height LSB
        ///   Bit 0-7 : 8 first bits of Vertical Display height
        VDHR0 = 0x19,
        /// LCD Vertical Display Height MSB
        ///   Bit 0 : 9th bit of Vertical Display height
        VDHR1 = 0x1a,
        /// LCD Vertical Non-Display Period LSB
        ///   Bit 0-7 : 8 first bits of Vertical Non-Display Period
        VNDR0 = 0x1b,
        /// LCD Vertical Non-Display Period MSB
        ///   Bit 0 : 9th bit of Vertical Non-Display Period
        VNDR1 = 0x1c,
        /// LCD Vertical Start Position LSB
        ///   Bit 0-7 : 8 first bits of Vertical Start Position
        VSTR0 = 0x1d,
        /// LCD Vertical Start Position MSB
        ///   Bit 0 : 9th bit of Vertical Start Position
        VSTR1 = 0x1e,
        /// VSYNC Pulse width
        ///   Bit 7 : DE Polarity, 0: high active, 1: low active
        ///   Bits 3-0 : Horizontal non-display period fine tuning
        VPWR = 0x1f,

        /// Font control register 0
        ///   Bit 7 : 0 ROM font, 1 RAM font
        ///   Bit 5 : 0 internal ROM font, 1 external rom font
        ///   Bits 1-0 : font selection (internal ROM) ISO 8859-x
        FNCR0 = 0x21,
        /// Font control register 1
        ///   Bit 7 : Full alignment selection enable?
        ///   Bit 6 : Font Transparancy
        ///   Bit 4 : Font rotation (1: 90 degree)
        ///   Bits 3-2 : Horizontal font Enlargement
        ///   Bits 1-0 : Vertical font Enlargement
        FNCR1 = 0x22,

        F_CURXL = 0x2a,
        F_CURXH = 0x2b,
        F_CURYL = 0x2c,
        F_CURYH = 0x2d,

        /// Horizontal Start Point of Active Window LSB
        HSAW0 = 0x30,
        /// Horizontal Start Point of Active Window MSB
        HSAW1 = 0x31,
        /// Vertical Start Point of Active Window LSB
        VSAW0 = 0x32,
        /// Vertical Start Point of Active Window MSB
        VSAW1 = 0x33,
        /// Horizontal end Point of Active Window LSB
        HEAW0 = 0x34,
        /// Horizontal end Point of Active Window LSB
        HEAW1 = 0x35,
        /// Vertical end Point of Active Window LSB
        VEAW0 = 0x36,
        /// Vertical end Point of Active Window MSB
        VEAW1 = 0x37,

        /// Memory Write Control Register 0
        ///   Bit 7 : TextMode Enabled?
        ///   Bit 6 : FontWrite Cursor/Memory write cursor visible?
        ///   Bit 5 : FontWrite Cursor/Memory write cursor Blink enable?
        ///   Bits 3-2 : Memory write direction:
        ///     0b00 L->R then T->D
        ///     0b01 R->L then T->D
        ///     0b10 T->D then L->R
        ///     0b11 D->T then L->R
        ///   Bit 1 : Memory Write cursor AutoIncrease Disabled?
        ///   Bit 0 : Memory Read cursor AutoIncrease Disabled?
        MWCR0 = 0x40,
        /// Memory Write Control Register 1
        ///   Bit 7 : Graphic Cursor Enabled?
        ///   Bits 6-4 : Graphic cursor type
        ///   Bits 3-2 : Write destination
        ///     0b00 Layer 1-2
        ///     0b01 CGRAM
        ///     0b10 Graphic cursor
        ///     0b11 Pattern
        ///   Bit 0 : Layer number for writing
        MWCR1 = 0x41,
        /// Blink time Control register
        ///   x + 1 frame time
        BTCR = 0x44,
        /// Memory read cursor direction
        ///   Bits 1-0:
        ///     0b00 L->R then T->D
        ///     0b01 R->L then T->D
        ///     0b10 T->D then L->R
        ///     0b11 D->T then L->R
        MRCD = 0x45,
        /// Memory Write cursor Horizontal Position 0
        CURH0 = 0x46,
        /// Memory Write cursor Horizontal Position 1
        CURH1 = 0x47,
        /// Memory Write cursor Vertical Position 0
        CURV0 = 0x48,
        /// Memory Write cursor Vertical Position 1
        CURV1 = 0x49,
        /// Background color red
        BGCR0 = 0x60,
        /// Background color green
        BGCR1 = 0x61,
        /// Background color blue
        BGCR2 = 0x62,
        /// Foreground color red
        FGCR0 = 0x63,
        /// Foreground color green
        FGCR1 = 0x64,
        /// Foreground color blue
        FGCR2 = 0x65,

        /// Touch panel control 0
        ///   Bit 7 : Touch panel enable
        ///   Bits 6-4 :TP Sample Time adfjust
        ///   Bit 3 : Touch Panel Wakeup enable
        ///   Bits 2-0 : ADC Clock setting
        TPCR0 = 0x70,
        /// Touch panel control 1
        ///   Bit 6 : TP Manual mode Enable
        ///   Bit 5 : TP ADC Reference voltage source
        ///   Bit 2 : De-bounce enable
        ///   Bits 1-0 :
        TPCR1 = 0x71,

        /// Touch Panel X High Byte Data bits(9-2)
        TPXH = 0x72,
        /// Touch Panel Y High Byte Data bits(9-2)
        TPYH = 0x73,
        /// Touch Panel X/Y Low Byte Data
        ///   Bit 7 ADET Touch event detector
        ///   Bits 3-2 : touch panel Y data bits (1-0)
        ///   Bits 1-0 : touch panel X data bits (1-0)
        TPXYL = 0x74,

        /// PLL Control 1
        ///   Bit 7 : PLLDIVM pre driver diviser 0: /1, 1: /2
        ///   Bits 4-0 : PLLDIVN, musut be in 1-31, 0 is forbidden
        PLLC1 = 0x88,
        /// PLL Control 2
        ///   Bits 2-0: PLLDIVK, PLL output divider power of 2
        PLLC2 = 0x89,

        /// PWM 1 Control
        ///   Bit 7 : 0: disable 1: enable
        ///   Bit 6 : Level if sleep or disable: 0: LOW, 1: HIGH
        ///   Bit 4 : function selection 0: function, 1: fixed frequency
        ///   Bits 3-0: Frequency divider (power of 2)
        P1CR = 0x8A,
        /// PWM 1 Duty cycle from 0x0=1/256 to 0xff=256/256
        P1DCR = 0x8B,
        /// PWM 2 Control
        ///   Bit 7 : 0: disable 1: enable
        ///   Bit 6 : Level if sleep or disable: 0: LOW, 1: HIGH
        ///   Bit 4 : function selection 0: function, 1: fixed frequency
        ///   Bits 3-0: Frequency divider (power of 2)
        P2CR = 0x8C,
        /// PWM 2 Duty cycle from 0x0=1/256 to 0xff=256/256
        P2DCR = 0x8D,

        /// Memory Clear Control
        ///   Bit 7 : Memory clear function 0 stop the clear, 1: start the clear
        ///   function
        ///   Bit 6 : Memory clear Area, 0 clear the full window, 1 clear active
        ///   window
        MCLR = 0x8E,

        /// Draw Line/Circle/Rectangle/triangle control
        ///   Bit 7 : Line, rectangle, triangle 1 start draw, 0 stop draw
        ///   Bit 6 : Circle, 1 start draw, 0 stop draw
        ///   Bit 5 : 0: no fill, 1: filled
        ///   Bit 4 : 0: draw line, 1: draw rectangle
        ///   Bit 0 : 0: draw line or rectangle, 1: draw triangle
        DCR = 0x90,

        DLHSR0 = 0x91,
        DLHSR1 = 0x92,
        DLVSR0 = 0x93,
        DLVSR1 = 0x94,

        DLHER0 = 0x95,
        DLHER1 = 0x96,
        DLVER0 = 0x97,
        DLVER1 = 0x98,

        /// Draw Circle center X bits 7-0
        DCHR0 = 0x99,
        /// Draw Circle center X bits 9-8
        DCHR1 = 0x9a,
        /// Draw Circle center Y bits 7-0
        DCVR0 = 0x9b,
        /// Draw Circle center Y bits 9-8
        DCVR1 = 0x9c,
        /// Draw Circle radius
        DCRR = 0x9d,

        /// Draw Ellipse/Ellipse Curve/Circle Square Control
        ///   Bit 7 : 1 start drawing, 0 stop drawing
        ///   Bit 6 : Fill the shape?
        ///   Bit 5 : 1 draw circle square, 0 draw Ellipse,ellipse curve
        ///   Bit 4 : 0 draw ellipse, 1 draw Ellipse Curve
        ///   Bits 10 : Draw ellipse part
        DECSC = 0xA0,

        /// Draw Ellipse/Circle Square Long axis Setting [7-0]
        ELL_A0 = 0xA1,
        /// Draw Ellipse/Circle Square Long axis Setting [9-8]
        ELL_A1 = 0xA2,
        /// Draw Ellipse/Circle Square short axis Setting [7-0]
        ELL_B0 = 0xA3,
        /// Draw Ellipse/Circle Square short axis Setting [9-8]
        ELL_B1 = 0xA4,
        /// Draw Ellipse/Circle Square Center Horizontal Address [7-0]
        DEHR0 = 0xA5,
        /// Draw Ellipse/Circle Square Center Horizontal Address [9-8]
        DEHR1 = 0xA6,
        /// Draw Ellipse/Circle Square Center Vertical Address [7-0]
        DEVR0 = 0xA7,
        /// Draw Ellipse/Circle Square Center Vertical Address [9-8]
        DEVR1 = 0xA8,
        /// Draw Triangle last point X [7-0]
        DTPH0 = 0xA9,
        /// Draw Triangle last point X [9-8]
        DTPH1 = 0xAA,
        /// Draw Triangle last point Y [7-0]
        DTPV0 = 0xAB,
        /// Draw Triangle last point Y [9-8]
        DTPV1 = 0xAC,

        /// Extra general purpose IO Register
        GPIOX = 0xC7,

        /// Interupt control register 1
        ///  Bit 4 : KEYScan Interrupt enable bit
        ///  Bit 3 : DMA interrupt enable bit
        ///  Bit 2 : Touch panel Enable bit
        ///  Bit 1 : BTE Process Complete
        ///  Bit 0 : bla bla
        INTC1 = 0xF0,
        /// Interupt control register 2
        ///  Bit 4 : KEYScan Interrupt clear bit
        ///  Bit 3 : DMA interrupt clear bit
        ///  Bit 2 : Touch panel clear bit
        ///  Bit 1 : BTE Process clear
        ///  Bit 0 : bla bla
        INTC2 = 0xF1,
    };

    /**
   * @brief Print the requested Register
   * @param reg The requested Register
   */
    void printRegister(const Registers& reg) const;
    /// Begins SPI transfer
    void spiBegin() const;
    /// Ends SPI transfer
    void spiEnd() const;
    /**
   * @brief Write command in the given Register
   * @param c the Register
   */
    void writeCommand(const Registers& c) const;
    /**
   * @brief Send raw data
   * @param data The data to send
   */
    void writeData(const std::vector<uint8_t>& data) const;
    /**
   * @brief Send raw data
   * @param data The data to send
   */
    void writeData(uint8_t data) const;
    /**
   * @brief Send raw data
   * @param data The data to send
   */
    void writeData16(uint16_t data) const;
    /**
   * @brief Write data at the given Register
   * @param reg The Register
   * @param data The data to write
   */
    void writeReg(const Registers& reg, const std::vector<uint8_t>& data) const;
    /**
   * @brief Write data at the given Register
   * @param reg The Register
   * @param data The data to write
   */
    void writeReg(const Registers& reg, uint8_t data) const;
    /**
   * @brief Write data at the given Register
   * @param reg The Register
   * @param data The data to write
   */
    void writeReg16(const Registers& reg, uint16_t data) const;
    /**
   * @brief Write color at the given Register
   * @param reg The Register
   * @param data The data to write
   */
    void writeReg(const Registers& reg, const Color& data) const;
    /**
   * @brief Get the requested Register value
   * @param reg The Register
   * @return The Value
   */
    [[nodiscard]] uint8_t readReg(const Registers& reg) const;
    /**
   * @brief Read data at the current pointer
   * @return The data
   */
    [[nodiscard]] uint8_t readData() const;
    /**
   * @brief Helper function used to wait for drawing to finish
   * @param reg The Drawing Register
   * @param waitFlag The flag indicating operation accomplished
   * @param timeout Time out
   * @return True if OK, False if fallback to timeout
   */
    [[nodiscard]] bool waitPoll(const Registers& reg, uint8_t waitFlag,
                                uint64_t timeout = 100) const;

    // drawing helpers
    /**
   * @brief Draw a rectangle
   * @param topLeft Starting point
   * @param bottomRight Ending point
   * @param color The color
   * @param filled iIf the rectangle should be filled
   * @return True if execution is OK
   */
    [[nodiscard]] bool rectHelper(const math::Point& topLeft,
                                  const math::Point& bottomRight,
                                  const Color& color, bool filled = false) const;

    /**
   * @brief Send command to draw an Ellipse
   * @param center The center of the ellipse
   * @param longAxis The X axis of the ellipse
   * @param shortAxis  The Y axis of the ellipse
   * @param curvePart The curve part to draw
   * @param color The color of the ellipse
   * @param filled If the ellipse should be filled.
   * @return True if execution is OK.
   */
    [[nodiscard]] bool ellipseHelper(const math::Point& center, uint16_t longAxis,
                                     uint16_t shortAxis, uint8_t curvePart,
                                     const Color& color,
                                     bool filled = false) const;
};

}// namespace obd::gfx
