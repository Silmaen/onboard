/**
 * @file OString.h
 * @author argawaen
 * @date 21/01/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#ifdef ARDUINO
#include <WString.h>
#else
#include <string>
#endif

/**
 * @brief Abstraction class usable in all platforms
 */
#ifdef ARDUINO
class OString: public String {
#else
class OString: public std::string {
#endif
public:
    OString() = default;
    virtual ~OString() = default;
    /**
     * @brief Copy constructor
     */
    OString(const OString&) = default;
    /**
     * @brief Move constructor
     */
    OString(OString&&) = default;
    /**
     * @brief Copy assignment
     * @return this
     */
    OString& operator=(const OString&) = default;
    /**
     * @brief Move Assignement
     * @return this
     */
    OString& operator=(OString&&) = default;
#ifdef ARDUINO
    using size_type = unsigned int;
    OString(const char* str):String(str){}
    OString(const String& str):String(str){}
    OString(const __FlashStringHelper *str):String(str){}
    OString(int number):String(number) {}
    [[nodiscard]] bool empty()const{return isEmpty();}
    [[nodiscard]] OString substr(size_type begin,size_type end) const {return substring(begin, end);}
    [[nodiscard]] OString substr(size_type begin) const {return substring(begin);}
    [[nodiscard]] char back()const{return *(begin()+length()-1);}
    void pop_back(){remove(length()-1,1);}
    [[nodiscard]] size_type find(const OString& str, size_type from=0)const{return indexOf(str, from);}
    [[nodiscard]] size_type find(char str, size_type from=0)const{return indexOf(str, from);}
    static constexpr size_type npos = -1;
    void erase(const char* pos){int index=pos-c_str();remove(index,1);}
#else
    /**
     * @brief Constructor from char type
     * @param str The origin string
     */
    OString(const char* str):std::string(str){}
    /**
     * @brief Constructor by std::string
     * @param str The origin string
     */
    OString(const std::string& str):std::string(str){}
    /**
     * @brief std::string conversion
     * @return Converted string
     */
    [[nodiscard]] std::string s_str()const{return c_str();}
#endif
private:
};
