#pragma once

#include <codecvt>
#include <locale>
#include <string>
#include <string_view>

namespace CwAPI3D::Utility {

/// Convert cwapi3d w_string to UTF-8 std::string
inline std::string ToUtf8(const std::wstring& value) {
    std::wstring wide(value.c_str());
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
    return conv.to_bytes(wide);
}

/// Convert UTF-8 std::string_view to cwapi3d w_string
inline std::wstring ToWString(std::string_view utf8) {
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
    std::wstring wide = conv.from_bytes(utf8.data(), utf8.data() + utf8.size());
    return std::wstring(wide.c_str());
}

inline std::wstring ToWString(const std::string& utf8) {
    return ToWString(std::string_view(utf8));
}

inline std::wstring ToWString(const char* utf8) {
    return utf8 ? ToWString(std::string_view(utf8)) : std::wstring();
}

}  // namespace CwAPI3D::Utility
