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

namespace utfia
{
	template <typename T> inline typename T::const_iterator range_begin(const T &c) {	return c.begin();	}
	template <typename T> inline typename T::const_iterator range_end(const T &c) {	return c.end();	}
	template <typename E> inline E *range_begin(E *cstring) {	return cstring;	}
	template <typename E> inline E *range_end(E *cstring) {	while (*cstring) cstring++;	return cstring;	}

	template <typename NextT, typename I1, typename I2, typename P>
	inline int compare(I1 first1, I1 last1, I2 first2, I2 last2, const P &predicate)
	{
		while ((first1 != last1) & (first2 != last2))
			if (int result = predicate(NextT::next(first1, last1), NextT::next(first2, last2)))
				return result;
		return first2 != last2 ? -1 : first1 != last1 ? 1 : 0;
	}

	template <typename NextT, typename T1, typename T2, typename P>
	inline int compare(const T1 &lhs, const T2 &rhs, const P &predicate)
	{	return compare<NextT>(range_begin(lhs), range_end(lhs), range_begin(rhs), range_end(rhs), predicate);	}
}
