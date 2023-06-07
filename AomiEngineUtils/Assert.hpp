#pragma once

#include <cassert>
#include <string>

#ifdef NDEBUG 
#define ASSERT_MSG(expression, message) (static_cast<void>(0))
#else
    static const std::wstring _assertMessagePrefix(L"\nMessage:");
    // メッセージを表示できるassert
    // 第一引数は通常のassertと同じ
    // 第二引数は文字列リテラル
#define ASSERT_MSG(expression, message) (static_cast<void>( \
            (!!(expression)) || \
            (_wassert((_CRT_WIDE(#expression) + _assertMessagePrefix + _CRT_WIDE(message)).c_str(), \
                      _CRT_WIDE(__FILE__), static_cast<unsigned int>(__LINE__)), 0)))
#endif
