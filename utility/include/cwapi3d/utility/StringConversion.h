#pragma once

#include <windows.h>

#include <string>
#include <string_view>

namespace CwAPI3D::Utility {

inline std::string ToUtf8(const std::wstring& value) {
    if (value.empty()) return {};

    // Ask Windows how many bytes we need
    // CP_UTF8 tells Windows to treat the source as UTF-8
    const int size_needed = WideCharToMultiByte(CP_UTF8, 0, value.data(), static_cast<int>(value.size()), nullptr, 0,
                                          nullptr, nullptr);

    std::string result(size_needed, 0);

    WideCharToMultiByte(CP_UTF8, 0, value.data(), static_cast<int>(value.size()), &result[0], size_needed,
                        nullptr, nullptr);

    return result;
}

inline std::wstring ToWString(const std::string_view utf8) {
    if (utf8.empty()) return {};

    // Ask Windows how many wide characters we need
    const int size_needed = MultiByteToWideChar(CP_UTF8, 0, utf8.data(), static_cast<int>(utf8.size()), nullptr, 0);

    std::wstring result(size_needed, 0);

    MultiByteToWideChar(CP_UTF8, 0, utf8.data(), static_cast<int>(utf8.size()), &result[0], size_needed);

    return result;
}

inline std::wstring ToWString(const std::string& utf8) { return ToWString(std::string_view(utf8)); }

inline std::wstring ToWString(const char* utf8) {
    return utf8 ? ToWString(std::string_view(utf8)) : std::wstring();
}

}  // namespace CwAPI3D::Utility
