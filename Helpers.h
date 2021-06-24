#pragma once
#include <array>

namespace Helpers
{
	constexpr int utf8SeqLen(char firstByte) noexcept
	{
		return (firstByte & 0x80) == 0x00 ? 1 :
			(firstByte & 0xE0) == 0xC0 ? 2 :
			(firstByte & 0xF0) == 0xE0 ? 3 :
			(firstByte & 0xF8) == 0xF0 ? 4 :
			-1;
	}

	constexpr auto utf8Substr(char* start, char* end, int n) noexcept
	{
		while (start < end && --n)
			start += utf8SeqLen(*start);
		return start;
	}
}
