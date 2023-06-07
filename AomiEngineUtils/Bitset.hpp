#pragma once

#include <bit>
#include <cstdint>

#include "Assert.h"

namespace AomiEngine {

	template<size_t BitCount>
	class Bitset {
		static_assert(BitCount > 0, "A bitset with 0 bits is not possible.");
	public:
		inline Bitset() {
			Reset();
		}

		// 先頭から探す
		inline size_t FindFirst(bool value = false) {
			for (size_t i = 0; i < kWordCount; ++i) {
				if (words_[i] != (value ? 0 : ~0)) {
					size_t bitIndex = static_cast<size_t>(value ? std::countr_zero(words_[i]) : std::countr_one(words_[i]));
					if (bitIndex < kWordSize) {
						return i * kWordSize + bitIndex;
					}
				}
			}
			return kWordCount * kWordSize;
		}
		// 指定したビットの後ろから探す
		inline size_t FindNext(size_t index, bool value = false) {
			return 0u;
		}
		// すべて反転
		inline Bitset& Flip() {
			for (auto& word : words_) {
				word = ~word;
			}
			return *this;
		}
		// 指定したビットを反転
		inline Bitset& Flip(size_t bitIndex) {
			auto& word = GetWord(bitIndex);
			word = word ^ static_cast<uint64_t>(1) << (bitIndex & kWordMask);
			return *this;
		}
		// すべてのビットを変更
		inline Bitset& Set(bool value = true) {
			memset(words_, value ? ~0 : 0, sizeof(words_));
			return *this;
		}
		// 指定したビットを変更
		inline Bitset& Set(size_t bitIndex, bool value = true) {
			ASSERT_MSG(bitIndex < BitCount, "Out of range");
			uint64_t& word = GetWord(bitIndex);
			if (value) {
				word |= static_cast<uint64_t>(1) << (bitIndex & kWordMask);
			}
			else {
				word &= ~(static_cast<uint64_t>(1) << (bitIndex & kWordMask));
			}
			return *this;
		}
		// すべてのビットを0にする
		inline Bitset& Reset() {
			Set(false);
			return *this;
		}
		// 指定したビットを0にする
		inline Bitset& Reset(size_t bitIndex) {
			Set(bitIndex, false);
			return *this;
		}
		// 指定したビットを判定
		inline bool Test(size_t bitIndex) const {
			ASSERT_MSG(bitIndex < BitCount, "Out of range");
			return (GetWord(bitIndex) & (static_cast<uint64_t>(1) << (bitIndex & kWordMask))) != 0;
		}
		// すべてのビットを判定
		inline bool All(bool value = true) const {
			for (auto& word : words_) {
				if (word != (value ? ~0 : 0)) { return false; }
			}
			return true;
		}

		// ビット数
		inline size_t GetBitCount() const { return BitCount; }

	private:
		static constexpr size_t kWordSize = CHAR_BIT * sizeof(uint64_t);
		static constexpr size_t kWordCount = (BitCount - 1) / kWordSize + 1;
		static constexpr size_t kWordMask = kWordSize - 1;
		static constexpr size_t kBitIndexToWordIndex = 6;

		inline uint64_t& GetWord(size_t bitIndex) {
			return words_[bitIndex >> kBitIndexToWordIndex];
		}
		inline const uint64_t& GetWord(size_t bitIndex) const {
			return words_[bitIndex >> kBitIndexToWordIndex];
		}

		uint64_t words_[kWordCount];
	};

} // namespace AomiEngine
