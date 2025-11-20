#pragma once

#include <cwapi3d/w_string.h>

#include <codecvt>
#include <locale>
#include <string>
#include <string_view>

namespace CwAPI3D::Utility {

/// Convert cwapi3d w_string to UTF-8 std::string
inline std::string ToUtf8(const w_string& value) {
    std::wstring wide(value.c_str());
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
    return conv.to_bytes(wide);
}

/// Convert UTF-8 std::string_view to cwapi3d w_string
inline w_string ToWString(std::string_view utf8) {
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
    std::wstring wide = conv.from_bytes(utf8.data(), utf8.data() + utf8.size());
    return w_string(wide.c_str());
}

inline w_string ToWString(const std::string& utf8) {
    return ToWString(std::string_view(utf8));
}

inline w_string ToWString(const char* utf8) {
    return utf8 ? ToWString(std::string_view(utf8)) : w_string();
}

}  // namespace CwAPI3D::Utility
