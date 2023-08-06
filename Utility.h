#pragma once

#include <cstdint>
#include <string>

namespace Utility {

	void Print(const std::string& msg);
	void Print(const std::wstring& msg);

	int32_t MsgBox(const std::string& msg, const std::string& caption, uint32_t type = 0u);
	int32_t MsgBox(const std::wstring& msg, const std::wstring& caption, uint32_t type = 0u);

	std::wstring ConvertString(const std::string& str);
	std::string ConvertString(const std::wstring& str);

}