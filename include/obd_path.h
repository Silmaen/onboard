/**
 * \author argawaen 
 * \date 06/06/2021
 *
 */

#pragma once
#include <WString.h>
#include <utility>

namespace obd::filesystem {

/**
 * @brief class handling path
 */
class path {
public:
    /**
     * @brief constructor from String
     * @param from the string from which to initialize
     */
    explicit path(String from) :
        _path{std::move(from)} {};

    /**
     * @brief get the path as string
     * @return the path
     */
    [[nodiscard]] const String& get() const { return _path; }

    /**
     * @brief append to path with folder separator
     * @param app the part to append
     * @return this path
     */
    path& operator/=(const String& app) {
        _path += "/" + app;
        return *this;
    }
    /**
     * @brief append to path with folder separator
     * @param app the part to append
     * @return the new composed path
     */
    path operator/(const String& app) {
        path t(*this);
        t /= app;
        return t;
    }

    /**
     * @brief returns if the path is an absolute one
     * @return tru if the path is absolute
     */
    [[nodiscard]] bool isAbsolute() const { return _path.startsWith(F("/")); }

    /**
     * @brief make the path absolute according to the given base
     * @param base the base path
     */
    void makeAbsolute(const String& base = "/") {
        if (isAbsolute()) return;
        if (base.endsWith(F("/"))) {
            _path = base + _path;
        } else {
            _path = base + F("/") + _path;
        }
    }

    /**
     * @brief simply parse the path
     */
    void simplify() {
        if (!isAbsolute())
            makeAbsolute();
        if (_path.length() <= 1 || _path == F("/.") || _path == F("/..")) {
            _path = F("/");
            return;
        }
        if (_path.endsWith(F("/"))) {
            _path = _path.substring(0, _path.length() - 1);
            simplify();
        }
        // treat the single .
        if (_path.endsWith(F("/."))) {
            _path = _path.substring(0, _path.length() - 2);
            simplify();
        }
        _path.replace(F("/./"), F("/"));
        // treat double ..
        if (_path.endsWith(F("/.."))) {
            _path = _path.substring(0, _path.length() - 3);
            _path = _path.substring(0, _path.lastIndexOf(F("/")));
            simplify();
        }
        if (_path.startsWith(F("/../"))) {
            _path = _path.substring(3);
            simplify();
        }
        int idx = _path.indexOf("/../");
        if (idx >= 0){
            String base = _path.substring(0,idx);
            base = base.substring(0, base.lastIndexOf(F("/")));
            _path = base + _path.substring(idx + 3);
            simplify();
        }
    }

private:
    String _path{"/"};
};

}// namespace obd::filesystem
