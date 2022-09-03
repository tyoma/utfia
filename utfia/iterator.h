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

namespace utfia
{
	struct utf8
	{
		typedef unsigned int codepoint;

		template <typename CharIteratorT>
		static codepoint next(CharIteratorT &iterator, CharIteratorT end, codepoint invalid = '?');

	private:
		template <typename CharIteratorT>
		static codepoint next_slow(CharIteratorT &iterator, CharIteratorT end, codepoint c, codepoint invalid);
	};



	template <typename CharIteratorT>
	UTFIA_INLINE utf8::codepoint utf8::next(CharIteratorT &iterator, CharIteratorT end, codepoint invalid)
	{
		codepoint c = static_cast<unsigned char>(*iterator);

		++iterator;
		return c < 0x80 ? c : next_slow(iterator, end, c, invalid);
	}

	template <typename CharIteratorT>
	UTFIA_AVOID_INLINE inline utf8::codepoint utf8::next_slow(CharIteratorT &iterator, CharIteratorT end, codepoint c, codepoint invalid)
	{
		typedef unsigned char uchar;

		uchar remainder;

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

		for (bool continuation_valid = true; remainder--; ++iterator)
		{
			if (iterator == end)
				return invalid;

			const uchar continuation = static_cast<uchar>(*iterator);

			if (((continuation & 0xC0) == 0x80) & continuation_valid)
				c <<= 6, c += continuation & 0x3F;
			else
				continuation_valid = false, c = invalid;
		}
		return c;
	}
}
