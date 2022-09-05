#include <utfia/iterator.h>

#include <ut/assert.h>
#include <ut/test.h>

namespace utfia
{
	namespace tests
	{
		namespace
		{
			const utf8::codepoint c_invalid = 0xFFFD;
		}

		begin_test_suite( UTF8IterationTests )
			test( ANSICharactersAreReadAsIs )
			{
				for (unsigned char i = 0u; i != 0x80; ++i)
				{
				// INIT
					char c = static_cast<char>(i);
					char *iterator = &c;
					char *const end = iterator + 1;

				// ACT / ASSERT
					assert_equal(i, utf8::next(iterator, end));
					assert_equal(end, iterator);
				}
			}


			test( InvalidCharacterIsReturnedWhenInvalidSingleCharIsMet )
			{
				for (unsigned char i = 0x80u; i != 0xC0u; ++i)
				{
				// INIT
					char c = static_cast<char>(i);
					char *iterator = &c;
					char *const end = iterator + 1;

				// ACT / ASSERT
					assert_equal(c_invalid, utf8::next(iterator, end));
					assert_equal(end, iterator);
					iterator = &c;
					assert_equal(c_invalid, utf8::next(iterator, end));
					assert_equal(end, iterator);
				}
				for (unsigned int i = 0xF8u; i != 0x100u; ++i)
				{
				// INIT
					char c = static_cast<char>(i);
					char *iterator = &c;
					char *const end = iterator + 1;

				// ACT / ASSERT
					assert_equal(c_invalid, utf8::next(iterator, end));
					assert_equal(end, iterator);
				}
			}


			test( DoubleByteCharactersAreParsedAndIteratorIsMovedAccordingly )
			{
				for (unsigned char i0 = 0xC0u; i0 != 0xE0; ++ i0)
				{
					for (unsigned char i1 = 0x80u; i1 != 0xC0u; ++i1)
					{
				// INIT
						char c[] = {	static_cast<char>(i0), static_cast<char>(i1),	};
						char *iterator = c;
						char *const end = iterator + 2;

				// ACT / ASSERT
						assert_equal(((i0 & 0x1Fu) << 6) + (i1 & 0x3Fu), utf8::next(iterator, end));
						assert_equal(end, iterator);
					}
				}
			}


			test( InsufficientLengthOfRangeSpitsInvalidCharacterAndPositionsIteratorToEnd )
			{
				// INIT
				char c[] = {	'\0', '\x81', '\x81', '\x81',	};
				char *iterator = c;

				// ACT / ASSERT
				c[0] = '\xC0', iterator = c;
				assert_equal(c_invalid, utf8::next(iterator, c + 1));
				assert_equal(c + 1, iterator);
				c[0] = '\xDF', iterator = c;
				assert_equal(c_invalid, utf8::next(iterator, c + 1));
				assert_equal(c + 1, iterator);
				c[0] = '\xE0', iterator = c;
				assert_equal(c_invalid, utf8::next(iterator, c + 2));
				assert_equal(c + 2, iterator);
				c[0] = '\xEF', iterator = c;
				assert_equal(c_invalid, utf8::next(iterator, c + 2));
				assert_equal(c + 2, iterator);
				c[0] = '\xF0', iterator = c;
				assert_equal(c_invalid, utf8::next(iterator, c + 3));
				assert_equal(c + 3, iterator);
				c[0] = '\xF7', iterator = c;
				assert_equal(c_invalid, utf8::next(iterator, c + 3));
				assert_equal(c + 3, iterator);
			}


			test( InvalidContinuationBytesLeadToInvalidCharacter )
			{
				for (unsigned char i = 0x00u; i != 0x80u; ++i)
				{
			// INIT
					char c[10] = {	0, static_cast<char>(i),	};
					char *iterator = c;

			// ACT / ASSERT
					c[0] = '\xC0', iterator = c;
					assert_equal(c_invalid, utf8::next(iterator, c + 10));
					assert_equal(c + 1, iterator);
					c[0] = '\xDF', iterator = c;
					assert_equal(c_invalid, utf8::next(iterator, c + 10));
					assert_equal(c + 1, iterator);
				}
				for (unsigned int i = 0xC0u; i != 0x100u; ++i)
				{
			// INIT
					char c[10] = {	0, static_cast<char>(i),	};
					char *iterator = c;

			// ACT / ASSERT
					c[0] = '\xC0', iterator = c;
					assert_equal(c_invalid, utf8::next(iterator, c + 10));
					assert_equal(c + 1, iterator);
					c[0] = '\xDF', iterator = c;
					assert_equal(c_invalid, utf8::next(iterator, c + 10));
					assert_equal(c + 1, iterator);
				}
				for (unsigned int i = 0xC0u; i != 0x100u; ++i)
				{
			// INIT
					char c[10] = {	0, static_cast<char>(i),	};
					char *iterator = c;

			// ACT / ASSERT
					c[0] = '\xE0', iterator = c;
					assert_equal(c_invalid, utf8::next(iterator, c + 10));
					assert_equal(c + 1, iterator);
					c[0] = '\xEF', iterator = c;
					assert_equal(c_invalid, utf8::next(iterator, c + 10));
					assert_equal(c + 1, iterator);
				}
				for (unsigned int i = 0xC0u; i != 0x100u; ++i)
				{
			// INIT
					char c[10] = {	0, static_cast<char>(i),	};
					char *iterator = c;

			// ACT / ASSERT
					c[0] = '\xF0', iterator = c;
					assert_equal(c_invalid, utf8::next(iterator, c + 10));
					assert_equal(c + 1, iterator);
					c[0] = '\xF7', iterator = c;
					assert_equal(c_invalid, utf8::next(iterator, c + 10));
					assert_equal(c + 1, iterator);
				}
			}


			test( InvalidContinuationStopsAtErroredCharacter )
			{
				// INIT
				unsigned char c[10] = {	0xE0, 0x83, 'a', 'b', 'c', 0xEF, 0x80, 'c', 'd', 'e',	};
				unsigned char *iterator = c;

				// ACT
				assert_equal(c_invalid, utf8::next(iterator, c + 10));

				// ASSERT
				assert_equal(c + 2, iterator);

				// INIT
				iterator = c + 5;

				// ACT
				assert_equal(c_invalid, utf8::next(iterator, c + 10));

				// ASSERT
				assert_equal(c + 7, iterator);

				// INIT
				c[0] = 0xF0, c[2] = 0x81, c[5] = 0xF7, c[7] = 0xBF;
				iterator = c;

				// ACT
				assert_equal(c_invalid, utf8::next(iterator, c + 10));

				// ASSERT
				assert_equal(c + 3, iterator);

				// INIT
				iterator = c + 5;

				// ACT
				assert_equal(c_invalid, utf8::next(iterator, c + 10));

				// ASSERT
				assert_equal(c + 8, iterator);
			}


			test( SomeSequencesDecoding )
			{
				// INIT
				const char checkmark[] = "\xE2\x9C\x93";
				const char *iterator = checkmark;

				// ACT / ASSERT
				assert_equal(0x2713u, utf8::next(iterator, checkmark + 4));
				assert_equal(checkmark + 3, iterator);

				// INIT
				const char grave_a[] = "\xC3\xA0";
				iterator = grave_a;

				// ACT / ASSERT
				assert_equal(0xE0u, utf8::next(iterator, grave_a + 3));
				assert_equal(grave_a + 2, iterator);

				// INIT
				const char u10030[] = "\xF0\x90\x80\xB0";
				iterator = u10030;

				// ACT / ASSERT
				assert_equal(0x10030u, utf8::next(iterator, u10030 + 5));
				assert_equal(u10030 + 4, iterator);
			}
		end_test_suite
	}
}
