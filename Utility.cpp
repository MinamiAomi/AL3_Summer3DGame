#include "Utility.h"

#include <Windows.h>

namespace Utility {

    void Print(const std::string& msg) {
        OutputDebugStringA(msg.c_str());
    }

    void Print(const std::wstring& msg) {
        OutputDebugStringW(msg.c_str());
    }

    int32_t MsgBox(const std::string& msg, const std::string& caption, uint32_t type) {
        MessageBoxA(nullptr, msg.c_str(), caption.c_str(), type);
    }

    int32_t MsgBox(const std::wstring& msg, const std::wstring& caption, uint32_t type) {
        MessageBoxW(nullptr, msg.c_str(), caption.c_str(), type);
    }


    std::wstring ConvertString(const std::string& str) {
        if (str.empty()) {
            return std::wstring();
        }
        int sizeNeeded = MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<const char*>(&str[0]), static_cast<int>(str.size()), NULL, 0);
        if (sizeNeeded == 0) {
            return std::wstring();
        }
        std::wstring result(sizeNeeded, 0);
        MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<const char*>(&str[0]), static_cast<int>(str.size()), &result[0], sizeNeeded);
        return result;
    }

    std::string ConvertString(const std::wstring& str) {
        if (str.empty()) {
            return std::string();
        }
        int sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, str.data(), static_cast<int>(str.size()), NULL, 0, NULL, NULL);
        if (sizeNeeded == 0) {
            return std::string();
        }
        std::string result(sizeNeeded, 0);
        WideCharToMultiByte(CP_UTF8, 0, str.data(), static_cast<int>(str.size()), result.data(), sizeNeeded, NULL, NULL);
        return result;
    }

}

