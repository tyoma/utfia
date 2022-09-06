#include <utfia/string.h>

#include <iterator>
#include <ut/assert.h>
#include <ut/test.h>

using namespace std;

namespace utfia
{
	namespace tests
	{
		namespace
		{
			struct trivial
			{
				template <typename I>
				static typename iterator_traits<I>::value_type next(I &iterator_, I /*end*/)
				{	return *iterator_++;	}
			};

			struct tri_compare
			{
				template <typename T>
				int operator ()(T lhs, T rhs) const
				{	return lhs < rhs ? -1 : rhs < lhs ? +1 : 0;	}
			};

			struct tri_compare_i
			{
				template <typename T>
				int operator ()(T lhs, T rhs) const
				{	return rhs < lhs ? -1 : lhs < rhs ? +1 : 0;	}
			};
		}

		begin_test_suite( StringTests )
			test( EqualStringsCompareWithZero )
			{
				// INIT / ACT / ASSERT
				assert_equal(0, compare<trivial>("", string(), tri_compare()));
				assert_equal(0, compare<trivial>("foo", "foo", tri_compare()));
				assert_equal(0, compare<trivial>("fooBAR", string("fooBAR"), tri_compare()));
				assert_equal(0, compare<trivial>(string("foobazbar"), "foobazbar", tri_compare()));
				assert_equal(0, compare<trivial>(string("zanzibar"), string("zanzibar"), tri_compare()));
			}


			test( ShorterSubSequencesAreLess )
			{
				// INIT / ACT / ASSERT
				assert_is_true(0 > compare<trivial>("fo", "foo", tri_compare()));
				assert_is_true(0 > compare<trivial>("fooB", "fooBAR", tri_compare()));
				assert_is_true(0 > compare<trivial>("foobaz", "foobazbar", tri_compare()));
				assert_is_true(0 < compare<trivial>("foo", "fo", tri_compare()));
				assert_is_true(0 < compare<trivial>("fooBAR", "fooB", tri_compare()));
				assert_is_true(0 < compare<trivial>("foobazbar", "foobaz", tri_compare()));
				assert_is_true(0 > compare<trivial>("fo", "foo", tri_compare_i()));
				assert_is_true(0 > compare<trivial>("fooB", "fooBAR", tri_compare_i()));
				assert_is_true(0 > compare<trivial>("foobaz", "foobazbar", tri_compare_i()));
				assert_is_true(0 < compare<trivial>("foo", "fo", tri_compare_i()));
				assert_is_true(0 < compare<trivial>("fooBAR", "fooB", tri_compare_i()));
				assert_is_true(0 < compare<trivial>("foobazbar", "foobaz", tri_compare_i()));
			}


			test( SmallerStringsAreLess )
			{
				// INIT / ACT / ASSERT
				assert_is_true(0 > compare<trivial>("Bar", "bar", tri_compare()));
				assert_is_true(0 < compare<trivial>("bar", "Bar", tri_compare()));
				assert_is_true(0 > compare<trivial>("foOo", "fooo", tri_compare()));
				assert_is_true(0 < compare<trivial>("foo", "foO", tri_compare()));
				assert_is_true(0 < compare<trivial>("foO", "foo", tri_compare_i()));
				assert_is_true(0 > compare<trivial>("foo", "foO", tri_compare_i()));
			}
		end_test_suite
	}
}
