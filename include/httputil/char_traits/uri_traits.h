#pragma once
#include <httputil/char_traits/core_traits.h>

/*
 * RFC3986 rules
 */

namespace httputil::char_traits
{
	// URI-related char traits
	enum
	{
		GENDELIM = 0x01,
		SUBDELIM = 0x02,
		UNRESERVED = 0x04,
		SCHAR = 0x08,  // scheme char
		UCHAR = 0x10,  // userinfo char
		PCHAR = 0x20,  // path char
		QCHAR = 0x40,  // query char
	};

	// type to store URI-related traits
	using uri_traits_t = unsigned char;

	namespace details
	{
		// gen-delims = ":" / "/" / "?" / "#" / "[" / "]" / "@"
		static constexpr bool is_gen_delim(char ch)
		{
			switch (ch)
			{
				case ':':
				case '/':
				case '?':
				case '#':
				case '[':
				case ']':
				case '@':
					return true;
				default:
					return false;
			}
		}

		// sub-delims = "!" / "$" / "&" / "'" / "(" / ")"
		//            / "*" / "+" / "," / ";" / "="
		static constexpr bool is_sub_delim(char ch)
		{
			switch (ch)
			{
				case '!':
				case '$':
				case '&':
				case '\'':
				case '(':
				case ')':
				case '*':
				case '+':
				case ',':
				case ';':
				case '=':
					return true;
				default:
					return false;
			}
		}

		// unreserved = ALPHA / DIGIT / "-" / "." / "_" / "~"
		static constexpr bool is_unreserved(char ch)
		{
			switch (ch)
			{
				case '-':
				case '.':
				case '_':
				case '~':
					return true;
				default:
					return is_alpha(ch) || is_digit(ch);
			}
		}

		// schar = ALPHA / DIGIT / "+" / "-" / "."
		static constexpr bool is_schar(char ch)
		{
			switch (ch)
			{
				case '+':
				case '-':
				case '.':
					return true;
				default:
					return is_alpha(ch) || is_digit(ch);
			}
		}

		// pct-char   = "%" / HEXDIG
		static constexpr bool is_pct_encoded(char ch) { return ch == '%' || is_hexdig(ch); }

		// uchar = unreserved / pct-encoded / sub-delims / ":"
		static constexpr bool is_uchar(char ch)
		{
			return ch == ':' || is_unreserved(ch) || is_pct_encoded(ch) || is_sub_delim(ch);
		}

		// pchar = unreserved / pct-encoded / sub-delims / ":" / "@"
		static constexpr bool is_pchar(char ch) { return ch == '@' || is_uchar(ch); }

		// qchar = pchar / "/" / "?"
		static constexpr bool is_qchar(char ch) { return ch == '/' || ch == '?' || is_pchar(ch); }

		static constexpr uri_traits_t uri_traits(char ch)
		{
			uri_traits_t t = 0;

			if (is_gen_delim(ch))
				t |= GENDELIM;
			if (is_sub_delim(ch))
				t |= SUBDELIM;
			if (is_unreserved(ch))
				t |= UNRESERVED;
			if (is_schar(ch))
				t |= SCHAR;
			if (is_uchar(ch))
				t |= UCHAR;
			if (is_pchar(ch))
				t |= PCHAR;
			if (is_qchar(ch))
				t |= QCHAR;

			return t;
		}

		extern const std::array<uri_traits_t, 128> __uri_traits;
	}  // namespace details

	static constexpr uri_traits_t uri_traits(char ch)
	{
		if constexpr (__builtin_constant_p(ch))
			return details::uri_traits(ch);
		else if (ch >= 0)
			return details::__uri_traits[ch];
		else
			return 0;
	}

	static constexpr bool is_gen_delim(char ch) { return uri_traits(ch) & GENDELIM; }
	static constexpr bool is_sub_delim(char ch) { return uri_traits(ch) & SUBDELIM; }
	static constexpr bool is_reserved(char ch) { return uri_traits(ch) & (GENDELIM | SUBDELIM); }
	static constexpr bool is_unreserved(char ch) { return uri_traits(ch) & UNRESERVED; }
	static constexpr bool is_schar(char ch) { return uri_traits(ch) & SCHAR; }
	static constexpr bool is_uchar(char ch) { return uri_traits(ch) & UCHAR; }
	static constexpr bool is_hchar(char ch) { return uri_traits(ch) & (UNRESERVED | SUBDELIM); }
	static constexpr bool is_pchar(char ch) { return uri_traits(ch) & PCHAR; }
	static constexpr bool is_qchar(char ch) { return uri_traits(ch) & QCHAR; }

}  // namespace httputil::char_traits
