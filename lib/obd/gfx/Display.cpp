/**
 * @file Display.cpp
 * @author argawaen
 * @date 30/12/2021
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "gfx/Display.h"
#ifdef ARDUINO
#include <SPI.h>
#endif

namespace obd::gfx {

bool Display::begin(const Resolution& displayMode) {
    if (displayMode == Resolution::DM_480x80) {
        resolution.x = 480;
        resolution.y = 80;
    } else if (displayMode == Resolution::DM_480x128) {
        resolution.x = 480;
        resolution.y = 128;
    } else if (displayMode == Resolution::DM_480x272) {
        resolution.x = 480;
        resolution.y = 272;
    } else if (displayMode == Resolution::DM_800x480) {
        resolution.x = 800;
        resolution.y = 480;
    } else {
        return false;
    }
#ifdef ARDUINO
    if (_cs != 255) {
        pinMode(_cs, OUTPUT);
        digitalWrite(_cs, HIGH);
        hardReset();

        SPI.begin();
    }
#endif
    setSpiSpeed(SpiSpeed::SpiSlow);
    uint8_t idReg = readReg(Registers::RID);
    if (idReg != ra8875_id) {// check if we really have a RA8875 online!!
        Message msg(type(),getConsoleId());
        msg.print("ERROR no RA8875 device found: ");
        msg.println(idReg);
        msg.setType(Message::MessageType::Error);
        broadcastMessage(msg);
        return false;
    }
    // now initialize the device!
    initialize();
    // Set SPI clock to normal speed
    setSpiSpeed(SpiSpeed::SpiNormal);
    return true;
}

void Display::PLLInit() {
    // System clock value is
    // SYS_CLK = Fin * (PLLDIVN +1)/((PLLDIVM+1)*(2^PLLDIVK))
    // Fin is given by the external cristal
    // PLLDIVM : bit 7 of register PLLC1
    // PLLDIVN : bits 4-0 of register PLLC1
    // PLLDIVK : bits 2-0 of register PLLC2
    // min SYS_clock is 1MHz max is 60MHz (typical is 20-30MHz)
    // Fin is typically 15-30MHz

    uint8_t PllDivM = 0;
    uint8_t PllDivN = 10;
    uint8_t PllDivK = 0x02;// divide by 4

    if (resolution.y != 480) {
        writeReg(Registers::PLLC1, (PllDivM << 7) + PllDivN);
#ifdef ARDUINO
        delay(1);
#endif
        writeReg(Registers::PLLC2, PllDivK);
#ifdef ARDUINO
        delay(1);
#endif
        // SYS_CLK =
    } else /* (_size == RA8875_800x480) */ {
        writeReg(Registers::PLLC1, (PllDivM << 7) + PllDivN + 1);
#ifdef ARDUINO
        delay(1);
#endif
        writeReg(Registers::PLLC2, PllDivK);
#ifdef ARDUINO
        delay(1);
#endif
    }
}

void Display::initialize() {
    PLLInit();
    //
    // System Register
    // set to 16bit color, 8 bit MCU. (values from adafruit, todo: Test other
    // values!)
    writeReg(Registers::SYSR, 0b00001100);

    //
    // Pixel clock
    // (values from adafruit, todo: Test other values!)
    uint8_t pixclk         = 0x80;// falling edge
    uint8_t hsync_nondisp  = 10;  // I don't know the effect.
    uint8_t hsync_start    = 8;   // I don't know the effect.
    uint8_t hsync_pw       = 48;  // I don't know the effect.
    uint16_t vsync_nondisp = 3;   // I don't know the effect.
    uint16_t vsync_start   = 8;   // I don't know the effect.
    uint8_t vsync_pw       = 10;  // I don't know the effect.
    if (resolution.y == 480) {
        pixclk += 0b10;// divide sys_clock frequency by 2
        hsync_nondisp = 26;
        hsync_start   = 32;
        hsync_pw      = 96;
        vsync_nondisp = 32;
        vsync_start   = 23;
        vsync_pw      = 2;
    }
    writeReg(Registers::PCSR, pixclk);
#ifdef ARDUINO
    delay(1);
#endif

    //
    // Horizontal settings
    // H width: (HDWR + 1) * 8
    writeReg(Registers::HDWR, (resolution.x / 8) - 1);
    writeReg(Registers::HNDFTR, 0);
    // H non-display: HNDR * 8 + HNDFTR + 2 = 10
    writeReg(Registers::HNDR, (hsync_nondisp - 2) / 8);
    // Hsync start: (HSTR + 1)*8
    writeReg(Registers::HSTR, hsync_start / 8 - 1);
    // HSync pulse width = (HPWR+1) * 8
    writeReg(Registers::HPWR, (hsync_pw / 8 - 1));

    //
    // Vertical settings
    _voffset = 0;
    if (resolution.y == 80)
        _voffset = 192;
    writeReg16(Registers::VDHR0, static_cast<uint16_t>(resolution.y - 1 + _voffset));
    // V non-display period = VNDR + 1
    writeReg16(Registers::VNDR0, vsync_nondisp - 1);
    // Vsync start position = VSTR + 1
    writeReg16(Registers::VSTR0, vsync_start - 1);
    // Vsync pulse width = VPWR + 1
    writeReg(Registers::VPWR, vsync_pw - 1);

    //
    // Active Window X
    // horizontal start point
    writeReg16(Registers::HSAW0, 0);
    // horizontal end point
    writeReg16(Registers::HEAW0, static_cast<uint16_t>(resolution.x - 1));

    //
    // Active Window Y
    // vertical start point
    writeReg16(Registers::VSAW0, 0 + _voffset);
    // vertical end point
    writeReg16(Registers::VEAW0, static_cast<uint16_t>(resolution.y - 1 + _voffset));

    //
    // Backlight
    writeReg(Registers::GPIOX, 1);
    writeReg(Registers::P1CR, (_pwmClock & 0xF));
    writeReg(Registers::P1DCR, 0);
    writeReg(Registers::P2CR, (_pwmClock & 0xF));
    writeReg(Registers::P2DCR, 0);

    //
    // Touch panel ?

    //
    // Clear the screen (full memory)
    writeReg(Registers::MCLR, 0x80);
#ifdef ARDUINO
    delay(500);
#endif
}

void Display::display(bool active, bool sleep) {
    if (sleep) {
        writeReg(Registers::PWRR, 0x02);
        writeReg(Registers::GPIOX, 0);
    } else {
        if (active) {
            writeReg(Registers::PWRR, 0x80);

        } else {
            writeReg(Registers::PWRR, 0x00);
            writeReg(Registers::GPIOX, 0);
        }
    }
}

void Display::softReset() {
    writeCommand(Registers::PWRR);
    uint8_t data = readData();
    writeData(0x01);
    writeData(data);
#ifdef ARDUINO
    delay(1);
#endif
}

void Display::hardReset() {
    if (_rst == 255)
        return;
#ifdef ARDUINO
    pinMode(_rst, OUTPUT);
    digitalWrite(_rst, HIGH);
    digitalWrite(_rst, LOW);
    delay(100);
    digitalWrite(_rst, HIGH);
    delay(100);
#endif
}

void Display::backlight(uint8_t percent) {
    if (percent == 0) {
        writeReg(Registers::P1CR, (_pwmClock & 0xF));
        writeReg(Registers::P1DCR, 0);
    } else {
        writeReg(Registers::P1CR, 0x80 | (_pwmClock & 0xF));
        writeReg(Registers::P1DCR, static_cast<uint8_t>(static_cast<uint16_t>(percent * 255) / 100));
    }
}

void Display::mode(const DisplayMode& displayMode) {
    writeCommand(Registers::MWCR0);
    uint8_t temp = readData();
    switch (displayMode) {
    case DisplayMode::Text:
        temp |= 0x80;// Set bit 7
        /* Select the internal (ROM) font */
        writeData(temp);
        writeCommand(Registers::FNCR0);
        temp = readData();
        temp &= ~((1 << 7) | (1 << 5));// Clear bits 7 and 5
        writeData(temp);
        break;
    case DisplayMode::Graphic:
        temp &= ~0x80;// clear bit #7
        writeData(temp);
        break;
    }
}

// =================== SPI functions ===========================================

/// Message for writing data
constexpr uint16_t RA8875DataWrite = 0x00;
/// Message for reading data
constexpr uint16_t RA8875DataRead = 0x40;
/// Message for writing command
constexpr uint16_t RA8875CmdWrite = 0x80;
/// Message for reading command
constexpr uint16_t RA8875CmdRead = 0xC0;

void Display::spiBegin() const {
#ifdef ARDUINO
    digitalWrite(_cs, LOW);
#ifdef SPI_HAS_TRANSACTION
    // max speed!
    SPI.beginTransaction(SPISettings(spi_speed, MSBFIRST, SPI_MODE0));
#endif
#endif
}

void Display::spiEnd() const {
#ifdef ARDUINO
#ifdef SPI_HAS_TRANSACTION
    SPI.endTransaction();
#endif
    digitalWrite(_cs, HIGH);
#endif
}
#ifdef ARDUINO
void Display::writeCommand(const Registers& c) const {
    spiBegin();
    SPI.transfer(RA8875CmdWrite);
    SPI.transfer(static_cast<uint8_t>(c));
    spiEnd();
#else
void Display::writeCommand(const Registers&) const {
#endif
}

#ifdef ARDUINO
void Display::writeData(const std::vector<uint8_t>& data) const {
    spiBegin();
    SPI.transfer(RA8875DataWrite);
    for (auto& d : data) {
        SPI.transfer(d);
    }
    spiEnd();
#else
void Display::writeData(const std::vector<uint8_t>&) const {
#endif
}

#ifdef ARDUINO
void Display::writeData(uint8_t data) const {
    spiBegin();
    SPI.transfer(RA8875DataWrite);
    SPI.transfer(data);
    spiEnd();
#else
void Display::writeData(uint8_t) const {
#endif
}

#ifdef ARDUINO
void Display::writeData16(uint16_t data) const {
    spiBegin();
    SPI.transfer(RA8875DataWrite);
    SPI.transfer(data >> 8);
    SPI.transfer(data);
    spiEnd();
#else
void Display::writeData16(uint16_t) const {
#endif
}

void Display::writeReg(const Registers& reg,
                       const std::vector<uint8_t>& data) const {
    writeCommand(reg);
    writeData(data);
}

void Display::writeReg(const Display::Registers& reg, uint8_t data) const {
    writeCommand(reg);
    writeData(data);
}

#ifdef ARDUINO
void Display::writeReg16(const Display::Registers& reg, uint16_t data) const {
    // writeCommand(reg);
    // writeData((uint8_t)(data & 0xFF));
    // writeData((uint8_t)(data >> 8U));
    spiBegin();
    SPI.transfer(RA8875CmdWrite);
    SPI.transfer(static_cast<uint8_t>(reg));
    spiEnd();
    spiBegin();
    SPI.transfer(RA8875DataWrite);
    SPI.transfer((uint8_t) (data & 0xFF));
    spiEnd();
    spiBegin();
    SPI.transfer(RA8875CmdWrite);
    SPI.transfer(static_cast<uint8_t>(reg) + 1);
    spiEnd();
    spiBegin();
    SPI.transfer(RA8875DataWrite);
    SPI.transfer((uint8_t) (data >> 8U));
    spiEnd();
#else
void Display::writeReg16(const Display::Registers&, uint16_t) const {
#endif
}

#ifdef ARDUINO
void Display::writeReg(const Registers& reg, const Color& data) const {
    spiBegin();
    SPI.transfer(RA8875CmdWrite);
    SPI.transfer(static_cast<uint8_t>(reg));
    spiEnd();
    spiBegin();
    SPI.transfer(RA8875DataWrite);
    SPI.transfer(data.red5());
    spiEnd();
    spiBegin();
    SPI.transfer(RA8875CmdWrite);
    SPI.transfer(static_cast<uint8_t>(reg) + 1);
    spiEnd();
    spiBegin();
    SPI.transfer(RA8875DataWrite);
    SPI.transfer(data.green6());
    spiEnd();
    spiBegin();
    SPI.transfer(RA8875CmdWrite);
    SPI.transfer(static_cast<uint8_t>(reg) + 2);
    spiEnd();
    spiBegin();
    SPI.transfer(RA8875DataWrite);
    SPI.transfer(data.blue5());
    spiEnd();
#else
void Display::writeReg(const Registers&, const Color&) const {
#endif
}

uint8_t Display::readStatus() const {
#ifdef ARDUINO
    spiBegin();
    SPI.transfer(RA8875CmdRead);
    uint8_t x = SPI.transfer(0x0);
    spiEnd();
    return x;
#else
    return 0;
#endif
}

uint8_t Display::readReg(const Registers& reg) const {
    writeCommand(reg);
    return readData();
}

uint8_t Display::readData() const {
#ifdef ARDUINO
    spiBegin();
    SPI.transfer(RA8875DataRead);
    uint8_t x = SPI.transfer(0x0);
    spiEnd();
    return x;
#else
    return 0;
#endif
}

void Display::setSpiSpeed(const SpiSpeed& spd, uint32_t custom_speed) {
    if (spd == SpiSpeed::SpiCustom) {
        spi_speed = custom_speed;
    } else {
        spi_speed = static_cast<uint32_t>(spd);
    }
}

void Display::printRegister(const Registers& reg) const {
    Message msg(type(),getConsoleId());
    auto val = static_cast<uint8_t>(reg);
    msg.print("0x");
    msg.print(val, Message::Format::Hexadecimal);
    msg.print(" : ");
    val = readReg(reg);
    msg.print("0x");
    msg.print(val, Message::Format::Hexadecimal);
    msg.print("  --  ");
    msg.println(val, Message::Format::Binary);
    broadcastMessage(msg);
}

void Display::printStatusRegisters() const {
    Message msg(type(),getConsoleId());
    msg.print("Status : ");
    uint8_t val = readStatus();
    msg.print("0x");
    msg.println(val, Message::Format::Hexadecimal);
    broadcastMessage(msg);
    std::vector<Registers> regs = {
            Registers::RID,
            Registers::PWRR,
            Registers::MRWC,
            Registers::PCSR,
            Registers::SROC,
            Registers::SFCLR,
            Registers::SYSR,
            Registers::GPIR,
            Registers::GPOR,
            Registers::HDWR,
            Registers::HNDFTR,
            Registers::HNDR,
            Registers::HSTR,
            Registers::HPWR,
            Registers::VDHR0,
            Registers::VDHR1,
            Registers::VNDR0,
            Registers::VNDR1,
            Registers::VSTR0,
            Registers::VSTR1,
            Registers::VPWR,
    };
    for (auto& reg : regs)
        printRegister(reg);
}

#ifdef ARDUINO
bool Display::waitPoll(const Registers& reg, uint8_t waitFlag,
                       uint64_t timeout) const {
    /* Wait for the command to finish */
    uint64_t start = millis();
    while (millis() - start < timeout) {
        uint8_t temp = readReg(reg);
        if (!(temp & waitFlag))
            return true;
    }
    return false;
#else
bool Display::waitPoll(const Registers&, uint8_t, uint64_t) const {
    /* Wait for the command to finish */
    return true;
#endif
}
// ==================== Text functions =========================================

void Display::textSetCursorBlink(uint8_t rate) {
    writeCommand(Registers::MWCR0);
    writeData(readData() | 0x60);// setting bit 6 & 5
    writeCommand(Registers::BTCR);
    writeData(rate);
}

void Display::textSetCursor(const math::Point& pos) {
    writeReg16(Registers::F_CURXL, pos.x);
    writeReg16(Registers::F_CURYL, pos.y);
}

void Display::textSetColor(const Color& color, bool transparent,
                           const Color& backColor) {
    /* Set Fore Color */
    writeReg(Registers::FGCR0, color);
    if (transparent) {
        /* Set transparency flag */
        writeCommand(Registers::FNCR1);
        writeData(readData() | (1 << 6));// Set bit 6
    } else {
        /* Set Background Color */
        writeReg(Registers::BGCR0, backColor);
        /* Clear transparency flag */
        writeCommand(Registers::FNCR1);
        writeData(readData() & ~(1 << 6));// Clear bit 6
    }
}

void Display::textSetScale(uint8_t scale) {
    if (scale > 3)
        scale = 3;// highest setting is 3
    /* Set font size flags */
    writeCommand(Registers::FNCR1);
    uint8_t temp = readData();
    temp &= ~(0xF);// Clears bits 0..3
    temp |= scale << 2;
    temp |= scale;
    writeData(temp);
}

void Display::textWrite(const OString& str) {
    writeCommand(Registers::MRWC);
    for (auto writeChar : str) {
        writeData(writeChar);
#ifdef ARDUINO
        delay(1);// small delay to let write
#endif
    }
}

// ----- Touch screen functions ---

void Display::touchEnable(bool enable, const TouchMode& mode) {
    uint8_t adcClk = 0x02;

    if (resolution.y == 480)// match up touch size with LCD size
        adcClk = 0x04;

    if (enable && mode != TouchMode::off) {
        touchMode = mode;
        // set (Reg 0x70)
        // enable Touch panel 0x80
        // sample time: 4096 block  0x30
        // Wakeup enable 0x08
        // ---> 0xb3
        writeReg(Registers::TPCR0, 0xb3 | adcClk);// 10mhz max!
        // Set (Reg 0x71)
        //   auto mode     Bit 6 =0
        //   and debounce  bit 2 = 1
        //     manual mode: wait for TP: Bits1-0 : 0b01
        uint8_t reg = (mode == TouchMode::Manual) ? 0x21 : 0 + 0b10;
        writeReg(Registers::TPCR1, reg);
        /* Enable TP INT */
        writeReg(Registers::INTC1, readReg(Registers::INTC1) | 0x04);
    } else {
        touchMode = TouchMode::off;
        /* Disable TP INT */
        writeReg(Registers::INTC1, readReg(Registers::INTC1) & ~0x04);
        /* Disable Touch Panel (Reg 0x70) */
        writeReg(Registers::TPCR0, 0x00);
    }
}

void Display::clearTouch() {
    writeReg(Registers::INTC2, readReg(Registers::INTC2) & 0b11110111);
}

[[nodiscard]] bool Display::touched() {
    return (readReg(Registers::INTC2) & 0x04) != 0;
}

[[nodiscard]] math::Point Display::touchRead() {
    if (!touched() || touchMode == TouchMode::off)
        return {-1, -1};
    if (touchMode == TouchMode::Manual) {
        uint8_t tpcr1 = readReg(Registers::TPCR1) & 0b11111100;
        // set state to "Latch X Data"
        writeReg(Registers::TPCR1, tpcr1 | 0b10);
#ifdef ARDUINO
        delayMicroseconds(50);
#endif
        // set state to "Latch X Data"
        writeReg(Registers::TPCR1, tpcr1 | 0b11);
#ifdef ARDUINO
        delayMicroseconds(50);
#endif
    }
    uint16_t touchX = readReg(Registers::TPXH);
    uint16_t touchY = readReg(Registers::TPYH);
    uint16_t temp   = readReg(Registers::TPXYL);
    touchX <<= 2;
    touchY <<= 2;
    touchX |= temp & 0x03;       // get the bottom x bits
    touchY |= (temp >> 2) & 0x03;// get the bottom y bits
    // encoded on 9bits : [0-1023]
    // correct ratio to convert into pixel
    // x is [50 - 950] -> [0 , resolution.x]
    touchX = static_cast<uint16_t>(static_cast<float>(touchX - 50) * static_cast<float>(resolution.x) / 900.0);
    // y is [150 - 900] -> [0 , resolution.x]
    touchY = static_cast<uint16_t>(static_cast<float>(touchY - 150) * static_cast<float>(resolution.y) / 750.0);

    clearTouch();
    if (touchMode == TouchMode::Manual) {
        uint8_t tpcr1 = readReg(Registers::TPCR1) & 0b11111100;
        // reset state to "wait for TP"
        writeReg(Registers::TPCR1, tpcr1 | 0b01);
    }
    return {math::clamp(static_cast<int16_t>(touchX), 0, resolution.x),
            math::clamp(static_cast<int16_t>(touchY), 0, resolution.y)};
}

// ==================== Draw functions =========================================

void Display::setPosition(const math::Point& pos) const {
    writeReg16(Registers::CURH0, pos.x);
    writeReg16(Registers::CURV0, pos.y);
}

bool Display::fillScreen(const Color& color) const {
    return rectHelper({0, 0}, resolution, color, true);
}

bool Display::rectHelper(const math::Point& topLeft,
                         const math::Point& bottomRight, const Color& color,
                         bool filled) const {
    math::Point lower = min(topLeft, bottomRight);
    math::Point upper = max(topLeft, bottomRight);
    writeReg16(Registers::DLHSR0, lower.x);
    writeReg16(Registers::DLVSR0, lower.y);
    writeReg16(Registers::DLHER0, upper.x);
    writeReg16(Registers::DLVER0, upper.y);
    writeReg(Registers::FGCR0, color);
    if (filled) {
        writeReg(Registers::DCR, 0xB0);
    } else {
        writeReg(Registers::DCR, 0x90);
    }
    /* Wait for the command to finish */
    return waitPoll(Registers::DCR, 0x80);
}

void Display::drawPixel(const math::Point& pos, const Color& color) const {
    writeReg16(Registers::CURH0, pos.x);
    writeReg16(Registers::CURV0, pos.y);
    writeCommand(Registers::MRWC);
    writeData16(color.toRGB565());
}

bool Display::drawLine(const math::Point& start, const math::Point& end,
                       const Color& color) const {
    /* Set X */
    writeReg16(Registers::DLHSR0, start.x);
    /* Set Y */
    writeReg16(Registers::DLVSR0, start.y);
    /* Set X1 */
    writeReg16(Registers::DLHER0, end.x);
    /* Set Y1 */
    writeReg16(Registers::DLVER0, end.y);

    /* Set Color */
    writeReg(Registers::FGCR0, color);

    /* Draw! */
    writeReg(Registers::DCR, 0x80);

    /* Wait for the command to finish */
    return waitPoll(Registers::DCR, 0x80);
}

bool Display::drawTriangle(const math::Point& point1, const math::Point& point2,
                           const math::Point& point3, const Color& color,
                           bool filled) const {
    /* Set Point 0 */
    writeReg16(Registers::DLHSR0, point1.x);
    writeReg16(Registers::DLVSR0, point1.y);

    /* Set Point 1 */
    writeReg16(Registers::DLHER0, point2.x);
    writeReg16(Registers::DLVER0, point2.y);

    /* Set Point 2 */
    writeReg16(Registers::DTPH0, point3.x);
    writeReg16(Registers::DTPV0, point3.y);

    /* Set Color */
    writeReg(Registers::FGCR0, color);

    /* Draw! */
    writeCommand(Registers::DCR);
    if (filled) {
        writeData(0xA1);
    } else {
        writeData(0x81);
    }

    /* Wait for the command to finish */
    return waitPoll(Registers::DCR, 0x80);
}

bool Display::drawRectangle(const math::Point& topLeft,
                            const math::Point& bottomRight, const Color& color,
                            bool filled) const {
    return rectHelper(topLeft, bottomRight, color, filled);
}

bool Display::drawRoundRectangle(const math::Point& topLeft,
                                 const math::Point& bottomRight, uint16_t radius,
                                 const Color& color, bool filled) const {
    math::Point lower =
            clamp(min(topLeft, bottomRight), {0, 0}, resolution - math::Point{1, 1});
    math::Point upper =
            clamp(max(topLeft, bottomRight), {0, 0}, resolution - math::Point{1, 1});

    writeReg16(Registers::DLHSR0, lower.x);
    writeReg16(Registers::DLVSR0, lower.y);
    writeReg16(Registers::DLHER0, upper.x);
    writeReg16(Registers::DLVER0, upper.y);
    writeReg16(Registers::ELL_A0, radius);
    writeReg16(Registers::ELL_B0, radius);
    writeReg(Registers::FGCR0, color);
    /* Draw! */
    writeCommand(Registers::DECSC);
    if (filled) {
        writeData(0xE0);
    } else {
        writeData(0xA0);
    }
    /* Wait for the command to finish */
    return waitPoll(Registers::DECSC, 0x80);
}

bool Display::drawCircle(const math::Point& center, uint16_t radius,
                         const Color& color, bool filled) const {
    /* Set X */
    writeReg16(Registers::DCHR0, center.x);

    /* Set Y */
    writeReg16(Registers::DCVR0, center.y);

    /* Set Radius */
    writeReg(Registers::DCRR, radius);

    /* Set Color */
    writeReg(Registers::FGCR0, color);

    /* Draw! */
    writeCommand(Registers::DCR);
    if (filled) {
        writeData(0x60);
    } else {
        writeData(0x40);
    }

    /* Wait for the command to finish */
    return waitPoll(Registers::DCR, 0x40);

    // alternate method using ellipse ellipse (Todo: test the fastest):
    // return ellipseHelper(center,radius,radius,5,color,filled);
}

bool Display::drawEllipse(const math::Point& center, uint16_t longAxis,
                          uint16_t shortAxis, const Color& color,
                          bool filled) const {
    return ellipseHelper(center, longAxis, shortAxis, 5, color, filled);
}

bool Display::drawCurve(const math::Point& center, uint16_t longAxis,
                        uint16_t shortAxis, const CurvePart& curvePart,
                        const Color& color, bool filled) const {
    auto curvePart8 = static_cast<uint8_t>(curvePart);
    return ellipseHelper(center, longAxis, shortAxis, curvePart8, color, filled);
}

bool Display::ellipseHelper(const math::Point& center, uint16_t longAxis,
                            uint16_t shortAxis, uint8_t curvePart, const Color& color,
                            bool filled) const {

    /* Set Center Point */
    writeCommand(Registers::DEHR0);
    writeData(center.x & 0xff);
    writeCommand(Registers::DEHR1);
    writeData(center.x >> 8);
    writeCommand(Registers::DEVR0);
    writeData(center.y & 0xff);
    writeCommand(Registers::DEVR1);
    writeData(center.y >> 8);

    /* Set Long and Short Axis */
    writeCommand(Registers::ELL_A0);
    writeData(longAxis & 0xff);
    writeCommand(Registers::ELL_A1);
    writeData(longAxis >> 8);
    writeCommand(Registers::ELL_B0);
    writeData(shortAxis & 0xff);
    writeCommand(Registers::ELL_B1);
    writeData(shortAxis >> 8);

    /* Set Color */
    writeCommand(Registers::FGCR0);
    writeData(color.red5());
    writeCommand(Registers::FGCR1);
    writeData(color.green6());
    writeCommand(Registers::FGCR2);
    writeData(color.blue5());

    /* Draw! */
    writeCommand(Registers::DECSC);
    if (curvePart <= 0x03) {
        if (filled) {
            writeData(0xD0 | (curvePart & 0x03));
        } else {
            writeData(0x90 | (curvePart & 0x03));
        }
    } else {
        if (filled) {
            writeData(0xC0);
        } else {
            writeData(0x80);
        }
    }

    /* Wait for the command to finish */
    return waitPoll(Registers::DECSC, 0x80);
}

}// namespace obd::gfx
