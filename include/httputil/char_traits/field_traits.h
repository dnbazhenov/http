#pragma once
#include <httputil/char_traits/core_traits.h>

/*
 * RFC9110 field related rules
 */

namespace httputil::char_traits
{
	// field-related char traits
	enum
	{
		CTEXT = 0x01,
		ETAGC = 0x02,
		QDTEXT = 0x04,
		TCHAR = 0x08,
		TCHAR68 = 0x10
	};

	// type to store URI-related traits
	using field_traits_t = unsigned char;

	// obs-text = %x80-FF
	static constexpr bool is_obs_text(char ch)
	{
		unsigned char uch = ch;
		return uch >= 0x80;
	}

	namespace details
	{
		// ctext = HTAB / SP / %x21-27 ; '!'-'''
		//       / %x2A-5B ; '*'-'['
		//       / %x5D-7E ; ']'-'~'
		//       / obs-text
		static constexpr bool is_ctext(char ch)
		{
			switch (ch)
			{
				case HTAB:
				case SP:
				case 0x21 ... 0x27:
				case 0x2A ... 0x5B:
				case 0x5D ... 0x7E:
					return true;
				default:
					return is_obs_text(ch);
			}
		}

		// etagc = "!" / %x23-7E ; '#'-'~'
		//       / obs-text
		static constexpr bool is_etagc(char ch)
		{
			switch (ch)
			{
				case '!':
				case 0x23 ... 0x7E:
					return true;
				default:
					return is_obs_text(ch);
			}
		}

		// qdtext = HTAB / SP / "!" / %x23-5B ; '#'-'['
		//        / %x5D-7E ; ']'-'~'
		//        / obs-text
		static constexpr bool is_qdtext(char ch)
		{
			switch (ch)
			{
				case HTAB:
				case SP:
				case '!':
				case 0x23 ... 0x5B:
				case 0x5D ... 0x7E:
					return true;
				default:
					return is_obs_text(ch);
			}
		}

		// tchar = "!" / "#" / "$" / "%" / "&" / "'" / "*" / "+" / "-" / "."
		//       / "^" / "_" / "`" / "|" / "~" / DIGIT / ALPHA
		static constexpr bool is_tchar(char ch)
		{
			switch (ch)
			{
				case '!':
				case '#':
				case '$':
				case '%':
				case '&':
				case '\'':
				case '*':
				case '+':
				case '-':
				case '.':
				case '^':
				case '_':
				case '`':
				case '|':
				case '~':
					return true;
				default:
					return is_digit(ch) || is_alpha(ch);
			}
		}

		// tchar68 = ALPHA / DIGIT / "-" / "." / "_" / "~" / "+" / "/"
		static constexpr bool is_tchar68(char ch)
		{
			switch (ch)
			{
				case '-':
				case '.':
				case '_':
				case '~':
				case '+':
				case '/':
					return true;
				default:
					return is_digit(ch) || is_alpha(ch);
			}
		}

		static constexpr field_traits_t field_traits(char ch)
		{
			field_traits_t t = 0;

			if (is_ctext(ch))
				t |= CTEXT;
			if (is_etagc(ch))
				t |= ETAGC;
			if (is_qdtext(ch))
				t |= QDTEXT;
			if (is_tchar(ch))
				t |= TCHAR;
			if (is_tchar68(ch))
				t |= TCHAR68;

			return t;
		}

		extern const std::array<field_traits_t, 256> __field_traits;
	}  // namespace details

	static constexpr field_traits_t field_traits(char ch)
	{
		if constexpr (__builtin_constant_p(ch))
			return details::field_traits(ch);
		else
			return details::__field_traits[ch];
	}

	static constexpr bool is_ctext(char ch) { return field_traits(ch) & CTEXT; }
	static constexpr bool is_etagc(char ch) { return field_traits(ch) & ETAGC; }
	static constexpr bool is_qdtext(char ch) { return field_traits(ch) & QDTEXT; }
	static constexpr bool is_tchar(char ch) { return field_traits(ch) & TCHAR; }
	static constexpr bool is_tchar68(char ch) { return field_traits(ch) & TCHAR68; }
	static constexpr bool is_field_vchar(char ch) { return is_obs_text(ch) || is_vchar(ch); }

}  // namespace httputil::char_traits
