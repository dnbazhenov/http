#pragma once
#include <array>
#include <type_traits>

/*
 * RFC5234 core rules
 */

namespace httputil::char_traits
{
	// CR = %x0D
	static constexpr char CR = '\r';
	// DQUOTE = %x22
	static constexpr char DQUOTE = '"';
	// HTAB = %x09
	static constexpr char HTAB = '\t';
	// LF = %x0A
	static constexpr char LF = '\n';
	// SP = %x20
	static constexpr char SP = ' ';

	// core char traits
	enum
	{
		ALPHA = 0x01,
		BIT = 0x02,
		CHAR = 0x04,
		CTL = 0x08,
		DIGIT = 0x10,
		HEXDIG = 0x20,
		VCHAR = 0x40,
		WSP = 0x80
	};

	// type to store core char traits
	using core_traits_t = unsigned char;

	namespace details
	{
		// ALPHA = %x41-5A / %x61-7A ; A-Z / a-z
		static constexpr bool is_alpha(char ch)
		{
			switch (ch)
			{
				case 'A' ... 'Z':
				case 'a' ... 'z':
					return true;
				default:
					return false;
			}
		}

		// BIT = "0" / "1"
		static constexpr bool is_bit(char ch) { return ch == '0' || ch == '1'; }

		// CHAR = %x01-7F
		static constexpr bool is_char(char ch) { return ch > 0 && ch <= 0x7F; }

		// CTL =  %x00-1F / %x7F
		static constexpr bool is_ctl(char ch) { return (ch >= 0 && ch <= 0x1F) || ch == 0x7F; }

		// DIGIT = %x30-39 ; 0-9
		static constexpr bool is_digit(char ch)
		{
			switch (ch)
			{
				case '0' ... '9':
					return true;
				default:
					return false;
			}
		}

		// HEXDIG = DIGIT / "A" / "B" / "C" / "D" / "E" / "F"
		static constexpr bool is_hexdig(char ch)
		{
			switch (ch)
			{
				case '0' ... '9':
				case 'A' ... 'F':
					return true;
				default:
					return false;
			}
		}

		// VCHAR =  %x21-7E
		static constexpr bool is_vchar(char ch) { return ch > 0x21 && ch <= 0x7E; }

		// WSP = SP / HTAB
		static constexpr bool is_wsp(char ch) { return ch == SP || ch == HTAB; }

		static constexpr core_traits_t core_traits(char ch)
		{
			core_traits_t t = 0;

			if (is_alpha(ch))
				t |= ALPHA;
			if (is_bit(ch))
				t |= BIT;
			if (is_char(ch))
				t |= CHAR;
			if (is_ctl(ch))
				t |= CTL;
			if (is_digit(ch))
				t |= DIGIT;
			if (is_hexdig(ch))
				t |= HEXDIG;
			if (is_vchar(ch))
				t |= VCHAR;
			if (is_wsp(ch))
				t |= WSP;

			return t;
		}

		extern const std::array<core_traits_t, 128> __core_traits;
	}  // namespace details

	template<typename _Tp>
	static constexpr std::array<_Tp, 128> mktraits(_Tp (*fn)(char))
	{
		std::array<_Tp, 128> t{ 0 };

		for (auto i = 0; i < 128; i++)
		{
			t[i] = fn(i);
		}

		return t;
	}

	static constexpr core_traits_t core_traits(char ch)
	{
		if constexpr (__builtin_constant_p(ch))
			return details::core_traits(ch);
		else if (ch >= 0)
			return details::__core_traits[ch];
		else
			return 0;
	}

	static constexpr bool is_alpha(char ch) { return core_traits(ch) & ALPHA; }
	static constexpr bool is_bit(char ch) { return core_traits(ch) & BIT; }
	static constexpr bool is_char(char ch) { return core_traits(ch) & CHAR; }
	static constexpr bool is_ctl(char ch) { return core_traits(ch) & CTL; }
	static constexpr bool is_digit(char ch) { return core_traits(ch) & DIGIT; }
	static constexpr bool is_hexdig(char ch) { return core_traits(ch) & HEXDIG; }
	static constexpr bool is_vchar(char ch) { return core_traits(ch) & VCHAR; }
	static constexpr bool is_wsp(char ch) { return core_traits(ch) & WSP; }

}  // namespace httputil::char_traits
