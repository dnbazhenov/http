#pragma once
#include <array>
#include <type_traits>

/*
 * RFC3986
 * absolute-URI  = scheme ":" hier-part [ "?" query ]
 * authority     = [ userinfo "@" ] host [ ":" port ]
 *
 * hier-part     = "//" authority path-abempty
 *				 / path-absolute
 *				 / path-rootless
 *				 / path-empty
 * scheme        = ALPHA *( ALPHA / DIGIT / "+" / "-" / "." )
 * userinfo      = *( unreserved / pct-encoded / sub-delims / ":" )
 * host          = IP-literal / IPv4address / reg-name
 * port          = *DIGIT
 * path-abempty  = *( "/" segment )
 * path-absolute = "/" [ segment-nz *( "/" segment ) ]
 * path-rootless = segment-nz *( "/" segment )
 * path-empty    = 0<pchar>
 * segment       = *pchar
 * segment-nz    = 1*pchar
 * pchar         = unreserved / pct-encoded / sub-delims / ":" / "@"
 *
 * query         = *( pchar / "/" / "?" )
 *
 * unreserved    = ALPHA / DIGIT / "-" / "." / "_" / "~"
 * reserved      = gen-delims / sub-delims
 * sub-delims    = "!" / "$" / "&" / "'" / "(" / ")"
 *				 / "*" / "+" / "," / ";" / "="
 * reg-name      = *( unreserved / pct-encoded / sub-delims )
 * pct-encoded   = "%" HEXDIG HEXDIG
 */

namespace http
{

	typedef unsigned short char_flags_t;

	namespace impl
	{
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

		static constexpr bool is_hexdig(char ch)
		{
			switch (ch)
			{
				case '0' ... '9':
				case 'A' ... 'F':
				case 'a' ... 'f':
					return true;
				default:
					return false;
			}
		}

		static constexpr bool is_pct(char ch) { return ch == '%'; }

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
					return is_alpha(ch) || is_digit(ch) || is_pct(ch);
			}
		}

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

		static constexpr bool is_uchar(char ch)
		{
			return ch == ':' || is_unreserved(ch) || is_pct(ch) || is_sub_delim(ch);
		}

		static constexpr bool is_pchar(char ch) { return ch == '@' || is_uchar(ch); }

		static constexpr bool is_qchar(char ch) { return ch == '/' || ch == '?' || is_pchar(ch); }

		enum
		{
			ALPHA = 0x0001,
			DIGIT = 0x0002,
			HEXDIG = 0x0004,
			PCT = 0x0008,  // %
			DELIM = 0x0010,
			SUBDELIM = 0x0020,
			CTL = 0x0040,
			OWS = 0x0080,
			UNRESERVED = 0x0100,
			SCHAR = 0x0200,  // scheme char
			UCHAR = 0x0400,  // user info char
			PCHAR = 0x0800,  // path char
			QCHAR = 0x1000   // query char
		};

		static constexpr char_flags_t  char_traits(char ch)
		{
			char_flags_t t = 0;

			if (is_alpha(ch))
				t |= ALPHA;
			if (is_digit(ch))
				t |= DIGIT;
			if (is_hexdig(ch))
				t |= HEXDIG;
			if (is_pct(ch))
				t |= PCT;
			if (is_gen_delim(ch))
				t |= DELIM;
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

		static constexpr std::array<char_flags_t, 256> mkchartraits()
		{
			std::array<char_flags_t, 256> t{ 0 };

			for (auto i = 0; i < 256; i++) {
				t[i] = char_traits(i);
			}

			return t;
		}

		extern const std::array<char_flags_t, 256> __char_traits;

	}  // namespace impl

	static constexpr char_flags_t char_traits(char ch)
	{
		if constexpr (__builtin_constant_p(ch))
			return impl::char_traits(ch);
		else
			return impl::__char_traits[ch];
	}

	static constexpr bool is_alpha(char ch) { return char_traits(ch) & impl::ALPHA; }

	static constexpr bool is_digit(char ch) { return char_traits(ch) & impl::DIGIT; }

	static constexpr bool is_hexdig(char ch) { return char_traits(ch) & impl::HEXDIG; }

	static constexpr bool is_pct(char ch) { return ch == '%'; }

	static constexpr bool is_gen_delim(char ch) { return char_traits(ch) & impl::DELIM; }

	static constexpr bool is_sub_delim(char ch) { return char_traits(ch) & impl::SUBDELIM; }

	static constexpr bool is_unreserved(char ch) { return char_traits(ch) & impl::UNRESERVED; }

	static constexpr bool is_schar(char ch) { return char_traits(ch) & impl::SCHAR; }

	static constexpr bool is_uchar(char ch) { return char_traits(ch) & impl::UCHAR; }

	static constexpr bool is_hchar(char ch)
	{
		return char_traits(ch) & (impl::UNRESERVED | impl::PCT | impl::SUBDELIM);
	}

	static constexpr bool is_pchar(char ch) { return char_traits(ch) & impl::PCHAR; }

	static constexpr bool is_qchar(char ch) { return char_traits(ch) & impl::QCHAR; }

	static constexpr bool is_reserved(char ch)
	{
		return char_traits(ch) & (impl::DELIM | impl::SUBDELIM);
	}
}  // namespace http
