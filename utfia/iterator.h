//	Copyright (c) 2011-2022 by Artem A. Gevorkyan (gevorkyan.org)
//
//	Permission is hereby granted, free of charge, to any person obtaining a copy
//	of this software and associated documentation files (the "Software"), to deal
//	in the Software without restriction, including without limitation the rights
//	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//	copies of the Software, and to permit persons to whom the Software is
//	furnished to do so, subject to the following conditions:
//
//	The above copyright notice and this permission notice shall be included in
//	all copies or substantial portions of the Software.
//
//	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//	THE SOFTWARE.

#pragma once

#include "config.h"

#include <cstddef>

namespace utfia
{
	struct utf8
	{
		typedef unsigned int codepoint;

		template <typename CharIteratorT>
		static codepoint next(CharIteratorT &iterator, CharIteratorT end);

	private:
		typedef unsigned char uchar;

		enum {	invalid = 0xFFFD	};

	private:
		template <typename CharIteratorT>
		static codepoint next_slow(CharIteratorT &iterator, std::size_t max_remainder, codepoint c);
	};



	template <typename CharIteratorT>
	UTFIA_INLINE utf8::codepoint utf8::next(CharIteratorT &iterator, CharIteratorT end)
	{
		codepoint c = static_cast<unsigned char>(*iterator++);

		return c < 0x80 ? c : next_slow(iterator, end - iterator, c);
	}

	template <typename CharIteratorT>
	inline utf8::codepoint utf8::next_slow(CharIteratorT &iterator, std::size_t max_remainder, codepoint c)
	{
		std::size_t remainder;
		CharIteratorT iterator_local = iterator;

		if (c < 0xC0)
			return invalid;
		else if (c < 0xE0)
			remainder = 1, c &= 0x1F;
		else if (c < 0xF0)
			remainder = 2, c &= 0x0F;
		else if (c < 0xF8)
			remainder = 3, c &= 0x07;
		else
			return invalid;
		if (max_remainder < remainder)
			return iterator = iterator_local + max_remainder, invalid;

		do
		{
			const codepoint continuation = static_cast<uchar>(*iterator_local);

			if ((continuation & 0xC0) != 0x80)
				return iterator = iterator_local, invalid;
			c <<= 6;
			c += continuation & 0x3F;
		} while (iterator_local++, --remainder);
		return iterator = iterator_local, c;
	}
}
